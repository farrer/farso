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

#ifndef _farso_opengl_surface_h
#define _farso_opengl_surface_h

#include "../surface.h"
#include "../sdl/sdlsurface.h"
#include <SDL2/SDL.h>

namespace Farso
{

/*! The Surface implementation for OpenGL Renderer: SDL's one. */
class OpenGLSurface : public SDLSurface
{
   public:
      /*! Constructor */
      OpenGLSurface(Kobold::String name, int width, int height);
      /*! Constructor */
      OpenGLSurface(Kobold::String filename);
      /*! Destructor */
      ~OpenGLSurface();
};

}


#endif

