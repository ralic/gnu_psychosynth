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

#include <iostream>

#include "object/KnownObjects.h"
#include "object/ObjectOscillator.h"

using namespace std;

ObjectOscillator::ObjectOscillator(const AudioInfo& prop, int mode) : 
    Object(prop,
	   OBJ_OSCILLATOR,
	   N_PARAM,
	   N_IN_A_SOCKETS,
	   N_IN_C_SOCKETS,
	   N_OUT_A_SOCKETS,
	   N_OUT_C_SOCKETS),
    m_table(WaveTable::instance()),
    m_time(0),
    m_param_mode(mode),
    m_param_freq(DEFAULT_FREQ),
    m_param_ampl(DEFAULT_AMPL),
    m_old_freq(DEFAULT_FREQ)
{    
    configureParam(PARAM_WAVE, PARAM_INT, &m_param_mode);
    configureParam(PARAM_FREQUENCY, PARAM_FLOAT, &m_param_freq);
    configureParam(PARAM_AMPLITUDE, PARAM_FLOAT, &m_param_ampl);
}

ObjectOscillator::~ObjectOscillator()
{
}

void ObjectOscillator::doUpdate()
{
    AudioBuffer*         buf = getOutput<AudioBuffer>(LINK_AUDIO, OUT_A_OUTPUT);
    const ControlBuffer* pitch_buf = getInput<ControlBuffer>(LINK_CONTROL, IN_C_FREQUENCY);
    
    Sample*       chan = buf->getChannel(0);
    const Sample* pitch = NULL;

    float  rate = getAudioInfo().sample_rate;
    size_t size = getAudioInfo().block_size;
    
    float  speed = WaveTable::TABLE_SIZE * m_param_freq  / rate;
    size_t i;

    if (pitch_buf)
	pitch = pitch_buf->getData();
    
    for (i = 0; i < size; ++i, ++chan) {
	while ( m_time < 0.0 )
	    m_time += WaveTable::TABLE_SIZE;
	while ( m_time >= WaveTable::TABLE_SIZE )
	    m_time -= WaveTable::TABLE_SIZE;
	
	size_t index = m_time;
	size_t alpha = m_time - index;
	
	*chan = m_table[m_param_mode][index];
	*chan += alpha * (m_table[m_param_mode][++index] - *chan);
	*chan *= m_param_ampl;

	if (pitch) {
	    speed = WaveTable::TABLE_SIZE * (m_param_freq + m_param_freq * *pitch++) / rate;
	}
	
	m_time += speed;
    }

    for (i = 1; i < (size_t)getAudioInfo().num_channels; i++)
	memcpy((*buf)[i], (*buf)[0], sizeof(Sample) * getAudioInfo().block_size);
}
