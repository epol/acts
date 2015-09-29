/* 
 * worldSim.cpp
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
#include <random>
#include "worldSim.hpp"

using namespace std;

Vec3D WorldSimulator::calculate_friction(Vec3D position, Vec3D velocity)
    // Calculate the acceleration due to friction at a given altitude and velocity
{
    normal_distribution<double> gauss(0, 1);
    double realC = frictionC + frictionCsigma * gauss(g);
    double realA = frictionA + frictionAsigma * gauss(g);
    double friction_coeff = realC * exp(-realA * position.z);
    return -friction_coeff * velocity;
}