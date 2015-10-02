/* 
 * EmptyApp.hpp
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

#ifndef _EMPTYAPP_HPP
#define _EMPTYAPP_HPP

#include <string>  // for std::string

#include <SDL2/SDL.h>


class EmptyApp
{
protected:
    int width,height;
    std::string windowName;
    bool running;
    SDL_Window* window;
    SDL_Renderer* renderer;
        
    virtual bool init();
    virtual void on_event();
    virtual void on_loop();
    virtual void on_render();
    virtual void on_cleanup();
    
public:
    EmptyApp(int width, int height, std::string windowName);
    ~EmptyApp();
    void run();
};

#endif
