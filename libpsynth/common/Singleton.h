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

#ifndef PSYNTH_SINGLETON_H
#define PSYNTH_SINGLETON_H

namespace psynth
{

/**
 * Singleton pattern implementation.
 *
 * A singleton class is one that can be instantiated only once and that has
 * global access to it. Try to use it only if both needs apply for your class
 * and not like a fancier global variable.
 *
 * Any class will be a singleton just by inheriting from this class. 
 */
template<typename T>
class Singleton
{
protected:
    /** Hidden constructor. */
    Singleton() {};

    /** Hidden copy constructor. */
    Singleton(Singleton const&) {};

    /** Hidden copy operator. */
    Singleton& operator=(Singleton const&) {};

    /** Hidden destructor. */
    ~Singleton() {};
    
  public:
    
    /**
     * Returns the instance of the Singleton.
     */
    static T& instance() {
        static T _instance;
        return _instance;
    };
};

} /* namespace psynth */

#endif /* PSYNTH_SINGLETON_H */
