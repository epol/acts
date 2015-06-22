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
#include "fitter.hpp"

class SimpleSimulator : public Simulator
{
protected:
    virtual Vec3D calculate_friction(Vec3D, Vec3D);
    using Simulator::Simulator;  // inherit constructors
    
public:
    inline SimpleSimulator(double dtime, double frictionC, double frictionA, double latitude):
        Simulator(dtime, frictionC, frictionA, latitude) {}
    
};


class Computer
{
private:
    double latitude;
    Fitter fitter;
    const double dtime = 1;
public:
    Computer(double latitude) : latitude(latitude), fitter(latitude) {}
    
    Launch calculate_launch_params(Target target, double speed);
    void add_event(Event event)
    {
        fitter.add_event(event);
    }
};











#endif // _COMPUTER_HPP
