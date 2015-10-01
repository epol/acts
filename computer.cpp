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
    
    Computer* comp = (Computer*)params;
    EventMemory* m = comp->get_memory();
    comp->set_simpleSim_friction(fC, fA);
    
    double chi2_accumulator = 0;
    
    for (int i=0; i< m->size(); ++i)
    {
        Event ev_real = m->get()[i];
        Event ev_sim = comp->simulate(ev_real.launch);
        
        Target delta = ev_real.target - ev_sim.target;
        double chi2_piece = delta.distance() * delta.distance();
        
        chi2_accumulator += chi2_piece / pow(2, i);
    }
    return chi2_accumulator;
}

int Minimizer::minimize(Computer* comp)
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
    minex_func.params = (void*)comp;  // pass a pointer to computer as parameter

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
    int status = minimizer.minimize(this);
    // FIXME DEBUG
    if (status!=0) cout << "Fit failed with status: " << status << endl;
    else cout << "Fit successful" << endl;
    cout << minimizer.finalPoint[0] << " - " << minimizer.finalPoint[1] << " -- " << minimizer.minValue << endl;
    
    this->updatedFriction = true;
    this->frictionC = minimizer.finalPoint[0];
    this->frictionA = minimizer.finalPoint[1];
}


// BEGIN: calculate_launch_params code

class EquationParam
// To be used as the equation parameters
{
public:
    SimpleSimulator* s;
    double speed;
    Target target;
    
    EquationParam(SimpleSimulator* s, double speed, Target target) : s(s), speed(speed), target(target) {}
};

int equation (const gsl_vector * x, void * param, gsl_vector * f)
{
    const double theta = gsl_vector_get(x,0);
    const double phi = gsl_vector_get(x,1);
    EquationParam * eq_param = (EquationParam *) param;
    SimpleSimulator * s = eq_param -> s;
    double speed = eq_param -> speed;
    Target target = eq_param -> target;
    
    Event e = s->simulate(Launch(theta,phi,speed));
    gsl_vector_set(f,0,e.target.x - target.x);
    gsl_vector_set(f,1,e.target.y - target.y);
    
    return GSL_SUCCESS;
}


Launch Computer::calculate_launch_params(Target target, double speed)
{
    // Initial point for zerofinder
    gsl_vector * l0 = gsl_vector_alloc(2);
    double d = hypot(target.x,target.y);
    double gd = simpleSim.get_gravity() * d;
    if (speed*speed < gd )
    {
        throw ComputerException(ComputerException::LOWPOWER);
    }

    gsl_vector_set(l0,0,.5*asin(gd/(speed*speed))); // set theta
    double phimod = atan(target.y/target.x); // phi restricted to [-pi/2,pi/2]
    if (target.x > 0)
    {
        gsl_vector_set(l0,1,phimod);
    }
    else
    {
        gsl_vector_set(l0,1,phimod + M_PI);
    }
    
    // Initialize a solver
    const gsl_multiroot_fsolver_type * T = gsl_multiroot_fsolver_hybrids; // https://www.gnu.org/software/gsl/manual/html_node/Algorithms-without-Derivatives.html#Algorithms-without-Derivatives
    gsl_multiroot_fsolver * s = gsl_multiroot_fsolver_alloc (T, 2);
    if (s == nullptr)
    {
        throw ComputerException(ComputerException::SOLVERINIT);
    }
    // construct the equation
    gsl_multiroot_function F;
    F.f = &equation;
    F.n = 2;
    this->simpleSim.set_friction(this->frictionC, this->frictionA);
    EquationParam param = EquationParam( &simpleSim, speed, target);
    F.params = & param;
    // Set the solver
    gsl_multiroot_fsolver_set(s,&F,l0);   // the value of x0 (the initial point) is copied
    // Garbage collection
    gsl_vector_free(l0);
    // Begin iteration
    int iteration_count = 0;
    const double eps = 5e-3*d;
    while ((iteration_count < 100) && (gsl_multiroot_test_residual(gsl_multiroot_fsolver_f(s),eps)==GSL_CONTINUE) )
    {
        int status = gsl_multiroot_fsolver_iterate(s);
        if (status)
        {
            //TODO: should we do something different?
            cout << "Warning: error at the " << iteration_count << "th iteration: " << gsl_strerror(status) << endl;
            break;
        }
        ++iteration_count;
    }
    
    if (gsl_multiroot_test_residual(gsl_multiroot_fsolver_f(s),eps)!=GSL_SUCCESS)
    {
        //TODO: should we do something different?
        cout << "Warning: we haven't reached a zero\n";
    }
    
    // Get the found root
    gsl_vector * root = gsl_multiroot_fsolver_root(s);
    Launch l(gsl_vector_get(root,0),gsl_vector_get(root,1),speed);
    
    // Garbage collection
    gsl_multiroot_fsolver_free (s);
    
    return l;
}


// END: calculate_launch_params code
