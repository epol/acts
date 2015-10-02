# Makefile
# 
# This file is part of acts
# 
# Copyright (C) 2015 Enrico Polesel <enrico.polesel@sns.it>,
# Copyright (C) 2015 Andrea Stacchiotti <andrea.stacchiotti@sns.it>
# 
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or (at
# your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301 USA.
# 


CXX=g++ -std=gnu++11
CPPFLAGS=-g -Og -Wall -Wextra -pedantic -lgsl -lgslcblas -lm
OBJS=main.o worldSim.o computer.o simulator.o utilities.o
SDLFLAGS=-D_REENTRANT -I/usr/include/SDL2 -lSDL2 -lpthread


build: $(OBJS)
	$(CXX) $(OBJS) $(CPPFLAGS) -o main

main.o: main.cpp utilities.hpp simulator.hpp computer.hpp worldSim.hpp
	$(CXX) -c main.cpp -o main.o $(CPPFLAGS)

worldSim.o: worldSim.cpp utilities.hpp simulator.hpp worldSim.hpp
	$(CXX) -c worldSim.cpp -o worldSim.o $(CPPFLAGS)

computer.o: computer.cpp utilities.hpp simulator.hpp computer.hpp
	$(CXX) -c computer.cpp -o computer.o $(CPPFLAGS)

simulator.o: simulator.cpp utilities.hpp simulator.hpp
	$(CXX) -c simulator.cpp -o simulator.o $(CPPFLAGS)

utilities.o: utilities.cpp utilities.hpp
	$(CXX) -c utilities.cpp -o utilities.o $(CPPFLAGS)

EmptyApp.o: EmptyApp.hpp
	$(CXX) -c EmptyApp.cpp -o EmptyApp.o $(CPPFLAGS) $(SDLFLAGS)

sdl.o: sdl.cpp EmptyApp.hpp EmptyApp.cpp utilities.hpp worldSim.hpp computer.hpp
	$(CXX) -c sdl.cpp -o sdl.o $(CPPFLAGS) $(SDLFLAGS)

sdl: $(OBJS) sdl.o EmptyApp.o
	$(CXX) utilities.o computer.o worldSim.o simulator.o EmptyApp.o sdl.o $(CPPFLAGS) $(SDLFLAGS) -o sdl

clean:
	-rm $(OBJS) main

rebuild: clean build
