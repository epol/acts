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
#include <vector>
#include <random>

#include <SDL2/SDL.h>

#include "utilities.hpp"
#include "computer.hpp"
#include "worldSim.hpp"

#include "EmptyApp.hpp"

using namespace std;

class TargetApp : public EmptyApp
{
private:
    vector<Target> history;
    Target desired;
    double border_s = 0;
    double border_n = 0;
    double border_e = 0;
    double border_w = 0;
    
    void draw_cross(double x, double y)
    {
        int sx = (int)(((x-this->border_w)*(this->width-20))/(this->border_e-this->border_w)) + 10;
        int sy = (int)(((y-this->border_s)*(this->height-20))/(this->border_n-this->border_s)) + 10;
        
        SDL_RenderDrawLine(this->renderer, sx - 10, sy, sx + 10, sy);
        SDL_RenderDrawLine(this->renderer, sx, sy-10, sx, sy+10);
    }
    
    Computer computer;
    bool fireOrder = false;
    
    void fire()
    {
        if (fireOrder)
        {
            fireOrder = false;
            Launch l;
            try
            {
                l = this->computer.calculate_launch_params(this->desired, 1e3);
            }
            catch (ComputerException e)
            {
                cout << "Error calculating the launching parameters: " << e.show_reason() << endl;
                exit(1);
            }
            Event e = this->computer.simulate(l);
            this->add_event(e);
        }
    }

public:
    TargetApp(Target desired, int width, int height, std::string windowName): 
            EmptyApp(width,height, windowName), 
            desired(desired), 
            computer(45,10,10)
    {
    }
    ~TargetApp()
    {
        this->history.clear();
    }
    void add_event(Event e)
    {
        Target t = e.target;
        this->history.push_back(t);
        if (t.x > border_e)
        {
            border_e = t.x;
        }
        if (t.x < border_w)
        {
            border_w = t.x;
        }
        if (t.y > border_n)
        {
            border_n = t.y;
        }
        if (t.y < border_s)
        {
            border_s = t.y;
        }
    }

    virtual void on_render()
    {
        // Create a white backgrund                                                                                                                                                                                 
        SDL_SetRenderDrawColor( this->renderer, 0x00,0x00,0x00,0xFF);   // black                                                                                                                                    
        SDL_RenderClear( this->renderer );
        
        SDL_SetRenderDrawColor( this->renderer, 0xFF, 0xFF, 0xFF, 0xFF ); // RGB alpha
/*        if (SDL_RenderDrawPoint(this->renderer,
                                (int)((this->border_w*this->width)/(this->border_e+this->border_w)),
                                (int)((this->border_s*this->width)/(this->border_n+this->border_s))
                                ) != 0)
        {
            cout << "Unable to render point, error: "<< SDL_GetError() << endl;
        }*/
        this->draw_cross(0,0);
        int alpha = 0xFF;
        
        for (vector<Target>::reverse_iterator i = this->history.rbegin() ; i != this->history.rend() ; ++i)
        {
            SDL_SetRenderDrawColor( this->renderer, 0xFF, 0, 0, alpha );  // RGB alpha
/*            if (SDL_RenderDrawPoint(renderer,
                                    (int)(((i->x + this->border_w)*this->width)/(this->border_e+this->border_w)),
                                    (int)(((i->y + this->border_s)*this->height)/(this->border_n+this->border_s))
                                    ) != 0)
            {
                cout << "Unable to render point, error: "<< SDL_GetError() << endl;
            }*/
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
//    virtual bool init();
};

void TargetApp::on_event()
/* Events handler, by default we only manage quit events.
 */
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        //quit event
        if (e.type == SDL_QUIT)
        {
            this->running = false;
        }
        if (e.type == SDL_KEYDOWN)
        {
            this->fireOrder = true;
        }
    }
}

int main (int argc, char** argv)
{
    if (argc < 3) {
        cout << "You need to provide 2 arguments (x,y) for targeting" << endl;
        return 1;
    }
    
    Computer c(45, 10, 10);
    Target t = Target(atof(argv[1]), atof(argv[2]));
    Launch l;
    try
    {
        l = c.calculate_launch_params(t, 1e3);
    }
    catch (ComputerException e)
    {
        cout << "Error calculating the launching parameters: " << e.show_reason() << endl;
        exit(1);
    }
    Event e = c.simulate(l);
    
    
    //TargetApp(Target desired, int width, int height, std::string windowName): EmptyApp(width,height, windowName) , desired(desired) {};
    TargetApp app(t,600,600, "ACTS");
    app.add_event(e);
    app.run();

    
    
    
    return 0;
}


