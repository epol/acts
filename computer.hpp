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


#include <utility>
#include "utilities.hpp"
#include "simulator.hpp"

class SimpleSimulator : public Simulator
{
private:
    virtual Vec3D calculate_friction(const Vec3D);
    using Simulator::Simulator;  // inherit constructors
    
public:
    inline SimpleSimulator(const double dtime, const double friction1, const double friction2, const double latitude):
        Simulator(dtime, friction1, friction2, latitude) {}
    
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
    
    double friction1;
    double friction2;
    bool updatedFriction = true;
    
    void update_values();
    
public:
    
    Computer(const double latitude, const double friction1=0, const double friction2=0, const int memorysize=10, const double relativeDtime=1e-3) :
        simpleSim(0.02, friction1, friction2, latitude),
        memory(memorysize),
        relativeDtime(relativeDtime),
        friction1(friction1),
        friction2(friction2) {}
    
    Launch calculate_launch_params(const Target target, const double speed);
    
    inline void add_event(const Event event)
    {
        this->updatedFriction = false;
        this->memory.add(event);
    }
    
    inline std::pair<double, double> get_friction()
    {
        return std::pair<double, double>(this->friction1, this->friction2);
    }
    
    inline void set_simpleSim_friction(const double friction1, const double friction2)
    {
        this->simpleSim.set_friction(friction1, friction2);
    }
    Event simulate(Launch launch)
    {
        return this->simpleSim.simulate(launch);
    }
    
    inline void set_dtime(const double distance,const double speed)
    {
        this->simpleSim.set_dtime(distance*this->relativeDtime/speed);
    }
    
    EventMemory* get_memory()
    {
        return &(this->memory);
    }
};


class ComputerException
{
public:
    enum errcode {LOWPOWER};
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
        default:
            return "Unknown ComputerException error code";
            break;
        }
    }
};


#endif // _COMPUTER_HPP
