/* 
 * sdl.cpp
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
#include <cmath>
#include <list>
#include <random>
#include <string>

#include <SDL2/SDL.h>

#include "utilities.hpp"
#include "computer.hpp"
#include "worldSim.hpp"

#include "EmptyApp.hpp"

#define LATITUDE 45

using namespace std;

class TargetApp : public EmptyApp
{
private:
    list<Target> history;
    Target desired;
    double border_s;
    double border_n;
    double border_e;
    double border_w;
    
    void draw_cross(double x, double y)
    {
        int sx = (int)(((-x-this->border_w)*(this->width-20))/(this->border_e-this->border_w)) + 10;
        int sy = (int)(((-y-this->border_s)*(this->height-20))/(this->border_n-this->border_s)) + 10;
        
        SDL_RenderDrawLine(this->renderer, sx - 10, sy, sx + 10, sy);
        SDL_RenderDrawLine(this->renderer, sx, sy-10, sx, sy+10);
    }
    
    Computer computer;
    random_device rd;
    WorldSimulator w;
    
    bool fireOrder = false;
    
    void fire()
    {
        if (fireOrder)
        {
            fireOrder = false;
            if (isfinite(desired.x) && isfinite(desired.y))
            {
                Launch l;
                try
                {
                    l = this->computer.calculate_launch_params(this->desired, 1e3);
                }
                catch (ComputerException e)
                {
                    cout << "Error calculating the launching parameters: " << e.show_reason() << endl;
                    return;
                }
                cout << "Expected event:  " << Event(l, this->desired, -1) << endl;
                w.set_seed(rd());
                Event e = w.simulate(l);
                cout << "Real event:      " << e << endl << endl;
                this->add_event(e);
                this->computer.add_event(e);
            }
            else
            {
                cout << "Select a valid target" << endl;
            }
        }
    }


public:
    TargetApp(int width, int height, double sizex, double sizey,std::string windowName="ACTS"): 
            EmptyApp(width,height, windowName), 
            desired(Target(NAN,NAN)), 
            border_s(-sizey),
            border_n(sizey),
            border_e(-sizex),
            border_w(sizex),
            computer(LATITUDE,0,0),
            w(1e-4, 0.05, 0, LATITUDE)
            {
                w.set_friction_sigmas(0.05, 0);
            }
    ~TargetApp()
    {
        this->history.clear();
    }
    
    void add_event(Event e)
    {
        Target t = e.target;
        this->history.push_back(t);
        if (this->history.size() > 10)
        {
            this->history.pop_front();
        }
    }

    virtual void on_render()
    {
        // Create a black backgrund
        SDL_SetRenderDrawColor( this->renderer, 0x00,0x00,0x00,0xFF);   // black                                                                                                                                    
        SDL_RenderClear( this->renderer );
        
        SDL_SetRenderDrawColor( this->renderer, 0x00, 0xFF, 0x00, 0xFF ); // RGB alpha
        this->draw_cross(0,0);
        if ((desired.x != NAN) && (desired.y != NAN))
        {
            SDL_SetRenderDrawColor( this->renderer, 0xFF, 0x00, 0x00, 0xFF );
            this->draw_cross(this->desired.x,this->desired.y);
        }
        
        
        int alpha = 0xFF;
        for (list<Target>::reverse_iterator i = this->history.rbegin() ; i != this->history.rend() ; ++i)
        {
            SDL_SetRenderDrawColor( this->renderer, 0xFF, 0xFF, 0x00, alpha );  // RGB alpha
            this->draw_cross(i->x,i->y);
            alpha /=2;
        }
        
        // Finally, render                                                                                                                                                                                          
        EmptyApp::on_render();
    }
    
    
    virtual void on_loop()
    {
        fire();
    }
    virtual void on_event();
    virtual bool init()
    {
        if (EmptyApp::init())
        {
            SDL_SetRenderDrawBlendMode(this->renderer,SDL_BLENDMODE_BLEND);
            return true;
        }
        else
        {
            return false;
        }
    }
};

void TargetApp::on_event()
/* Events handler, by default we only manage quit events.
 */
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        //quit event
        switch(e.type)
        {
        case SDL_QUIT:
            this->running = false;
            break;
        case SDL_KEYDOWN:
            this->fireOrder = true;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if ((e.button.button == SDL_BUTTON_LEFT) && (e.button.state == SDL_PRESSED))
            {
                double x = -(this->border_w + ((double)e.button.x -10)*(this->border_e-this->border_w)/(this->width-20));
                double y = -(this->border_s + ((double)e.button.y -10)*(this->border_n-this->border_s)/(this->height-20));
                this->desired = Target(x,y);
            }
            break;
        }
    }
}

int main (int argc, char** argv)
{
    if (argc < 2) {
        cout << "You need to provide the screen size" << endl;
        return 1;
    }
    
    TargetApp app(600,600,atof(argv[1]),atof(argv[1]));
    app.run();

    
    return 0;
}


