/* 
 * utilities.hpp
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

#ifndef _UTILITIES_HPP
#define _UTILITIES_HPP

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
    friend Vec3D operator- (const Vec3D a, const Vec3D b);
    friend Vec3D operator^ (const Vec3D a, const Vec3D b);
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
