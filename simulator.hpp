/* 
 * simulator.hpp
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
    
    virtual Vec3D calculate_friction(double, Vec3D)
    // Calculate the acceleration due to friction at a given altitude and velocity
    {
        return Vec3D(0, 0, 0);
    }

    virtual Vec3D calculate_nonfriction(Vec3D position, Vec3D velocity);
    // Calculate all the non-friction accelerations (gravity & non-inertial)

public:
    inline Simulator(double dtime, double frictionC, double frictionA): dtime(dtime), frictionC(frictionC), frictionA(frictionA) {}
    inline Simulator(double dtime, double frictionC, double frictionA, double latitude)
    {
        Simulator(dtime, frictionC, frictionA);
        this->set_latitude(latitude);
    }
    
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
    
    Event simulate(Launch launch);
};


#endif // _SIMULATOR_HPP
