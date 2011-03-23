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

#include <iostream>
#include <synth/oscillator.hpp>
#include <synth/envelope_simple.hpp>

using namespace std;

namespace psynth
{

bool oscillator::m_table_init = false;
wave_table oscillator::TABLE [oscillator::WAVE_TYPES];


void oscillator::update(sample* out_buf, size_t n_frames)
{
    float speed = m_freq / m_info.sample_rate;
    for (size_t i = 0; i < n_frames; ++i) {
	*out_buf++ = compute_sample(m_x) * m_ampl;
	m_x += speed;
    }

    m_x = phase(m_x);
}

void oscillator::update_fm (sample* out_buf, const sample* mod_buf,
			    envelope_simple& mod_env, size_t n_frames)
{
    for (size_t i = 0; i < n_frames; ++i) {
	*out_buf++ = compute_sample(m_x) * m_ampl;
	m_x += (m_freq + m_freq * *mod_buf++ * mod_env.update()) / m_info.sample_rate;
    }

    m_x = phase(m_x);
}

void oscillator::update_pm (sample* out_buf, const sample* mod_buf,
			    envelope_simple& mod_env, size_t n_frames)
{
    float speed = m_freq / m_info.sample_rate;
    for (size_t i = 0; i < n_frames; ++i) {
	*out_buf++ = compute_sample(m_x + *mod_buf * mod_env.update()) * m_ampl;
	m_x += speed;
    }

    m_x = phase(m_x);
}

void oscillator::update_am (sample* out_buf, const sample* mod_buf,
			    envelope_simple& mod_env, size_t n_frames)
{
    float speed = m_freq / m_info.sample_rate;
    for (size_t i = 0; i < n_frames; ++i) {
	*out_buf++ = compute_sample(m_x) * (m_ampl + m_ampl * *mod_buf++ * mod_env.update());
	m_x += speed;
    }

    m_x = phase(m_x);
}

} /* namespace psynth */
