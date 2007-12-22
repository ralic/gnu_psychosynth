/***************************************************************************
 *                                                                         *
 *   PSYCHOSYNTH                                                           *
 *   ===========                                                           *
 *                                                                         *
 *   Copyright (C) 2007 Juan Pedro Bolivar Puente                          *
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

#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <map>

#include "common/MapIterator.h"
#include "object/ObjectManager.h"
#include "object/ObjectOutput.h"

class ObjectManager {
    std::map<int, Object*> m_objmap;
    std::list<ObjectOutput*> m_outputs;
	
public:
    typedef MapIterator<int, Object*> Iterator;
    typedef MapConstIterator<int, Object*> ConstIterator;

    ObjectManager();
    ~ObjectManager();
	
    bool attachObject(Object* obj, int id);
    bool detachObject(int id);
    void detachObject(Iterator it);
    void remove(Iterator it);

    ConstIterator begin() const {
	return m_objmap.begin();
    }
    
    Iterator begin() {
	return m_objmap.begin();
    }

    ConstIterator end() const {
	return m_objmap.end();
    }
    
    Iterator end() {
	return m_objmap.end();
    }
    
    Iterator find(int id) {
	return m_objmap.find(id);
    }

    ConstIterator find(int id) const {
	return m_objmap.find(id);
    }

    /**
     * Makes a full new update of the objects. This means that it first resets
     * the is-updated property of the objects and then calls update() on all
     * the attached ObjectOutputs. The update is propagated via DFS. Some
     * objects may not be updated if not conected to a subgraph containing an
     * OutputObject.
     *
     * This function may be called by an OutputObject if a registered Output
     * system calls for new data and not enought data is availible in its
     * buffer.
     */
    void update();
};

#endif /* OBJECTMANAGER_H */
