/***************************************************************************
 *                                                                         *
 *   PSYCHOSYNTH                                                           *
 *   ===========                                                           *
 *                                                                         *
 *   Copyright (C) Juan Pedro Bolivar Puente 2007                          *
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

#ifndef PSYNTH_OBJECTFILTER_H
#define PSYNTH_OBJECTFILTER_H

#include <vector>

#include <libpsynth/object/Object.h>
#include <libpsynth/object/Filter.h>
#include <libpsynth/object/ObjectFactory.h>

namespace psynth
{

class ObjectFilter : public Object
{
public:	
    enum InAudioSocketID {
	IN_A_INPUT,
	N_IN_A_SOCKETS
    };
	
    enum InControlSocketID {
	IN_C_CUTOFF,
	IN_C_EMPHASIS,
	N_IN_C_SOCKETS
    };
	
    enum OutAudioSocketID {
	OUT_A_OUTPUT,
	N_OUT_A_SOCKETS
    };
	
    enum OutControlSocketID {
	N_OUT_C_SOCKETS
    };

    enum FilterType {
	FILTER_LOWPASS       = FilterValues::LOWPASS,
	FILTER_HIGHPASS      = FilterValues::HIPASS,
	FILTER_BANDPASS_CSG  = FilterValues::BANDPASS_CSG,
	FILTER_BANDPASS_CZPG = FilterValues::BANDPASS_CZPG,
	FILTER_NOTCH         = FilterValues::NOTCH,
	FILTER_MOOG          = FilterValues::MOOG,
	N_FILTER_TYPES,
    };

    enum ParamID {
	PARAM_TYPE = Object::N_COMMON_PARAMS,
	PARAM_CUTOFF,
	PARAM_RESONANCE,
	N_PARAM
    };

    static const float DEFAULT_CUTOFF    = 220.0f;
    static const float DEFAULT_RESONANCE = 0.1f;
    
private:
    int m_param_type;
    float m_param_cutoff;
    float m_param_resonance;

    FilterValues m_filter_values;
    std::vector<Filter> m_filter;
    
    void doUpdate(const Object* caller, int caller_port_type, int caller_port);
    void doAdvance() {}
    void onInfoChange() {}
    
public:
    ObjectFilter(const AudioInfo& prop, int mode = FILTER_LOWPASS);
    ~ObjectFilter();
};

PSYNTH_DECLARE_OBJECT_FACTORY(ObjectFilter, "filter");

} /* namespace psynth */

#endif /* PSYNTH_OBJECTFILTER_H */
