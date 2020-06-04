
#include "sdl_example.h"
#include <kobold/log.h>

using namespace FarsoExample;

/************************************************************************
 *                              SDLExample                           *
 ************************************************************************/
SDLExample::SDLExample()
{
   example = NULL;
}

/************************************************************************
 *                             ~SDLExample                           *
 ************************************************************************/
SDLExample::~SDLExample()
{
   if(example != NULL)
   {
      delete example;
   }
}

/************************************************************************
 *                                   init                               *
 ************************************************************************/
void SDLExample::init()
{
   example = new Example();
   example->init(&loader, renderer); 
}

/************************************************************************
 *                                shouldQuit                            *
 ************************************************************************/
bool SDLExample::shouldQuit()
{
   return example->shouldQuit();
}


/************************************************************************
 *                                   run                                *
 ************************************************************************/
void SDLExample::step(bool leftButtonPressed, bool rightButtonPressed,
      int mouseX, int mouseY)
{
   example->step(Kobold::Mouse::isLeftButtonPressed(), 
         Kobold::Mouse::isRightButtonPressed(),
         Kobold::Mouse::getX(), Kobold::Mouse::getY());
}

/*********************************************************************
 *                           Main Code                               *
 *********************************************************************/
int main(int argc, char **argv)
{
   SDLExample* example = new SDLExample();
   example->run();
   delete example;
}


