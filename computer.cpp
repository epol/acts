/* 
 * computer.cpp
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
#include <cstdlib>
#include <gsl/gsl_multiroots.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_multimin.h>
#include <iostream> // FIXME: we are not using it for debug

#include "computer.hpp"
#include "utilities.hpp"
#include "simulator.hpp"

using namespace std;

Vec3D SimpleSimulator::calculate_friction(Vec3D position, Vec3D velocity)
// Calculate the acceleration due to friction at a given altitude and velocity
{
    double friction_coeff = frictionC * exp(-frictionA * position.z);
    return -friction_coeff * velocity;
}

// -----------------

void EventMemory::add(Event e)
{
    if (this->used < this->maxsize)
    {
        // The array is not full, yet
        ++(this->used);
    }
    this->first = (this->first == 0) ? (this->maxsize - 1 ) : (this->first -1);
    this->memory[this->first] = e;
}

Event * EventMemory::get()
// returns pointer to first element of a correctly ordered array
{
    if (this->used < this->maxsize)
    {
        return this->memory + this->first;
    }
    
    if (this->first == 0)
    {
        return this->memory;
    }
    else
    {
        // sorting the array to move first in position 0
        Event * newmemory = new Event[this->maxsize];
        for (int i = 0; i<this->maxsize ; ++i)
        {
            newmemory[i] = this->memory[(this->first + i)%this->maxsize];
        }
        delete[] this->memory;
        this->memory = newmemory;
        this->first = 0;
        return this->memory;
    }
}

// -----------------

double chi2 (const gsl_vector *v, void *params)
// this signature is required by gsl
{
    double fC = gsl_vector_get(v, 0);
    double fA = gsl_vector_get(v, 1);
    
    EventMemory* m = (EventMemory*)params;
    
    SimpleSimulator s(0.05, fC, fA, 45); // very inefficient, we'll use the one in the computer instance
    
    double chi2_accumulator = 0;
    
    for (int i=0; i< m->size(); ++i)
    {
        Event ev_real = m->get()[i];
        Event ev_sim = s.simulate(ev_real.launch);
        
        Target delta = ev_real.target - ev_sim.target;
        double chi2_piece = delta.distance() * delta.distance();
        
        chi2_accumulator += chi2_piece / pow(2, i);
    }
    return chi2_accumulator;
}

int Minimizer::minimize(EventMemory* mem)
{
    const gsl_multimin_fminimizer_type* T = gsl_multimin_fminimizer_nmsimplex2;
    gsl_vector *ss, *x;
    gsl_multimin_function minex_func;

    int iter = 0;
    double size;
    status=0;

    /* Starting point */
    x = gsl_vector_alloc(npars);
    for (int i=0; i<npars; ++i) {
        gsl_vector_set (x, i, startingPoint[i]);
    }

    /* Set initial step sizes to initial step */
    ss = gsl_vector_alloc (npars);
    gsl_vector_set_all (ss, step);

    /* Initialize method and iterate */
    minex_func.n = npars;
    minex_func.f = chi2;  // here we use our function
    minex_func.params = (void*)mem;  // pass the event memory as parameter

    gsl_multimin_fminimizer* s = gsl_multimin_fminimizer_alloc (T, nvars);
    gsl_multimin_fminimizer_set(s, &minex_func, x, ss);

    do {
        iter++;
        status = gsl_multimin_fminimizer_iterate(s);
        
        if (status) break;

        size = gsl_multimin_fminimizer_size (s);
        status = gsl_multimin_test_size (size, accuracy);
    }
    while (status == GSL_CONTINUE && iter < maxiter);
    
    for (int i=0; i<npars; ++i) {
        finalPoint[i] = gsl_vector_get(s->x, i);
    }
    minValue = s->fval;
    
    gsl_vector_free(x);
    gsl_vector_free(ss);
    gsl_multimin_fminimizer_free (s);
    
    return status;
}

// -----------------

void Computer::update_values()
{
    minimizer.set_starting_point(frictionC, frictionA);
    int status = minimizer.minimize(&memory);
    if (status!=0) cout << "Fit failed with status: " << status << endl;
    else {
        // FIXME DEBUG
        cout << "Fit successful" << endl;
        cout << minimizer.finalPoint[0] << " - " << minimizer.finalPoint[1] << " -- " << minimizer.minValue << endl;
        
        this->updatedFriction = true;
        this->frictionC = minimizer.finalPoint[0];
        this->frictionA = minimizer.finalPoint[1];
    }
}


// BEGIN: calculate_launch_params code

class Polar
{
public:
    double r;
    double phi;
    
    Polar(double r, double phi) : r(r), phi(phi) {}
    Polar() {}
    
};

double partial_d_on_partial_theta(SimpleSimulator simulator, Launch l);

Launch Computer::calculate_launch_params(Target target, double speed)
{
    // Initial point for zerofinder
    double d = hypot(target.x,target.y);
    double gd = this->simpleSim.get_gravity() * d;
    if (speed*speed < gd )
    {
        throw ComputerException(ComputerException::LOWPOWER);
    }

    double phi = atan(target.y/target.x); // phi restricted to [-pi/2,pi/2]
    if (target.x < 0)
    {
        // Extending to [-pi/2,3/2pi]
        phi += M_PI;
    }

    Launch params(.5*asin(gd/(speed*speed)), phi, speed);
    Polar polarTarget = Polar(d,phi);
    Target currentTarget;
    double error = 0; // FIXME: set it to Inf
    double eps = 1e-5;
    int counter = 0;
    this->simpleSim.set_friction(this->frictionC, this->frictionA);
    
    // TODO: remove the following line
    //double gOnSpeed2 = this->simpleSim.get_gravity()/(speed*speed); // g / (speed *speed) to be used in \partial r / \partial theta
    do
    {
        currentTarget = this->simpleSim.simulate(params).target;
        Polar t = Polar(hypot(currentTarget.x,currentTarget.y),atan(currentTarget.y/currentTarget.x));
        if (currentTarget.x < 0)
        {
            t.phi += M_PI;
        }
        params.phi -= (t.phi - polarTarget.phi); // The derivative is 1
        //params.theta -= (t.r - polarTarget.r) * gOnSpeed2 /2 / cos(2*params.theta) ;
        params.theta -= (t.r - polarTarget.r) / partial_d_on_partial_theta(simpleSim,params);
        error = hypot(target.x - currentTarget.x,target.y - currentTarget.y);
        ++counter;
    }
    while (error > d*eps && counter < 1000);
    
    if (error > d*eps)
    {
        cout << "Warning: nonzero reached. Error: " << error/d << endl;
    }
    
    return params;
}

double partial_d_on_partial_theta(SimpleSimulator simulator, Launch l)
{
    /* This function is used to approximate the derivative of d respect
     * to theta using a centered difference
     * 
     * TODO: choose h in some smart way
     */
    double h = 1e-10;
    // Calculate f(x+h)
    Launch launch_plus = l;
    launch_plus.theta = l.theta + h;
    Target target_plus = simulator.simulate(launch_plus).target;
    double d_plus = hypot(target_plus.x,target_plus.y);
    // Calculate f(x-h)
    Launch launch_minus = l;
    launch_plus.theta = l.theta - h;
    Target target_minus = simulator.simulate(launch_minus).target;
    double d_minus = hypot(target_minus.x,target_minus.y);
    // The derivative
    return (d_plus - d_minus)/(2*h);
}

// END: calculate_launch_params code
