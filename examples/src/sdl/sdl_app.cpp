#include "sdl_app.h"

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
 *                              SDLApp                           *
 ************************************************************************/
SDLApp::SDLApp()
{
   window = NULL;
   renderer = NULL;
   Kobold::Log::init(false);

   /* Try to init SDL */
   if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "ERROR: Couldn't init SDL");
      exit(-1);
   }

   /* Try to create our window */
   if(SDL_CreateWindowAndRenderer(FARSO_EXAMPLE_WINDOW_WIDTH, 
            FARSO_EXAMPLE_WINDOW_HEIGHT, 0, &window, &renderer) != 0)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
           "Couldn't create SDL Window and Renderer!");
      SDL_Quit();
      exit(-2);
   }
}

/************************************************************************
 *                             ~SDLApp                           *
 ************************************************************************/
SDLApp::~SDLApp()
{
   if(renderer != NULL)
   {
      SDL_DestroyRenderer(renderer);
   }
   if(window != NULL)
   {
      SDL_DestroyWindow(window);

      window = NULL;
   }

   SDL_Quit();
}

/************************************************************************
 *                                   run                                *
 ************************************************************************/
void SDLApp::run()
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

         SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
         SDL_RenderClear(renderer);

         step(Kobold::Mouse::isLeftButtonPressed(), 
              Kobold::Mouse::isRightButtonPressed(),
              Kobold::Mouse::getX(), Kobold::Mouse::getY());

         SDL_RenderPresent(renderer);
      }
      else if((UPDATE_RATE-1) - (time - lastTime) > 0 )
      {
         /* Must sleep a little */
         SDL_Delay((UPDATE_RATE-1) - (time - lastTime) );
      }
   }
}

