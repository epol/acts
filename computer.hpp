/* 
 * computer.hpp
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
 
#ifndef _COMPUTER_HPP
#define _COMPUTER_HPP


#include "utilities.hpp"
#include "simulator.hpp"
#include "fitter.hpp"

class SimpleSimulator : public Simulator
{
public:
    using Simulator::Simulator;  // inherit constructors
    
protected:
    virtual Vec3D calculate_friction(Vec3D, Vec3D);
    
};


class Computer
{
private:
    double latitude;
    Fitter fitter;
public:
    Computer(double latitude) : latitude(latitude), fitter(latitude) {}
    
    Launch calculate_launch_params(Target target, double speed);
    void add_event(Event event)
    {
        fitter.add_event(event);
    }
};











#endif // _COMPUTER_HPP
