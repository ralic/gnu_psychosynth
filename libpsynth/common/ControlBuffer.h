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

#ifndef PSYNTH_CONTROLBUFFER_H
#define PSYNTH_CONTROLBUFFER_H

#include <cstring>

#include <libpsynth/common/AudioInfo.h>

namespace psynth
{

/**
 * A buffer for control data. It holds only one channel of scalar samples
 * and contains no information about sampling rate or other audio signal related
 * information, in contrast with @c AudioBuffer.
 */
class ControlBuffer
{
    Sample* m_data;
    size_t m_size;

    void allocate() {
	m_data = new Sample[m_size];
        /* FIXME: what is the effect of new Sample[0] ?*/
    }
    
    void liberate() {
	delete [] m_data;
	m_data = NULL;
    }
    
public:
    /** Constructor. */
    ControlBuffer() :
	m_data(NULL),
	m_size(0)
	{}

    /** Destructor. */
    ControlBuffer(size_t size) :
	m_size(size) {
	allocate();
    }

    /** Copy constructor. */
    ControlBuffer(const ControlBuffer& buf) :
	m_size(buf.m_size) {
	allocate();
	memcpy(m_data, buf.m_data, sizeof(Sample) * m_size);
    }

    /** Destructor. */
    ~ControlBuffer() {
	liberate();
    }

    /** Assignment operator. */
    ControlBuffer& operator=(const ControlBuffer& buf) {
	if (&buf != this) {
	    liberate();
	    m_size = buf.m_size;
	    allocate();
	    memcpy(m_data, buf.m_data, sizeof(Sample) * m_size);
	}
	return *this;
    }

    /**
     * Returns a reference to a sample contained in the buffer.
     * @param i Index to the sample we want.
     */
    Sample& operator[] (size_t i) {
	return m_data[i];
    }

    /**
     * Returns a constant reference to a sample contained in the buffer.
     * @param i Index to the sample we want.
     */
    const Sample& operator[] (size_t i) const {
	return m_data[i];
    }

    /**
     * Returns a pointer to the raw data of the buffer.
     */
    Sample* getData() {
	return m_data;
    }

    /**
     * Returns a constant pointer to the raw data of the buffer.
     */
    const Sample* getData() const {
	return m_data;
    }

    /**
     * Free all the data in the buffer setting its size to 0.
     */
    void clear() {
	liberate();
	m_size = 0;
    }

    /**
     * Returns the current size of the buffer.
     */
    size_t size() const {
	return m_size;
    }

    /**
     * Gives the buffer a new size.
     * @param newsize The new size of the buffer.
     */
    void resize(size_t newsize) {
	liberate();
	m_size = newsize;
	allocate();
    }

    /**
     * Fills the whole buffer with value zero.
     */
    void zero() {
	memset(m_data, 0, sizeof(Sample) * m_size);
    }
};

} /* namespace psynth */

#endif /* PSYNTH_CONTROLBUFER_H */
