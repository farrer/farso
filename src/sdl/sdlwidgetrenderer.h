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

#ifndef _farso_sdl_widget_renderer_h
#define _farso_sdl_widget_renderer_h

#include "../widgetrenderer.h"
#include <SDL2/SDL.h>
#include "sdljunction.h"

namespace Farso
{
   /*! WidgetRenderer for SDL */
   class SDLWidgetRenderer : public WidgetRenderer
   {
      public:
         SDLWidgetRenderer(int width, int height, 
               ControllerRendererJunction* junction);
         ~SDLWidgetRenderer();

         void uploadSurface();
         void setRenderQueueSubGroup(int renderQueueId){};

      protected:
         void createSurface();
         void doSetPosition(float x, float y);
         void doHide();
         void doShow();
         void doRender();

      private:
         SDL_Texture* texture; /**< SDL_Texture related */
         SDLJunction* sdlJunction; /**< The junction of this WidgetRenderer */
         int posX;        /**< current X position on screen */
         int posY;        /**< current Y position on screen */
   };
}

#endif

