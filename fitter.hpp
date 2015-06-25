/* 
 * fitter.hpp
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
 
#ifndef _FITTER_HPP
#define _FITTER_HPP

#include "utilities.hpp"
#include "simulator.hpp"

class EventMemory
{
private:
    int maxsize;
    Event * memory;
    int used = 0;
    int first = 0;
public:
    EventMemory(int maxsize) : maxsize(maxsize), first(maxsize)
    {
        this->memory = new Event[this->maxsize];
    }
    ~EventMemory()
    {
        delete[] this->memory;
    }
    
    void add(Event e);
    Event * get();
    inline int size()
    {
        return this->used;
    }
};

class Fitter
{
private:
    double frictionC=0; //TODO
    double frictionA=0; //TODO
    bool validFriction = true;
    
    double latitude;
    EventMemory memory; // TODO: set a sound memory size

    void update_values();
    
public:
    Fitter(double latitude): latitude(latitude), memory(10) {}
    inline void add_event(Event event)
    {
        this->validFriction = false;
        this->memory.add(event);
    }
    
    double get_frictionA();
    double get_frictionC();
};



#endif // _FITTER_HPP
