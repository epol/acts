/* 
 * utilities.cpp
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

#include <iostream>
#include <cmath>
#include "utilities.hpp"

Vec3D operator* (const double lambda, const Vec3D vec)
{
    return Vec3D(lambda* vec.x, lambda* vec.y, lambda* vec.z);
}

Vec3D operator+ (const Vec3D a, const Vec3D b)
{
    return Vec3D(a.x+b.x, a.y+b.y, a.z+b.z);
}

Vec3D operator- (const Vec3D a)
{
    return Vec3D(-a.x, -a.y, -a.z);
}

Vec3D operator- (const Vec3D a, const Vec3D b)
{
    return a + ( -b );
}

Vec3D operator^ (const Vec3D a, const Vec3D b)
// 3D vector product in cartesian coordinates
{
    return Vec3D(
                    a.y*b.z - a.z*b.y,
                    a.z*b.x - a.x*b.z,
                    a.x*b.y - a.y*b.x
                );
}

std::ostream& operator<< (std::ostream& ostr, const Vec3D a)
{
    return ostr << "(" << a.x << ", " << a.y << ", " << a.z << ")" << std::endl;
}

// -----------------

std::ostream& operator<< (std::ostream& ostr, const Launch l)
{
    return ostr << "Launch(theta=" << l.theta << ", phi=" << l.phi << ", speed=" << l.speed << ")";
}

// -----------------

double Target::distance()
{
    return hypot(this->x, this->y);
}

double Target::phi()
{
    return atan2(this->y, this->x);
}

Target operator* (const double lambda, const Target trg)
{
    return Target(lambda* trg.x, lambda* trg.y);
}

Target operator+ (const Target a, const Target b)
{
    return Target(a.x+b.x, a.y+b.y);
}

Target operator- (const Target a)
{
    return Target(-a.x, -a.y);
}

Target operator- (const Target a, const Target b)
{
    return a + ( -b );
}

std::ostream& operator<< (std::ostream& ostr, const Target t)
{
    return ostr << "Target(" << t.x << ", " << t.y << ")";
}

// -----------------

std::ostream& operator<< (std::ostream& ostr, const Event e)
{
    return ostr << "Event(" << e.launch << ", " << e.target << ", time=" << e.time << ")";
}
