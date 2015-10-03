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
#include <gsl/gsl_vector.h>
#include <gsl/gsl_multimin.h>
#include <iostream> // FIXME: we are not using it for debug

#include "computer.hpp"
#include "utilities.hpp"
#include "simulator.hpp"

using namespace std;

Vec3D SimpleSimulator::calculate_friction(const Vec3D position, const Vec3D velocity)
// Calculate the acceleration due to friction at a given altitude and velocity
{
    double friction_coeff = this->frictionC * (1 -this->frictionA * position.z);
    return -friction_coeff * velocity;
}

// -----------------

void EventMemory::add(const Event e)
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


Launch Computer::calculate_launch_params(const Target target, const double speed)
{
    // Initial point for zerofinder using balistic without friction
    double d = target.distance();
    double gd = simpleSim.get_gravity() * d;
    if (speed*speed < gd )
    {
        throw ComputerException(ComputerException::LOWPOWER);
    }
    double theta = .5*asin(gd/(speed*speed));
    double phi = target.phi();
    
    // Update the simultor dtime
    this->simpleSim.set_dtime(d*this->relativeDtime/speed);
    
    // Read the friction values
    //this->update_values();
    this->simpleSim.set_friction(this->frictionC, this->frictionA);
    
    // Error constants
    double eps = 1e-5;
    double eps2 = 1e-2;
    double epsd = eps*target.distance();
    double error = 0; 
    
    // Finite differences increments
    double htr = 1e-3;
    double hp = 1e-5;
    double maxdthetar = 5e-1;
    
    int iteration_count = 0;
    do
    {
        // Try current parameters
        Event e = this->simulate(Launch(theta,phi,speed));
        Target currentTarget = e.target;
        Target diffTarget = currentTarget - target;
        error = diffTarget.distance();
        
        // Use finite difference to approximante the Jacobian
        double ht = htr*theta;
        Target targetTP = this->simulate(Launch(theta+ht,phi,speed)).target;
        Target targetTM = this->simulate(Launch(theta-ht,phi,speed)).target;
        
        Target targetPP = this->simulate(Launch(theta,phi+hp,speed)).target;
        Target targetPM = this->simulate(Launch(theta,phi-hp,speed)).target;

        
        double J11 = (targetTP.x - targetTM.x)/(2*ht);
        double J12 = (targetPP.x - targetPM.x)/(2*hp);
        double J21 = (targetTP.y - targetTM.y)/(2*ht);
        double J22 = (targetPP.y - targetPM.y)/(2*hp);
        
        // Invert the Jacobian
        double den = 1/(J11*J22-J12*J21);
        double K11 = den*J22;
        double K12 = -den*J12;
        double K21 = -den*J21;
        double K22 = den*J11;
        
        /* in case of debug
        cout << J11 << " " << J12 << endl;
        cout << J21 << " " << J22 << endl;

        cout << K11 << " " << K12 << endl;
        cout << K21 << " " << K22 << endl;
        */
        
        // Prevent the method from choosing too large increments for theta
        double maxdtheta = maxdthetar*theta;
        double deltatheta = ((K11*diffTarget.x + K12*diffTarget.y));
        if (fabs(deltatheta) > maxdtheta)
        {
            deltatheta = (deltatheta/fabs(deltatheta))*maxdtheta;  // aka sig(deltatheta)*maxdtheta
        }
        // Finally update the parameters with Netwon
        theta = theta - deltatheta;
        phi = phi - (K21*diffTarget.x + K22*diffTarget.y);
        
        ++iteration_count;
    }
    while ((iteration_count < 1000) && (error > epsd));
    
    if (error > eps2*d  )
    {
        // We hit too far from the target, maybe we haven't enough power? TODO: maybe some better condition?
        throw ComputerException(ComputerException::LOWPOWER);
    }
    
    // Sometimes phi is out of [0,2pi]
    phi = mod(phi,2*M_PI);
    
    return Launch(theta,phi,speed);
}

