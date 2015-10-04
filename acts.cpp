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
#include <utility>
#include <list>
#include <random>
#include <string>

#include <SDL2/SDL.h>

#include "utilities.hpp"
#include "computer.hpp"
#include "worldSim.hpp"

#include "EmptyApp.hpp"

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
            if (isfinite(this->desired.x) && isfinite(this->desired.y))
            {
                if (this->desired.distance() < 10)
                {
                    cout << "You asked for a launch at " << this->desired.distance() << "m, are you trying to kill us?" << endl << endl;
                    return;
                }
                cout << "Current friction coeffs: " << this->computer.get_friction().first;
                cout << " - "                       << this->computer.get_friction().second << endl;
                Launch l;
                try
                {
                    l = this->computer.calculate_launch_params(this->desired, 1e3);
                }
                catch (ComputerException e)
                {
                    cout << "Desired target: " << this->desired << endl;
                    cout << "Error calculating the launching parameters: " << e.show_reason() << endl << endl;
                    return;
                }
                cout << "Expected event:  " << Event(l, this->desired, -1) << endl;
                this->w.set_seed(rd());
                Event e = this->w.simulate(l);
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
    TargetApp(const int width, const int height, const double sizex, const double sizey, const double latitude=45, string windowName="ACTS"): 
            EmptyApp(width, height, windowName),
            desired(Target(NAN,NAN)), 
            border_s(-sizey),
            border_n(sizey),
            border_e(-sizex),
            border_w(sizex),
            computer(latitude,0,0),
            w(1e-4, 0.02, 4e-6, latitude)
            {
                w.set_friction_sigmas(1, 1);
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
        // Create a dark blue backgrund
        SDL_SetRenderDrawColor( this->renderer, 0x00,0x00,0x30,0xFF);   // dark blue
        SDL_RenderClear( this->renderer );
        
        SDL_SetRenderDrawColor( this->renderer, 0x00, 0xFF, 0x00, 0xFF ); // RGB alpha
        this->draw_cross(0,0);
        if ((this->desired.x != NAN) && (this->desired.y != NAN))
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
            switch(e.key.keysym.sym)
            {
            case SDLK_SPACE:
            case SDLK_f:
                this->fireOrder = true;
                break;
            }
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
    if (argc < 3) {
        cout << "You need to provide the screen size and the latitude (degrees)" << endl;
        return 1;
    }
    
    TargetApp app(600, 600, atof(argv[1]), atof(argv[1]), atof(argv[2]));
    app.run();

    
    return 0;
}

