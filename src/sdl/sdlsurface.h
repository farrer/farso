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

#ifndef _farso_sdl_surface_h
#define _farso_sdl_surface_h

#include "../surface.h"
#include <SDL2/SDL.h>

namespace Farso
{

/*! The Surface implementation for SDL Renderer: SDL's one. */
class SDLSurface : public Surface
{
   public:
      /*! Constructor */
      SDLSurface(Kobold::String name, int width, int height);
      /*! Constructor 
       * \param load if should load the image referenced by filename or not */
      SDLSurface(Kobold::String filename, bool load=true);
      /*! Destructor */
      virtual ~SDLSurface();

      /*! Lock the surface to draw.
       * \note: must be called before start to draw on it. */
      void lock();

      /*! Unlock surface after draw.
       * \note: must be called when done with draw on it. */
      void unlock();

      /*! Get the real surface width (usually, surfaces are created as power 
       * of two, so this function will reflect that) */
      int getRealWidth();

      /*! Get the real surface height (usually, surfaces are created as power 
       * of two, so this function will reflect that) */
      int getRealHeight();

      /*! \return SDL_Surface */
      SDL_Surface* getSurface();

   protected:
      /*! Create the surface. Should be called when load == false
       * on image load constructor, as soon as know the image's dimensions
       * to load */
      void createSurface(int width, int height);

   private:

      SDL_Surface* surface; /**< The SDL Surface */

      int realWidth; /**< Width as power of two */
      int realHeight; /**< Height as power of two */
};

}


#endif

