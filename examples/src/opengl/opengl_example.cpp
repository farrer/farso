
#include "opengl_example.h"
#include <kobold/log.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

using namespace FarsoExample;

#define NORMAL_FPS 35       /**< Minimun FPS to the engine runs smooth */
#define UPDATE_RATE (1000 / NORMAL_FPS) /**< Update Rate in ms */

/************************************************************************
 *                              OpenGLExample                           *
 ************************************************************************/
OpenGLExample::OpenGLExample()
{
   window = NULL;
   Kobold::Log::init(false);

   /* Try to init SDL */
   if( SDL_Init(SDL_INIT_VIDEO) < 0)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "ERROR: Couldn't init SDL");
      exit(-1);
   }

   /* Try to create our window */
   window = SDL_CreateWindow("farso_opengl_example", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, EXAMPLE_WINDOW_WIDTH, 
            EXAMPLE_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
   if(window == NULL)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
           "Couldn't create SDL Window!");
      SDL_Quit();
      exit(-2);
   }

   /* Define OpenGL Context */
   glcontext = SDL_GL_CreateContext(window);

   example = new Example();
   example->init(Farso::RENDERER_TYPE_OPENGL, NULL);
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

   if(window != NULL)
   {
      SDL_GL_DeleteContext(glcontext);
      SDL_DestroyWindow(window);

      window = NULL;
   }

   SDL_Quit();
}

/************************************************************************
 *                                   run                                *
 ************************************************************************/
void OpenGLExample::run()
{
   Uint32 lastTime = 0;
   Uint32 time;

   while(!example->shouldQuit())
   {
      time = SDL_GetTicks();
      if(time - lastTime >= UPDATE_RATE)
      {
         lastTime = time;

         /* Get Keyboard and Mouse State */
         SDL_PumpEvents();
         Kobold::Keyboard::updateState();
         Kobold::Mouse::update();

         /* Let's update things by events (usually, only used for text 
          * editing and mouse release states) */
         SDL_Event event;
         while(SDL_PollEvent(&event))
         {
            if(Kobold::Keyboard::isEditingText())
            {
               Kobold::Keyboard::updateByEvent(event);
            }
            Kobold::Mouse::updateByEvent(event);
         }

         glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

         example->step(Kobold::Mouse::isLeftButtonPressed(), 
                       Kobold::Mouse::isRightButtonPressed(),
                       Kobold::Mouse::getX(), Kobold::Mouse::getY());

         glFlush();
         SDL_GL_SwapWindow(window);
      }
      else if((UPDATE_RATE-1) - (time - lastTime) > 0 )
      {
         /* Must sleep a little */
         SDL_Delay((UPDATE_RATE-1) - (time - lastTime) );
      }
   }
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


