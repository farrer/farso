
#ifndef _farso_sdl_app_h
#define _farso_sdl_app_h

#include <SDL2/SDL.h>
#include <kobold/log.h>

#include "../../../src/sdl/sdlrenderer.h"
#include "../../../src/loader.h"

namespace FarsoExample
{

#define SDL_WINDOW_WIDTH 1024
#define SDL_WINDOW_HEIGHT 768

class SDLApp
{
    public:
       SDLApp();
       virtual ~SDLApp();

       void run();

       virtual void init() = 0;

       virtual bool shouldQuit() = 0;

       virtual void step(bool leftButtonPressed, bool rightButtonPressed,
             int mouseX, int mouseY) = 0; 

    protected:
       /*! The window used */
       SDL_Window* window;
       Kobold::DefaultLog log;
       Farso::DefaultLoader loader;
       Farso::SDLRenderer* renderer;

    private:
       SDL_Renderer* sdlRenderer;

};

}

#endif
