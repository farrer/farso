/* 
  Farso: a simple GUI.
  Copyright (C) DNTeam <dnt@dnteam.org>
 
  This file is part of Farso.
 
  Farso is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Farso is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with Farso.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _farso_sdl_junction_h
#define _farso_sdl_junction_h

#include <SDL2/SDL.h>
#include "controller.h"

namespace Farso
{

   class SDLJunctionInfo : public RendererJunctionInfo 
   {
      public:
         /* Constructor.
          * \param sdlRdr pointer to the SDL_Renderer farso should use */
         SDLJunctionInfo(SDL_Renderer* sdlRdr) { this->sdlRenderer = sdlRdr; };
         ~SDLJunctionInfo() {};

         SDL_Renderer* sdlRenderer;
   };

   /* Junction for SDL widget renderer usage */
   class SDLJunction : public ControllerRendererJunction
   {
      public:
         /*! Constructor 
          * \param junction Pointer to the needed information needed by SDL
          *        farso integration */
         SDLJunction(SDLJunctionInfo* info);
         /*! Destructor */
         ~SDLJunction();
         
         /*! \return pointer to the SDLRenderer used */
         SDL_Renderer* getRenderer() { return renderer; };

         void enter2dMode(){};
         void restore3dMode(){};
         const bool shouldManualRender() const {return true;};

      private:

         SDL_Renderer* renderer; /**< The SDL renderer of the window */
   };

}

#endif

