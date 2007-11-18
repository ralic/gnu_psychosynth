/***************************************************************************
 *                                                                         *
 *   PSYCHOSYNTH                                                           *
 *   ===========                                                           *
 *                                                                         *
 *   Copyright (C) 2007 by Juan Pedro Bolivar Puente                       *
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#include "table/PatcherDynamic.h"

#include "object/Object.h"
#include "object/ObjectOutput.h"
#include "object/ObjectMixer.h"
#include "object/ObjectOscillator.h"

using namespace std;

const int PATCHER_ANY  = -1;

struct PatcherData {
    int socket_type;
    int src_socket;
    int dest_socket;
};

/*
 * Each object has several inputs and output sockets. On a dinamically patched synth we only
 * want one output connected with one input. This table tells the dinamic patcher which
 * output should be connected with which inpunt on each object.
 * The first dimension is the source object type and the second is the destiny object type.
 */
const PatcherData PATCHER_TABLE[N_OBJECTS][N_OBJECTS] =
{
    /* ObjectOutput */
    {
	{Object::LINK_NONE, 0, 0}, /* ObjectOutput */
	{Object::LINK_NONE, 0, 0}, /* ObjectMixer */
	{Object::LINK_NONE, 0, 0}  /* ObjectOscillator */
    },

    /* ObjectMixer */
    {
	{Object::LINK_AUDIO, ObjectMixer::OUT_A_OUTPUT, ObjectOutput::IN_A_INPUT},  /* ObjectOutput */
	{Object::LINK_AUDIO, ObjectMixer::OUT_A_OUTPUT, PATCHER_ANY}, /* ObjectMixer */
	{Object::LINK_NONE, 0, 0} /* ObjectOscillator */
    },

    /* ObjectOscillator */
    {
	{Object::LINK_NONE, 0, 0}, /* ObjectOutput */
	{Object::LINK_AUDIO, ObjectMixer::OUT_A_OUTPUT, PATCHER_ANY}, /* ObjectMixer */
	{Object::LINK_NONE, 0, 0} /* ObjectOscillator */
    }
};

PatcherDynamic::PatcherDynamic() :
    m_changed(false)
{
}

PatcherDynamic::~PatcherDynamic()
{
    for (multiset<Link*, LinkPtrCmp>::iterator i = m_links.begin(); i != m_links.end(); ++i)
	delete *i;
}

bool PatcherDynamic::addObject(Object* obj)
{
    if (m_nodes.insert(pair<int, Node>(obj->getID(), Node(obj))).second) {
	map<int, Node>::iterator i;
	int this_type = obj->getType();
	int other_type;
	
	for (i = m_nodes.begin(); i != m_nodes.end(); ++i) {
	    if (i->first == obj->getID())
		continue;
	    
	    other_type = (*i).second.obj->getType(); 

	    if (PATCHER_TABLE[this_type][other_type].socket_type != Object::LINK_NONE) {
		m_links.insert(new Link(obj, (*i).second.obj,
					obj->sqrDistanceTo(*(*i).second.obj),
					PATCHER_TABLE[this_type][other_type].socket_type,
					PATCHER_TABLE[this_type][other_type].src_socket,
					PATCHER_TABLE[this_type][other_type].dest_socket));
	    } else if (PATCHER_TABLE[other_type][this_type].socket_type != Object::LINK_NONE) {
		m_links.insert(new Link((*i).second.obj, obj,
					(*i).second.obj->sqrDistanceTo(*obj),
					PATCHER_TABLE[other_type][this_type].socket_type,
					PATCHER_TABLE[other_type][this_type].src_socket,
					PATCHER_TABLE[other_type][this_type].dest_socket));
	    }
	}
	
	m_changed = true;
	return true;
    }
    
    return false;
}

bool PatcherDynamic::deleteObject(Object* obj)
{
    if (m_nodes.erase(obj->getID())) {
	multiset<Link*>::iterator i, r;
	
	for (i = m_links.begin(); i != m_links.end();) {
	    if ((*i)->src == obj || (*i)->dest == obj) {
		r = i++;
		undoLink(**r);
		delete *r;
		m_links.erase(r);
	    } else
		++i;
	}

	m_changed = true;
	return true;
    }
    
    return false;
}

void PatcherDynamic::moveObject(Object* obj)
{
    multiset<Link*>::iterator i, r;
    list<Link*> readd;
    
    for (i = m_links.begin(); i != m_links.end();) {
	if ((*i)->src == obj || (*i)->dest == obj) {
	    r = i++;
	    (*r)->dist = (*r)->src->sqrDistanceTo(*(*r)->dest);
	    readd.push_back(*r);
	    m_links.erase(r);
	    m_changed = true;
	}
	else
	    ++i;
    }

    for (list<Link*>::iterator it = readd.begin(); it != readd.end(); ++it)
	m_links.insert(*it);
}

void PatcherDynamic::makeLink(Link& l)
{
    cout << "making link, source: " << l.src->getID() << " dest: "<< l.dest->getID() << endl;

    if (l.dest->getInSocket(l.sock_type, l.actual_in_sock).getSourceObject() != NULL)
	notifyLinkDeleted(PatcherEvent(l.src, l.dest, l.actual_in_sock, l.out_sock, l.sock_type));
      
    l.dest->connectIn(l.sock_type, l.actual_in_sock, l.src, l.out_sock);
    notifyLinkAdded(PatcherEvent(l.src, l.dest, l.actual_in_sock, l.out_sock, l.sock_type));
}

void PatcherDynamic::undoLink(Link& l)
{
    if (l.actual_in_sock >= 0) {
	if (l.dest->getInSocket(l.sock_type, l.actual_in_sock).getSourceObject() == l.src) {
	    cout << "undoing link, source: " << l.src->getID() << " dest: "<< l.dest->getID() << endl;
	    l.dest->connectIn(l.sock_type, l.actual_in_sock, NULL, l.out_sock);

	    notifyLinkDeleted(PatcherEvent(l.src, l.dest, l.actual_in_sock, l.out_sock, l.sock_type));
	}
    }
}

bool PatcherDynamic::isLinked(Link& l)
{
    if (l.actual_in_sock >= 0) {
	if (l.dest->getInSocket(l.sock_type, l.actual_in_sock).getSourceObject() == l.src)
	    return true;
    }
    return false;
}

void PatcherDynamic::findInSock(Link &l)
{
    float max_dist = l.dist;
    
    l.actual_in_sock = -1; 
    
    if (l.in_sock == -1) {
	for (int i = 0; i < l.dest->getNumInput(l.sock_type); i++) {
	    Object* obj = l.dest->getInSocket(l.sock_type, i).getSourceObject(); 
	    float dist;
	    
	    if (obj == NULL) {
		l.actual_in_sock = i;
		return;
	    }
	    else if ((dist = obj->sqrDistanceTo(*l.dest)) >= max_dist) {
		max_dist = dist;
		l.actual_in_sock = i;
	    }
	}
    } else {
	Object* obj = l.dest->getInSocket(l.sock_type, l.in_sock).getSourceObject(); 
	if (obj == NULL || obj->sqrDistanceTo(*l.dest))
	    l.actual_in_sock = l.in_sock;
    }
}

void PatcherDynamic::update()
{
    if (!m_changed)
	return;
    
    for (map<int, Node>::iterator i = m_nodes.begin(); i != m_nodes.end(); ++i)
	(*i).second.out_used = false;

    for (multiset<Link*>::iterator i = m_links.begin(); i != m_links.end(); ++i) {
	map<int, Node>::iterator n = m_nodes.find((*i)->src->getID());
	if (n == m_nodes.end()) {
	    WARNING("Object with id " << (*i)->src->getID() << "not found.");
	}
	
	Node& node = (*n).second;
	Link& link = **i;

	cout << "trying, src: " << link.src->getID() << " dest: " << link.dest->getID() << " out: " << node.out_used << endl; 

	if (!node.out_used) {
	    if (!isLinked(link)) {
		findInSock(link);
		if (link.actual_in_sock >= 0) {
		    makeLink(link);
		    node.out_used = true;
		}
	    } else
		node.out_used = true;
	} else {
	    undoLink(link);
	    link.actual_in_sock = -1;
	}
    }
    cout << "---" << endl;

    m_changed = false;
}

void PatcherDynamic::clear()
{
    for (multiset<Link*>::iterator i = m_links.begin(); i != m_links.end(); ++i)
	undoLink(**i);
}