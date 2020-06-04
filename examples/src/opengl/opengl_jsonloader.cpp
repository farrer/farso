#include "opengl_jsonloader.h"

#if FARSO_HAS_RAPIDJSON == 1

#include <kobold/log.h>

using namespace FarsoExample;

/************************************************************************
 *                              OpenGLJsonLoader                           *
 ************************************************************************/
OpenGLJsonLoader::OpenGLJsonLoader()
{
   jsonLoader = NULL;
}

/************************************************************************
 *                             ~OpenGLJsonLoader                           *
 ************************************************************************/
OpenGLJsonLoader::~OpenGLJsonLoader()
{
   if(jsonLoader != NULL)
   {
      delete jsonLoader;
   }
}

/************************************************************************
 *                                   init                               *
 ************************************************************************/
void OpenGLJsonLoader::init()
{
   jsonLoader = new JsonLoader();
   jsonLoader->init(&loader, renderer);
}

/************************************************************************
 *                                shouldQuit                            *
 ************************************************************************/
bool OpenGLJsonLoader::shouldQuit()
{
   return jsonLoader->shouldQuit();
}


/************************************************************************
 *                                   run                                *
 ************************************************************************/
void OpenGLJsonLoader::step(bool leftButtonPressed, bool rightButtonPressed,
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
   OpenGLJsonLoader* jsonLoader = new OpenGLJsonLoader();
   jsonLoader->run();
   delete jsonLoader;
}

#endif

