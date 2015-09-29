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

#include <gsl/gsl_vector.h>
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

double rolling_chi2 (const gsl_vector *v, void *params);

class Minimizer
{
private:
    static const int nvars = 2;
    static const int npars = 2;
    double startingPoint[npars] = {0};
    
    double accuracy = 1e-4;
    double step = 1;
    double maxiter = 100;
    
    int status = -1;

public:
    double finalPoint[npars];
    double minValue;

    Minimizer() {};
    
    void set_starting_point(double x0, double y0)
    {
        startingPoint[0] = x0;
        startingPoint[1] = y0;
    }

    int minimize(EventMemory* mem);
};


class Fitter
{
private:
    double frictionC = 0;
    double frictionA = 0;
    bool updatedFriction = true;
    
    double latitude;
    EventMemory memory;
    Minimizer minimizer;
    
public:
    Fitter(double latitude): latitude(latitude), memory(10) {}
    Fitter(double frictionC, double frictionA, double latitude) : Fitter(latitude)
    {
        this->frictionC = frictionC;
        this->frictionA = frictionA;
    }
    
    inline void add_event(Event event)
    {
        this->updatedFriction = false;
        this->memory.add(event);
    }
    
    void update_values();
    
    double get_frictionA();
    double get_frictionC();
};



#endif // _FITTER_HPP
