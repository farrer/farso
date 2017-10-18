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
   /* Junction for SDL widget renderer usage */
   class SDLJunction : public ControllerRendererJunction
   {
      public:
         /*! Constructor 
          * \param sdlRenderer pointer to the SDL_Renderer farso should use */
         SDLJunction(SDL_Renderer* sdlRenderer);
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

