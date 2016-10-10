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

#ifndef _farso_opengl_widget_renderer_h
#define _farso_opengl_widget_renderer_h

#include "../widgetrenderer.h"
#include <SDL2/SDL_opengl.h>

namespace Farso
{

/*! The WidgetRenderer for OpenGL/SDL */
class OpenGLWidgetRenderer: public WidgetRenderer
{
   public:
      OpenGLWidgetRenderer(int width, int height, 
            ControllerRendererJunction* junction); 
      ~OpenGLWidgetRenderer();

      void uploadSurface();

   protected:
      
      void doSetPosition(float x, float y);
      void doHide();
      void doShow();
      void doRender(float depth);

   private:

      int posX;        /**< current X position on screen */
      int posY;        /**< current Y position on screen */
      GLuint texture;  /**< GL texture for the renderer */
      float propX;     /**< Proportional texture coordinate */
      float propY;     /**< Proportional texture coordinate */
};

}


#endif


