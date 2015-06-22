/* 
 * worldSim.hpp
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
 
#ifndef _WORLD_HPP
#define _WORLD_HPP

#include <random>
#include "simulator.hpp"

class WorldSimulator : public Simulator
{
private:
    double frictionCsigma;
    double frictionAsigma;
    
    std::default_random_engine g;
    
    using Simulator::Simulator;  // inherit constructors
    
protected:
    virtual Vec3D calculate_friction(Vec3D, Vec3D);

public:
    
    WorldSimulator(double dtime, double frictionC, double frictionA, double latitude, uint seed):
        Simulator(dtime, frictionC, frictionA, latitude)
    {
        this->g = std::default_random_engine(seed);
    }
    
    inline void set_friction_sigmas(double frictionCsigma, double frictionAsigma)
    {
        this->frictionCsigma = frictionCsigma;
        this->frictionAsigma = frictionAsigma;
    }
    
};

#endif // _WORLD_HPP