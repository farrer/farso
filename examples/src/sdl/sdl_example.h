
#ifndef _farso_sdl_example_h
#define _farso_sdl_example_h

#include "../common.h"

#include "sdl_app.h"

namespace FarsoExample
{

class SDLExample : public SDLApp
{
    public:
       SDLExample();
       ~SDLExample();

       void init();

       bool shouldQuit();

       void step(bool leftButtonPressed, bool rightButtonPressed,
             int mouseX, int mouseY);

    private:
       /*! The example itself */
       Example* example;
};

}

#endif

