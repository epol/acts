/* 
 * simulator.cpp
 * 
 * This file is part of acts
 * 
 * Copyright (C) 2015 Enrico Polesel, Andrea Stacchiotti
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <cmath>
#include "simulator.hpp"
#include "utilities.hpp"

Vec3D Simulator::calculate_nonfriction(Vec3D position, Vec3D velocity)
// Calculate all the non-friction accelerations (gravity & non-inertial)
{
    Vec3D vecGravity = Vec3D(0, 0, -gravity);
    Vec3D vecCoriolis = -2 * omega ^ velocity;
    Vec3D vecCentrifugal = - omega ^ (omega ^ position);
    return vecGravity + vecCoriolis + vecCentrifugal;
}

Event Simulator::simulate (Launch launch)
{
    Vec3D position(0,0,0);
    Vec3D velocity = launch.speed * Vec3D(
            cos(launch.theta)*cos(launch.phi),
            cos(launch.theta)*sin(launch.phi),
            sin(launch.theta)
            );
    double t = 0;
    do 
    {
        Vec3D acceleration = this->calculate_friction(position,velocity) + this->calculate_nonfriction(position,velocity);
        position = position + .5 * (this->dtime*this->dtime) * acceleration + this->dtime * velocity;
        velocity = velocity + this->dtime * acceleration;
        t += this->dtime;
    }
    while (position.z > 0);
    
    return Event(launch,Target(position.x,position.y),t);
}