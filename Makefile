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
OBJS=main.o computer.o fitter.o simulator.o utilities.o 

build: $(OBJS)
	$(CXX) $(OBJS) $(CPPFLAGS) -o main

main.o: main.cpp utilities.hpp simulator.hpp
	$(CXX) -c main.cpp -o main.o $(CPPFLAGS)

computer.o: computer.cpp utilities.hpp simulator.hpp fitter.hpp computer.hpp
	$(CXX) -c computer.cpp -o computer.o $(CPPFLAGS)

fitter.o: fitter.cpp utilities.hpp fitter.hpp
	$(CXX) -c fitter.cpp -o fitter.o $(CPPFLAGS)

simulator.o: simulator.cpp utilities.hpp simulator.hpp
	$(CXX) -c simulator.cpp -o simulator.o $(CPPFLAGS)

utilities.o: utilities.cpp utilities.hpp
	$(CXX) -c utilities.cpp -o utilities.o $(CPPFLAGS)

clean:
	-rm $(OBJS) main

rebuild: clean build
