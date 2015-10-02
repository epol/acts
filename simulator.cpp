/* 
 * simulator.cpp
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

#include <cmath>
#include "simulator.hpp"
#include "utilities.hpp"

Vec3D Simulator::calculate_nonfriction(const Vec3D position, const Vec3D velocity) const
// Calculate all the non-friction accelerations (gravity & non-inertial)
{
    Vec3D vecGravity = Vec3D(0, 0, -gravity);
    Vec3D vecCoriolis = -2 * this->omega ^ velocity;
    Vec3D vecCentrifugal = - this->omega ^ (this->omega ^ position);
    return vecGravity + vecCoriolis + vecCentrifugal;
}

Event Simulator::simulate (const Launch launch) const
{
    Vec3D position(0,0,0);
    Vec3D velocity = launch.speed * Vec3D(
            cos(launch.theta)*cos(launch.phi),
            cos(launch.theta)*sin(launch.phi),
            sin(launch.theta)
            );
    double t = 0;
    Vec3D prev_position(0, 0, 0);
    do
    {
        prev_position = position;
        Vec3D acceleration = this->calculate_friction(position,velocity) + this->calculate_nonfriction(position,velocity);
        position = position + this->dtime * velocity + .5 * (this->dtime*this->dtime) * acceleration;
        velocity = velocity + this->dtime * acceleration;
        t += this->dtime;
    }
    while (position.z > 0);
    
    Target trg = Target(0.5 * (position.x + prev_position.x), 0.5 * (position.y + prev_position.y));
    
    return Event(launch, trg, t - 0.5*dtime);
}
