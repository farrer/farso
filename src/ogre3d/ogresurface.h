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

#ifndef _farso_ogre_surface_h
#define _farso_ogre_surface_h

#include "../surface.h"
#include "../sdl/sdlsurface.h"

namespace Farso
{

/*! Surface implementation for Ogre3d. Always use a SDL_Surface to represent it,
 * event if a image loaded from disk (in this case, the Ogre::Image is copied 
 * to the surface after loading). */
class OgreSurface : public SDLSurface
{
   public:
      /*! Constructor, for a new empty drawable surface.
       * \param name -> name of the drawable surface texture to create
       *                (must be unique). 
       * \param width -> width of the surface. Should be power of two. 
       * \param height -> height of the surface. Should be power of two. */
      OgreSurface(Kobold::String name, int width, int height);
      /*! Constructor to load an image from file and use it as a surface.
       * \param filename -> filename of the image file to load 
       * \param group -> group that image belongs to. */
      OgreSurface(Kobold::String filename, Kobold::String group);
      /*! Destructor */
      ~OgreSurface();

   protected:
      bool load(Kobold::String filename, Kobold::String group);

};

}


#endif


