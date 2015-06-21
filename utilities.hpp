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


class Vec3D
{
public:
    double x;
    double y;
    double z;
    
    inline Vec3D(double x, double y, double z) : x(x), y(y), z(z) {}
    // TODO: operators
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


class Position
{
public:
    double x;
    double y;
    
    inline Position(double x, double y) : x(x), y(y) {}
};

class Event
{
public:
    Launch launch;
    Position position;
    double time;
    
    inline Event(Launch launch, Position position, double time=0) :
        launch(launch), position(position), time(time) {}
};
