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

#include <iostream>

#include "psychosynth.h"

#include "table/Table.h"
#include "table/PatcherDynamic.h"
#include "common/ArgParser.h"
#include "gui3d/PsychoSynth3D.h"
#include "output/OutputAlsa.h"
#include "output/OutputOss.h"
#include "object/ObjectOscillator.h"
#include "object/ObjectMixer.h"
#include "object/ObjectOutput.h"
#include "object/ObjectManager.h"

using namespace std;

int main(int argc, const char *argv[])
{
    PsychoSynth3D main_app;

    main_app.run(argc, argv);
	
    return 0;
}