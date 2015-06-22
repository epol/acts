# Makefile
# 
# This file is part of acts
# 
# Copyright (C) 2015 Enrico Polesel, Andrea Stacchiotti
# 
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
# 
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

CXX=g++ -std=gnu++11
CPPFLAGS=-g -Og -Wall -Wextra -pedantic
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
