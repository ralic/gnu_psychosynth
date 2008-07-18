/***************************************************************************
 *                                                                         *
 *   PSYCHOSYNTH                                                           *
 *   ===========                                                           *
 *                                                                         *
 *   Copyright (C) Juan Pedro Bolivar Puente 2008                          *
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

#ifndef DEFAULTSELECTORPOPULATOR_H
#define DEFAULTSELECTORPOPULATOR_H

#include <libpsynth/common/Config.h>
#include "gui3d/SelectorWindow.h"

class DefaultSelectorPopulator
{
    SelectorWindow* m_selector;

    void populateSamples(SelectorWindow::Category* cat);
    
    bool onSamplesConfNudge(psynth::ConfNode& conf);
public:
    void populate(SelectorWindow* sel);
};

#endif /* DEFAULTSELECTORPOPULATOR_H */