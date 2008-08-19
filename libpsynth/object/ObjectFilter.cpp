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

#include <algorithm>
#include "object/ObjectFilter.h"
#include "object/KnownObjects.h"

using namespace std;

namespace psynth
{

PSYNTH_DEFINE_OBJECT_FACTORY(ObjectFilter);

ObjectFilter::ObjectFilter(const audio_info& prop, int mode) : 
    Object(prop,
	   OBJ_FILTER,
	   "filter",
	   N_IN_A_SOCKETS,
	   N_IN_C_SOCKETS,
	   N_OUT_A_SOCKETS,
	   N_OUT_C_SOCKETS),
    m_param_type(mode),
    m_param_cutoff(DEFAULT_CUTOFF),
    m_param_resonance(DEFAULT_RESONANCE),
    m_filter_values((FilterValues::Type)m_param_type,
		    m_param_cutoff,
		    m_param_resonance,
		    prop.sample_rate),
    m_filter(prop.num_channels, Filter(&m_filter_values))
{
    addParam("type", ObjParam::INT, &m_param_type);
    addParam("cutoff", ObjParam::FLOAT, &m_param_cutoff);
    addParam("resonance", ObjParam::FLOAT, &m_param_resonance);

    //for (int i = 0; i < prop.num_channels; ++i)
}

/* TODO cambiar el numero de canales. */

ObjectFilter::~ObjectFilter()
{
}

void ObjectFilter::doUpdate(const Object* caller, int caller_port_type, int caller_por)
{
    const audio_buffer* input = getInput<audio_buffer>(LINK_AUDIO, IN_A_INPUT);
    const sample_buffer* cutoff = getInput<sample_buffer>(LINK_CONTROL, IN_C_CUTOFF);
    audio_buffer* output = getOutput<audio_buffer>(LINK_AUDIO, IN_A_INPUT);    

    if (input) {
	if (m_param_type != m_filter_values.getType() ||
	    m_param_cutoff != m_filter_values.getFrequency() ||
	    m_param_resonance != m_filter_values.getResonance())
	    m_filter_values.calculate((FilterValues::Type)m_param_type,
				      m_param_cutoff,
				      m_param_resonance,
				      getaudio_info().sample_rate);
	
	for (int i = 0; i < getaudio_info().num_channels; ++i) {
	    sample* outbuf = output->get_channel(i);
	    const sample* inbuf = input->get_channel(i);
	    Filter& filter = m_filter[i];
	    EnvelopeSimple env = getInEnvelope(LINK_AUDIO, IN_A_INPUT);
	    if (!cutoff)
		for (size_t j = 0; j < output->size(); ++j)
		    *outbuf++ = filter.update(*inbuf++ * env.update());
	    else {
		EnvelopeSimple mod_env = getInEnvelope(LINK_CONTROL,
						       IN_C_CUTOFF);
		const sample* cutoff_buf = cutoff->get_data();
		
		for (size_t j = 0; j < output->size(); ++j) {
		    /* FIXME: Slow */
		    m_filter_values.calculate(m_param_cutoff
					      + *cutoff_buf++
					      * m_param_cutoff
					      * mod_env.update());
		    *outbuf++ = filter.update(*inbuf++ * env.update());
		}
	    }
	}
    } else {
	output->zero();
    }
}

} /* namespace psynth */
