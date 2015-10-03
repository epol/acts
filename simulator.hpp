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
protected:
    double dtime;
    double friction1;
    double friction2;
    
    double latitude=90;
    const double gravity=9.8;
    const double modOmega=1./24/60/60;
    Vec3D omega=Vec3D(0,0,modOmega);
    double target_elevation = 0;
    
    inline Simulator(const double dtime, const double friction1, const double friction2): dtime(dtime), friction1(friction1), friction2(friction2) {}
    
    inline Simulator(const double dtime, const double friction1, const double friction2, const double latitude) : Simulator(dtime, friction1, friction2)
    {
        this->set_latitude(latitude);
    }
    
    virtual Vec3D calculate_friction(const Vec3D)
    // Calculate the acceleration due to friction at a given position and velocity
    {
        return Vec3D(0, 0, 0);
    }

    virtual Vec3D calculate_nonfriction(const Vec3D position, const Vec3D velocity) const;
    // Calculate all the non-friction accelerations (gravity & non-inertial)

public:
    inline void set_latitude(const double latitude)
    // Wants latitude in degrees, stores it in radians
    {
        this->latitude = latitude * M_PI/180;
        this->omega = this->modOmega * Vec3D(0, cos(this->latitude), sin(this->latitude));
    }
    
    inline void set_friction(const double friction1, const double friction2)
    {
        this->friction1 = friction1;
        this->friction2 = friction2;
    }
    
    inline double get_gravity() const
    {
        return this->gravity;
    }
    
    Event simulate(const Launch launch);
};


#endif // _SIMULATOR_HPP
