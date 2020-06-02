
#include "opengl_example.h"
#include <kobold/log.h>

using namespace FarsoExample;

/************************************************************************
 *                              OpenGLExample                           *
 ************************************************************************/
OpenGLExample::OpenGLExample()
{
   example = NULL;
}

/************************************************************************
 *                             ~OpenGLExample                           *
 ************************************************************************/
OpenGLExample::~OpenGLExample()
{
   if(example != NULL)
   {
      delete example;
   }
}

/************************************************************************
 *                                   init                               *
 ************************************************************************/
void OpenGLExample::init()
{
   example = new Example();
   example->init(&fontLoader, Farso::RENDERER_TYPE_OPENGL, NULL);
}

/************************************************************************
 *                                shouldQuit                            *
 ************************************************************************/
bool OpenGLExample::shouldQuit()
{
   return example->shouldQuit();
}


/************************************************************************
 *                                   run                                *
 ************************************************************************/
void OpenGLExample::step(bool leftButtonPressed, bool rightButtonPressed,
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
   OpenGLExample* example = new OpenGLExample();
   example->run();
   delete example;
}


