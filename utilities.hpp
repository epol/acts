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
#include <cmath>

class Vec3D
{
public:
    double x;
    double y;
    double z;
    
    inline Vec3D(const double x, const double y, const double z) : x(x), y(y), z(z) {}
    inline Vec3D() : x(0), y(0), z(0) {}

    friend Vec3D operator* (const double lambda, const Vec3D vec);
    friend Vec3D operator+ (const Vec3D a, const Vec3D b);
    friend Vec3D operator- (const Vec3D a);
    friend Vec3D operator- (const Vec3D a, const Vec3D b);
    friend Vec3D operator^ (const Vec3D a, const Vec3D b);
    
    inline double norm() const
    {
        return sqrt(this->x*this->x + this->z*this->z + this->z*this->z);
    }
    
    friend std::ostream& operator<< (std::ostream& ostr, const Vec3D a);
};

inline double mod (const double n, const double p)
{
    return n - p*(floor(n/p));
}

class Launch
{
public:
    double theta;
    double phi;
    double speed;
    
    inline Launch(const double theta, const double phi, const double speed) : 
        theta(theta), phi(mod(phi,2*M_PI)), speed(speed) { }
    inline Launch() {}
    
    friend std::ostream& operator<< (std::ostream& ostr, const Launch l);
};


class Target
{
public:
    double x;
    double y;
    
    inline Target(const double x, const double y) : x(x), y(y) {}
    inline Target() {}
    
    double distance() const;
    double phi() const;

    friend Target operator* (const double lambda, const Target trg);
    friend Target operator+ (const Target a, const Target b);
    friend Target operator- (const Target a);
    friend Target operator- (const Target a, const Target b);
    
    friend std::ostream& operator<< (std::ostream& ostr, const Target t);
};

class Event
{
public:
    Launch launch;
    Target target;
    double time;
    
    inline Event(const Launch launch, const Target target, const double time=0) :
        launch(launch), target(target), time(time) {}
    inline Event() {}
    
    friend std::ostream& operator<< (std::ostream& ostr, const Event e);
};


#endif  // _UTILITIES_HPP
