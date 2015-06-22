/* 
 * utilities.hpp
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

#ifndef _UTILITIES_HPP
#define _UTILITIES_HPP

#include <iostream>

class Vec3D
{
public:
    double x;
    double y;
    double z;
    
    inline Vec3D(double x, double y, double z) : x(x), y(y), z(z) {}
    inline Vec3D() : x(0), y(0), z(0) {}
    // TODO: operators
    friend Vec3D operator* (const double lambda, const Vec3D vec);
    friend Vec3D operator+ (const Vec3D a, const Vec3D b);
    friend Vec3D operator- (const Vec3D a);
    friend Vec3D operator- (const Vec3D a, const Vec3D b);
    friend Vec3D operator^ (const Vec3D a, const Vec3D b);
    friend std::ostream& operator<< (std::ostream& ostr, const Vec3D a);
};

class Launch
{
public:
    double theta;
    double phi;
    double speed;
    
    inline Launch(double theta, double phi, double speed) : 
        theta(theta), phi(phi), speed(speed) {}
};


class Target
{
public:
    double x;
    double y;
    
    inline Target(double x, double y) : x(x), y(y) {}
};

class Event
{
public:
    Launch launch;
    Target target;
    double time;
    
    inline Event(Launch launch, Target target, double time=0) :
        launch(launch), target(target), time(time) {}
};


#endif  // _UTILITIES_HPP
