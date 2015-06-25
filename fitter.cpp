/* 
 * fitter.cpp
 * 
 * This file is part of acts
 * 
 * Copyright (C) 2015 Enrico Polesel <enrico.polesel@sns.it>,
 * Copyright (C) 2015 Andrea Stacchiotti <andrea.stacchiotti@sns.it>
 * 
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA.
 * 
 */

#include "fitter.hpp"

void Fitter::update_values()
{
    // TODO
    this->validFriction = true;
}


double Fitter::get_frictionA()
{
    if (! (this->validFriction))
    {
        this->update_values();
    }
    return this->frictionA;
}

double Fitter::get_frictionC()
{
    if (! (this->validFriction))
    {
        this->update_values();
    }
    return this->frictionC;
}

void EventMemory::add(Event e)
{
    if (this->used < this->maxsize)
    {
        // The array is not full, yet
        ++(this->used);
    }
    this->first = (this->first == 0) ? (this->maxsize - 1 ) : (this->first -1);
    this->memory[this->first] = e;
}

Event * EventMemory::get()
{
    if (this->used < this->maxsize)
    {
        return this->memory + this->first;
    }
    
    if (this->first == 0)
    {
        return this->memory;
    }
    else
    {
        // sorting the array to move first in position 0
        Event * newmemory = new Event[this->maxsize];
        for (int i = 0; i<this->maxsize ; ++i)
        {
            newmemory[i] = this->memory[(this->first + i)%this->maxsize];
        }
        delete[] this->memory;
        this->memory = newmemory;
        this->first = 0;
        return this->memory;
    }
}
