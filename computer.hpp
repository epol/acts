/* 
 * computer.hpp
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
 
#ifndef _COMPUTER_HPP
#define _COMPUTER_HPP


#include "utilities.hpp"
#include "simulator.hpp"

class SimpleSimulator : public Simulator
{
private:
    virtual Vec3D calculate_friction(const Vec3D, const Vec3D);
    using Simulator::Simulator;  // inherit constructors
    
public:
    inline SimpleSimulator(const double dtime, const double frictionC, const double frictionA, const double latitude):
        Simulator(dtime, frictionC, frictionA, latitude) {}
    
    inline void set_dtime(const double dtime)
    {
        this->dtime = dtime;
    }
};

class EventMemory
{
private:
    int maxsize;
    Event * memory;
    int used = 0;
    int first = 0;

public:
    EventMemory(const int maxsize) : maxsize(maxsize), first(maxsize)
    {
        this->memory = new Event[this->maxsize];
    }
    ~EventMemory()
    {
        delete[] this->memory;
    }
    
    void add(const Event e);
    Event * get();
    inline int size() const
    {
        return this->used;
    }
};

class Computer;

class Minimizer
{
private:
    static const int nvars = 2;
    static const int npars = 2;
    double startingPoint[npars] = {0};
    
    double accuracy = 1e-5;
    double step = 0.001;
    double maxiter = 1000;
    
    int status = -1;

public:
    double finalPoint[npars];
    double minValue;

    Minimizer() {};
    
    void set_starting_point(double x0, double y0)
    {
        this->startingPoint[0] = x0;
        this->startingPoint[1] = y0;
    }

    int minimize(Computer* comp);
};

class Computer
{
private:
    SimpleSimulator simpleSim;
    EventMemory memory;
    Minimizer minimizer;
    
    double relativeDtime;
    
    double frictionC;
    double frictionA;
    bool updatedFriction = true;
    
    void update_values();
    
public:
    
    Computer(const double latitude, const double frictionC=0, const double frictionA=0, const int memorysize=10, const double relativeDtime=1e-3) :
        simpleSim(0.02, frictionC, frictionA, latitude),
        memory(memorysize),
        relativeDtime(relativeDtime),
        frictionC(frictionC),
        frictionA(frictionA) {}
    
    Launch calculate_launch_params(const Target target, const double speed);
    
    inline void add_event(const Event event)
    {
        this->updatedFriction = false;
        this->memory.add(event);
    }
    
    inline void set_simpleSim_friction(const double frictionC, const double frictionA)
    {
        this->simpleSim.set_friction(frictionC, frictionA);
    }
    Event simulate(Launch launch)
    {
        return this->simpleSim.simulate(launch);
    }
    
    EventMemory* get_memory()
    {
        return &(this->memory);
    }
};


class ComputerException
{
public:
    enum errcode {LOWPOWER,SOLVERINIT,NEGATIVETHETA};
private:
    enum errcode type;
public:
    ComputerException(const enum errcode e): type(e) {};
    const char *show_reason() const
    {
        switch (this->type)
        {
        case LOWPOWER:
            return "Launch power is not sufficient";
            break;
        case SOLVERINIT:
            return "Error initializing the solver";
            break;
        case NEGATIVETHETA:
            return "Negative theta";
            break;
        default:
            return "Unknown ComputerException error code";
            break;
        }
    }
};


#endif // _COMPUTER_HPP
