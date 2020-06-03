#ifndef _farso_sdl_json_loader_h
#define _farso_sdl_json_loader_h

#include "farsoconfig.h"

#if FARSO_HAS_RAPIDJSON == 1

#include "../jsonloader.h"

#include "sdl_app.h"

namespace FarsoExample
{

class SDLJsonLoader : public SDLApp
{
    public:
       SDLJsonLoader();
       ~SDLJsonLoader();

       void init();

       bool shouldQuit();

       void step(bool leftButtonPressed, bool rightButtonPressed,
             int mouseX, int mouseY);

    private:
       JsonLoader* jsonLoader;
       Farso::DefaultLoader loader;
};

}

#endif

#endif

