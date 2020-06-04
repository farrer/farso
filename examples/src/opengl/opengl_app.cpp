#include "opengl_app.h"

#include <kobold/log.h>
#include <kobold/keyboard.h>
#include <kobold/mouse.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "../size.h"

using namespace FarsoExample;

#define NORMAL_FPS 35       /**< Minimun FPS to the engine runs smooth */
#define UPDATE_RATE (1000 / NORMAL_FPS) /**< Update Rate in ms */

/************************************************************************
 *                              OpenGLApp                           *
 ************************************************************************/
OpenGLApp::OpenGLApp()
{
   window = NULL;
   Kobold::Log::init(&log);

   /* Try to init SDL */
   if( SDL_Init(SDL_INIT_VIDEO) < 0)
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR, 
            "ERROR: Couldn't init SDL");
      exit(-1);
   }

   /* Try to create our window */
   window = SDL_CreateWindow("farso_opengl_example", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, FARSO_EXAMPLE_WINDOW_WIDTH, 
            FARSO_EXAMPLE_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
   if(window == NULL)
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR, 
           "Couldn't create SDL Window!");
      SDL_Quit();
      exit(-2);
   }

   /* Define OpenGL Context */
   glcontext = SDL_GL_CreateContext(window);

   /* Create farso renderer */
   renderer = new Farso::OpenGLRenderer();
}

/************************************************************************
 *                             ~OpenGLApp                           *
 ************************************************************************/
OpenGLApp::~OpenGLApp()
{
   if(renderer != NULL)
   {
      delete renderer;
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
void OpenGLApp::run()
{
   Uint32 lastTime = 0;
   Uint32 time;

   init();

   while(!shouldQuit())
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

         step(Kobold::Mouse::isLeftButtonPressed(), 
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

