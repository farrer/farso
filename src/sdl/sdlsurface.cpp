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

#include "sdlsurface.h"
#include "../controller.h"
#include "../draw.h"

#include <kobold/platform.h>

#if KOBOLD_PLATFORM == KOBOLD_PLATFORM_MACOS
   #include <SDL2_Image/SDL_image.h>
#else 
   #include <SDL2/SDL_image.h>
#endif

#include <kobold/log.h>

using namespace Farso;

/******************************************************************
 *                           Constructor                          *
 ******************************************************************/
SDLSurface::SDLSurface(Kobold::String name, int width, int height)
              :Surface(name, width, height)
{
   createSurface(width, height);
}

/******************************************************************
 *                          createSurface                         *
 ******************************************************************/
void SDLSurface::createSurface(int width, int height)
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
SDLSurface::SDLSurface(Kobold::String filename, Kobold::String group, 
                       bool load)
              :Surface(filename, group)
{
   if(load)
   {
      Draw* draw = Controller::getDraw();

      /* Load image from source */
      surface = IMG_Load(filename.c_str());
      if(!surface)
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
               "Can't open image: '%s'", filename.c_str());
      }

      /* Check if power of two image */
      realWidth = surface->w;
      realHeight = surface->h;

      this->width = surface->w;
      this->height = surface->h;

      if( ((surface->w) != draw->smallestPowerOfTwo(surface->w)) ||
            ((surface->h) != draw->smallestPowerOfTwo(surface->h)) )
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_DEBUG, 
               "Warning: loaded non-power of two image: '%s' (%d x %d)",
               filename.c_str(), surface->w, surface->h);
      }
   }
   else
   {
      surface = NULL;
   }
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
SDLSurface::~SDLSurface()
{
   if(surface != NULL)
   {
      SDL_FreeSurface(surface);
   }
}

/******************************************************************
 *                                lock                            *
 ******************************************************************/
void SDLSurface::lock()
{
   if(SDL_MUSTLOCK(surface))
   {
      SDL_LockSurface(surface);
   }
}

/******************************************************************
 *                               unlock                           *
 ******************************************************************/
void SDLSurface::unlock()
{
   if(SDL_MUSTLOCK(surface))
   {
      SDL_UnlockSurface(surface);
   }
}

/******************************************************************
 *                            getRealWidth                        *
 ******************************************************************/
int SDLSurface::getRealWidth()
{
   return realWidth;
}

/******************************************************************
 *                           getRealHeight                        *
 ******************************************************************/
int SDLSurface::getRealHeight()
{
   return realHeight;
}

/******************************************************************
 *                            getSurface                          *
 ******************************************************************/
SDL_Surface* SDLSurface::getSurface()
{
   return surface;
}

