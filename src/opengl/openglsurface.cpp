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

#include "openglsurface.h"
#include "../controller.h"
#include "../draw.h"

#include <SDL2/SDL_image.h>
#include <kobold/log.h>

using namespace Farso;

/******************************************************************
 *                           Constructor                          *
 ******************************************************************/
OpenGLSurface::OpenGLSurface(Kobold::String name, int width, int height)
              :Surface(name, width, height)
{
   Draw* draw = Controller::getDraw();

   /* Define Machine Bit Order */
   Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
   rmask = 0xff000000;
   gmask = 0x00ff0000;
   bmask = 0x0000ff00;
   amask = 0x000000ff;
#else
   rmask = 0x000000ff;
   gmask = 0x0000ff00;
   bmask = 0x00ff0000;
   amask = 0xff000000;
#endif

   /* Define power of two dimensions */
   realWidth = draw->smallestPowerOfTwo(width);
   realHeight = draw->smallestPowerOfTwo(height);

   /* Create the surface */
   surface = SDL_CreateRGBSurface(SDL_SWSURFACE, realWidth, realHeight, 32,
                                  rmask, gmask, bmask, amask);

   /* Clear the surface */
   lock();
   clear();
   unlock();
}

/******************************************************************
 *                           Constructor                          *
 ******************************************************************/
OpenGLSurface::OpenGLSurface(Kobold::String filename, Kobold::String group)
              :Surface(filename, group)
{
   Draw* draw = Controller::getDraw();

   /* Load image from source */
   surface = IMG_Load(filename.c_str());
   if(!surface)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, "Can't open image: '%s'",
            filename.c_str());
      return;
   }

   /* Check if power of two image */
   realWidth = surface->w;
   realHeight = surface->h;

   this->width = surface->w;
   this->height = surface->h;

   if( ((surface->w) != draw->smallestPowerOfTwo(surface->w)) ||
       ((surface->h) != draw->smallestPowerOfTwo(surface->h)) )
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL, 
            "Warning: loaded non-power of two image: '%s' (%d x %d)",
            filename.c_str(), surface->w, surface->h);
   }
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
OpenGLSurface::~OpenGLSurface()
{
   if(surface != NULL)
   {
      SDL_FreeSurface(surface);
   }
}

/******************************************************************
 *                                lock                            *
 ******************************************************************/
void OpenGLSurface::lock()
{
   if(SDL_MUSTLOCK(surface))
   {
      SDL_LockSurface(surface);
   }
}

/******************************************************************
 *                               unlock                           *
 ******************************************************************/
void OpenGLSurface::unlock()
{
   if(SDL_MUSTLOCK(surface))
   {
      SDL_UnlockSurface(surface);
   }
}

/******************************************************************
 *                            getRealWidth                        *
 ******************************************************************/
int OpenGLSurface::getRealWidth()
{
   return realWidth;
}

/******************************************************************
 *                           getRealHeight                        *
 ******************************************************************/
int OpenGLSurface::getRealHeight()
{
   return realHeight;
}

/******************************************************************
 *                            getSurface                          *
 ******************************************************************/
SDL_Surface* OpenGLSurface::getSurface()
{
   return surface;
}

