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

#include "gui3d/SelectorWindow.h"

using namespace std;
using namespace CEGUI;

const float BUT_XGAP = 8;
const float BUT_YGAP = 8;
const float BUT_WIDTH = 100;
const float BUT_HEIGHT = 20;

SelectorWindow::Category::Button::Button(const std::string& name, 
					  ElementManager* mgr, int elem_type,
					  int index) :
    m_elem_type(elem_type),
    m_mgr(mgr),
    m_index(index)
{
    WindowManager& wmgr = WindowManager::getSingleton();
	
    m_window = wmgr.createWindow("TaharezLook/Button");
    m_window->setText    (name);
    m_window->setSize    ( UVector2(CEGUI::UDim(0, BUT_WIDTH), CEGUI::UDim(0, BUT_HEIGHT)));
    m_window->setWantsMultiClickEvents(false);
    m_window->subscribeEvent(PushButton::EventClicked,
			     Event::Subscriber(&SelectorWindow::Category::Button::onClick, this));
    m_window->subscribeEvent(Window::EventParentSized,
			     Event::Subscriber(&SelectorWindow::Category::Button::onParentResize, this));
}

void SelectorWindow::Category::Button::setPosition()
{
    float x, y;
    int xindex, yindex, butsinrow;
    float maxw = m_window->getParentPixelWidth();
	
    butsinrow = int(maxw) / int(BUT_WIDTH + BUT_XGAP);
    if (butsinrow < 1)
	butsinrow = 1;
		
    yindex = m_index / butsinrow;
    xindex = m_index % butsinrow;
	
    y = (yindex) * BUT_YGAP + yindex * BUT_HEIGHT;
    x = (xindex) * BUT_XGAP + xindex * BUT_WIDTH;

    m_window->setPosition(UVector2(UDim(0, x), UDim(0, y)));
}

SelectorWindow::Category::Category(const std::string& name, ElementManager* mgr) :
    m_mgr(mgr),
    m_nbut(0)
{
    WindowManager& wmgr = WindowManager::getSingleton();
	
    m_window = wmgr.createWindow("TaharezLook/ScrollablePane");
    m_window->setPosition( UVector2(UDim(0, BUT_XGAP),    UDim(0, BUT_YGAP)) );
    m_window->setSize    ( UVector2(UDim(1, -2*BUT_XGAP), UDim(1, -2*BUT_YGAP)) );
    m_window->setText    (name);
}

SelectorWindow::Category::~Category()
{
    list<Button*>::iterator i;
    for (i = m_buts.begin(); i != m_buts.end(); i++);
    delete *i;
	
    //delete m_window;
}

void SelectorWindow::Category::addButton(const std::string& name, int elem_id)
{
    Button* but = new Button(name, m_mgr, elem_id, m_nbut++);
    m_buts.push_back(but);

    m_window->addChildWindow( but->getWindow() );
	
    but->setPosition();
}

CEGUI::FrameWindow* SelectorWindow::createWindow()
{
    WindowManager& wmgr = WindowManager::getSingleton();
	
    FrameWindow* window = dynamic_cast<FrameWindow*>
	(wmgr.createWindow("TaharezLook/FrameWindow", "window_selector"));
	
    window->setPosition( UVector2(UDim(0, 10), UDim(0, 10)) );
    window->setSize    ( UVector2(UDim(1, -20),UDim(0, 100)) );
    window->setText("Object Selector");
	
    m_container = wmgr.createWindow("TaharezLook/TabControl", "container_selector");
    m_container->setPosition( UVector2(UDim(0, 10), UDim(0, 30)) );
    m_container->setSize    ( UVector2(UDim(1, -20),     UDim(1, -40)) );
	
    window->addChildWindow(m_container);
	
    return window;
}

SelectorWindow::Category* SelectorWindow::addCategory(const std::string& name)
{
    if (m_cat[name] != NULL)
	return NULL;

    Category* newcat = new Category(name, m_mgr);
    m_container->addChildWindow(newcat->getWindow());
    m_cat[name] = newcat;
	
    return newcat;
}

SelectorWindow::Category* SelectorWindow::findCategory(const std::string& name)
{
    map<string,Category*>::iterator i = m_cat.find(name);
	
    if (i == m_cat.end())
	return NULL;
	
    return (*i).second;
}

SelectorWindow::SelectorWindow(ElementManager* emgr) :
    m_mgr(emgr)
{
    setActive(false); // Force creation of the window;
}

SelectorWindow::~SelectorWindow()
{
    map<string,Category*>::iterator i;
    for (i = m_cat.begin(); i != m_cat.end(); i++)
	delete (*i).second;
}