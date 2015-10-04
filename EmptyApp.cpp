/* 
 * EmptyApp.cpp
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


#include "EmptyApp.hpp"

#include <iostream>
#include <string>  // for std::string

#include <SDL2/SDL.h>


using namespace std;

EmptyApp::EmptyApp(int width, int height, string windowName)
{
    this->running = false;
    this->window = NULL;
    this->width = width;
    this->height = height;
    this->windowName = windowName;
    
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) 
    {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        throw -1;
    }

}

EmptyApp::~EmptyApp()
{
    SDL_Quit();
}

void EmptyApp::run()
{
    if (!this->init())
    {
        cout << "Error initializing App1\n";
    }
    // Main loop
    while (this->running)
    {
        this->on_event();
        this->on_loop();
        this->on_render();
    }
    this->on_cleanup();
}


bool EmptyApp::init()
/* app initialization function
 */
{
    //Create window
    if ((this->window = SDL_CreateWindow( &windowName[0],
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        this->width,
                                        this->height,
                                        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
                                        ) ) == NULL )
    {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    
    // Create renderer
    if ((this->renderer = SDL_CreateRenderer (  this->window,
                                                -1,
                                                SDL_RENDERER_ACCELERATED
                                            ) ) == NULL)
    {
        cout << "Renderer could not be crated! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }
    
    this->running = true;
    return true;
}

void EmptyApp::on_event()
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
    }
}

void EmptyApp::on_loop()
/* Main loop
 */
{
    
    
}

void EmptyApp::on_render()
/* Render things on the window
 */
{
    // update the window
    SDL_RenderPresent( this->renderer );
    // some delay to release the processor
    SDL_Delay(32);
}

void EmptyApp::on_cleanup()
/* Garbage collector
 */
{
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
}

