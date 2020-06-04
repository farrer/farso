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

#ifndef _farso_sdl_renderer_h
#define _farso_sdl_renderer_h

#include <SDL2/SDL.h>
#include "../renderer.h"

namespace Farso
{

/*! SDL Renderer implemenation */
class SDLRenderer : public Renderer
{
   public:
      /*! Constructor 
       * \param sdlRenderer pointer to the SDL_Renderer Farso should use. */
      SDLRenderer(SDL_Renderer* sdlRenderer);
      /*! Destructor */
      virtual ~SDLRenderer();

      /*! \return new SDLWidgetRenderer */
      WidgetRenderer* createWidgetRenderer(int width, int height) override;

      /*! \return pointer to the SDL_Renderer used */
      SDL_Renderer* getSDLRenderer() { return sdlRenderer; };

      void enter2dMode() override {};
      void restore3dMode() override {};
      const bool shouldManualRender() const override { return true; };
      Surface* loadImageToSurface(const Kobold::String& filename) override;

   private:
      SDL_Renderer* sdlRenderer; /**< The renderer from SDL */

};

}

#endif

