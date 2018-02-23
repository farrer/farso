#include "sdl_jsonloader.h"
#include "../../../src/sdl/sdljunction.h"

#if FARSO_HAS_RAPIDJSON == 1

#include <kobold/log.h>

using namespace FarsoExample;

/************************************************************************
 *                              SDLJsonLoader                           *
 ************************************************************************/
SDLJsonLoader::SDLJsonLoader()
{
   jsonLoader = NULL;
}

/************************************************************************
 *                             ~SDLJsonLoader                           *
 ************************************************************************/
SDLJsonLoader::~SDLJsonLoader()
{
   if(jsonLoader != NULL)
   {
      delete jsonLoader;
   }
}

/************************************************************************
 *                                   init                               *
 ************************************************************************/
void SDLJsonLoader::init()
{
   Farso::SDLJunctionInfo info(renderer);
   jsonLoader = new JsonLoader();
   jsonLoader->init(Farso::RENDERER_TYPE_SDL, &info);
}

/************************************************************************
 *                                shouldQuit                            *
 ************************************************************************/
bool SDLJsonLoader::shouldQuit()
{
   return jsonLoader->shouldQuit();
}


/************************************************************************
 *                                   run                                *
 ************************************************************************/
void SDLJsonLoader::step(bool leftButtonPressed, bool rightButtonPressed,
      int mouseX, int mouseY)
{
   jsonLoader->step(Kobold::Mouse::isLeftButtonPressed(), 
         Kobold::Mouse::isRightButtonPressed(),
         Kobold::Mouse::getX(), Kobold::Mouse::getY());
}

/*********************************************************************
 *                           Main Code                               *
 *********************************************************************/
int main(int argc, char **argv)
{
   SDLJsonLoader* jsonLoader = new SDLJsonLoader();
   jsonLoader->run();
   delete jsonLoader;
}

#endif

