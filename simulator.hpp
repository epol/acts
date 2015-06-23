/* 
 * simulator.hpp
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

#ifndef _SIMULATOR_HPP
#define _SIMULATOR_HPP

#include "utilities.hpp"
#include <cmath>

class Simulator
{
private:
    double dtime;

protected:
    double frictionC;
    double frictionA;
    
    double latitude=90;
    const double gravity=9.8;
    const double modOmega=1./24/60/60;
    Vec3D omega=Vec3D(0,0,modOmega);
    
    inline Simulator(double dtime, double frictionC, double frictionA): dtime(dtime), frictionC(frictionC), frictionA(frictionA) {}
    
    inline Simulator(double dtime, double frictionC, double frictionA, double latitude) : Simulator(dtime, frictionC, frictionA)
    {
        this->set_latitude(latitude);
    }
    
    virtual Vec3D calculate_friction(Vec3D, Vec3D)
    // Calculate the acceleration due to friction at a given position and velocity
    {
        return Vec3D(0, 0, 0);
    }

    virtual Vec3D calculate_nonfriction(Vec3D position, Vec3D velocity);
    // Calculate all the non-friction accelerations (gravity & non-inertial)

public:
    inline void set_latitude(double latitude)
    // Wants latitude in degrees, stores it in radians
    {
        this->latitude = latitude * M_PI/180;
        this->omega = modOmega * Vec3D(0, cos(this->latitude), sin(this->latitude));
    }
    
    inline void set_friction(double frictionC, double frictionA)
    {
        this->frictionC = frictionC;
        this->frictionA = frictionA;
    }
    
    inline double get_gravity()
    {
        return this->gravity;
    }
    
    Event simulate(Launch launch);
};


#endif // _SIMULATOR_HPP
