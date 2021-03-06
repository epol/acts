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
    double friction1sigma;
    double friction2sigma;
    
    std::default_random_engine g;
    
    using Simulator::Simulator;  // inherit constructors
    
protected:
    virtual Vec3D calculate_friction(const Vec3D);

public:
    WorldSimulator(const double dtime, const double friction1, const double friction2, const double latitude):
        Simulator(dtime, friction1, friction2, latitude) {}
    
    inline void set_seed(unsigned int seed)
    {
        this->g = std::default_random_engine(seed);
    }
    
    WorldSimulator(const double dtime, const double friction1, const double friction2, const double latitude, unsigned int seed):
        WorldSimulator(dtime, friction1, friction2, latitude)
    {
        this->set_seed(seed);
    }
    
    inline void set_friction_sigmas(double friction1sigma, double friction2sigma)
    {
        this->friction1sigma = friction1sigma;
        this->friction2sigma = friction2sigma;
    }
    
};

#endif // _WORLD_HPP
