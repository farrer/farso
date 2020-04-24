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

#include "ogredraw.h"
#include "ogresurface.h"

#include <kobold/platform.h>
#include <kobold/log.h>

#if OGRE_VERSION_MAJOR == 2 && OGRE_VERSION_MINOR >= 2
   #include <OGRE/OgreTextureBox.h>
#else
   #include <OGRE/OgrePixelBox.h>
#endif

#include <string.h>

using namespace Farso;

/******************************************************************
 *                           constructor                          *
 ******************************************************************/
OgreDraw::OgreDraw()
         :SDLDraw()
{
}

/******************************************************************
 *                            destructor                          *
 ******************************************************************/
OgreDraw::~OgreDraw()
{
}

/******************************************************************
 *                        doOgreImageCopy                         *
 ******************************************************************/

/* Note: we are copying pixel by pixel the image. It's inneficient, but a
 * simple way to support multiple Ogre::Image's formats.  Anyway, it's only
 * done once per image, and at it's load time, so not much a big problem at
 * all. */

#if OGRE_VERSION_MAJOR == 2 && OGRE_VERSION_MINOR >= 2
void OgreDraw::doOgreImageCopy(OgreSurface* target, Ogre::Image2* image)
{
   Ogre::TextureBox texBox = image->getData(0);

   size_t width = target->getWidth();
   size_t height = target->getHeight();
   SDLSurface* sdlSurface = static_cast<SDLSurface*>(target);

   assert(width == static_cast<size_t>(image->getWidth()));
   assert(height == static_cast<size_t>(image->getHeight()));

   const size_t bytesPerRow = texBox.bytesPerRow;
   const size_t bytesPerPixel = texBox.bytesPerPixel;
   Uint8 sr, sg, sb, sa;

   for(size_t y = 0; y < height; y++)
   {
      for(size_t x = 0; x < width; x++)
      {
         /* Get pixel from source */
         Ogre::uint8* pixel = reinterpret_cast<Ogre::uint8*>(texBox.data) +
            y * bytesPerRow + x * bytesPerPixel;
         getPixel(pixel, Ogre::PFG_RGBA8_UNORM_SRGB, sr, sg, sb, sa);

         /* Set it on dest */
         SDLDraw::setPixel(sdlSurface, x, y, sr, sg, sb, sa);
      }
   }

}
#else
void OgreDraw::doOgreImageCopy(OgreSurface* target, Ogre::Image* image)
{   
   Ogre::PixelFormat pixelFormat = image->getFormat();
   Ogre::PixelBox pixelBox = image->getPixelBox();

   Uint8* pSrc = static_cast<Uint8*>(pixelBox.data);
   SDLSurface* sdlSurface = static_cast<SDLSurface*>(target);
 
   int width = target->getWidth();
   int height = target->getHeight();

   assert(width == static_cast<int>(image->getWidth()));
   assert(height == static_cast<int>(image->getHeight()));

   /* Define surface's bytes per pixel */
   int bpp = Ogre::PixelUtil::getNumElemBytes(pixelBox.format);

   /* Define the 'jump' after each valid line */
   int lineJump = pixelBox.getRowSkip();
   Uint8 sr, sg, sb, sa;

   for(int y = 0; y < height; y++)
   {
      for(int x = 0; x < width; x++)
      {
         /* Get pixel from source */
         getPixel(pSrc, pixelFormat, sr, sg, sb, sa);

         /* Set it on dest */
         SDLDraw::setPixel(sdlSurface, x, y, sr, sg, sb, sa);

         /* Go to next source pixel position */
         pSrc += bpp;
      }
      /* Jump to next line */
      pSrc += lineJump;
   }
}
#endif

/******************************************************************
 *                            getPixel                            *
 ******************************************************************/

#if OGRE_VERSION_MAJOR == 2 && OGRE_VERSION_MINOR >= 2
void OgreDraw::getPixel(Uint8* pixel, Ogre::PixelFormatGpu pixelFormat,
      Uint8& red, Uint8& green, Uint8& blue, Uint8& alpha)
{
   if((pixelFormat == Ogre::PFG_RGBA8_UNORM_SRGB) ||
      (pixelFormat == Ogre::PFG_RGBA8_UNORM))
   {
#if OGRE_ENDIAN == OGRE_ENDIAN_LITTLE
      red = pixel[2];
      green = pixel[1];
      blue = pixel[0];
      alpha = pixel[3];
#else
      red = pixel[1];
      green = pixel[2];
      blue = pixel[3];
      alpha = pixel[0];
#endif
   }
   else
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "Error: Unsupported getPixel format '%d'", pixelFormat);
   }
}
#else
void OgreDraw::getPixel(Uint8* pixel, Ogre::PixelFormat pixelFormat,
      Uint8& red, Uint8& green, Uint8& blue, Uint8& alpha)
{
   if(pixelFormat == Ogre::PF_R8G8B8A8)
   {
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
      red = pixel[0];
      green = pixel[1];
      blue = pixel[2];
      alpha = pixel[3];
#else
      red = pixel[3];
      green = pixel[2];
      blue = pixel[1];
      alpha = pixel[0];
#endif
   }
   else if(pixelFormat == Ogre::PF_R8G8B8)
   {
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
      red = pixel[0];
      green = pixel[1];
      blue = pixel[2];
      alpha = 255;
#else
      red = pixel[2];
      green = pixel[1];
      blue = pixel[0];
      alpha = 255;
#endif
   }
   else if(pixelFormat == Ogre::PF_B8G8R8A8)
   {
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
      blue = pixel[0];
      green = pixel[1];
      red = pixel[2];
      alpha = pixel[3];
#else
      blue = pixel[3];
      green = pixel[2];
      red = pixel[1];
      alpha = pixel[0];
#endif
   }
   else if(pixelFormat == Ogre::PF_A8R8G8B8)
   {
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
      alpha = pixel[0];
      red = pixel[1];
      green = pixel[2];
      blue = pixel[3];
#else
   #if KOBOLD_PLATFORM == KOBOLD_PLATFORM_IOS
      //XXX on iOS, ogre3d mixed blue with red channels.
      alpha = pixel[3];
      red = pixel[0];
      green = pixel[1];
      blue = pixel[2];
   #else
      alpha = pixel[3];
      red = pixel[2];
      green = pixel[1];
      blue = pixel[0];
   #endif
#endif
   }
   else if(pixelFormat == Ogre::PF_A8B8G8R8)
   {
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
      alpha = pixel[0];
      blue = pixel[1];
      green = pixel[2];
      red = pixel[3];
#else
   #if KOBOLD_PLATFORM == KOBOLD_PLATFORM_IOS
      //XXX on iOS, ogre3d mixed blue with red channels.
      alpha = pixel[3];
      blue = pixel[0];
      green = pixel[1];
      red = pixel[2];
   #else
      alpha = pixel[3];
      blue = pixel[2];
      green = pixel[1];
      red = pixel[0];
   #endif
#endif
   }
   else
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "Error: Unsupported getPixel format '%d'", pixelFormat);
   }
}
#endif

