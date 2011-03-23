/**
 *  Time-stamp:  <2011-03-09 17:18:52 raskolnikov>
 *
 *  @file        logger.hpp
 *  @author      Juan Pedro Bolívar Puente <raskolnikov@es.gnu.org>
 *  @date        2007
 *
 *  A hierarchical, multi-backend and thread-safe logging facility.
 *  Implementation file.
 */

/*
 *  Copyright (C) 2007, 2008, 2009 Juan Pedro Bolívar Puente
 *
 *  This file is part of Psychosynth.
 *   
 *  Psychosynth is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Psychosynth is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "base/logger.hpp"

using namespace std;

namespace psynth
{
namespace base
{

log::~log()
{
}

void log::add_sink (log_sink_ptr d)    
{
    lock lock (this);
    _dumpers.push_back (d);
}

void log::del_sink (log_sink_ptr d)    
{
    lock lock (this);

    _dumpers.remove (d);
#if 0
    _dumpers.remove_if ([&] (log_sink_ptr p) {
            return equal_ptr (d, p);
        });
#endif
}

void log::operator () (log& l, int level, const string& msg)
{
    for (auto it = _dumpers.begin(); it != _dumpers.end(); ++it)
    {
	(*it)->dump (l, level, msg);
    }

    if (parent ())
	parent ()->operator () (l, level, msg);
}

} /* namespace base */
} /* namespace psynth */

