/* 
 * main.cpp
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
#include <random>

#include "utilities.hpp"
#include "simulator.hpp"
#include "computer.hpp"
#include "worldSim.hpp"

using namespace std;

int main(int, char** argv)
{
    Computer c(45);
    SimpleSimulator s(0.05, 0, 0, 45);
    Launch l = c.calculate_launch_params(Target(atof(argv[1]), atof(argv[2])),1e3);
    Event e = s.simulate(l);
    cout << e << endl;
    
    random_device rd;
    WorldSimulator w(0.05, 0, 0, 45, rd());
    w.set_friction_sigmas(0.01, 0.01);
    e = w.simulate(l);
    cout << e << endl;

    
    return 0;
}
