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

#include <OGRE/OgreMath.h>
#include <OGRE/OgreLogManager.h>

#include <string.h>

using namespace Farso;

/******************************************************************
 *                           constructor                          *
 ******************************************************************/
OgreDraw::OgreDraw()
         :Draw()
{
}

/******************************************************************
 *                            destructor                          *
 ******************************************************************/
OgreDraw::~OgreDraw()
{
}


/******************************************************************
 *                            setPixel                            *
 ******************************************************************/
void OgreDraw::setPixel(Uint8* pixel, Ogre::PixelFormat pixelFormat,
      Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
   if(pixelFormat == Ogre::PF_R8G8B8A8)
   {
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
      pixel[0] = red;
      pixel[1] = green;
      pixel[2] = blue;
      pixel[3] = alpha;
#else
      pixel[3] = red;
      pixel[2] = green;
      pixel[1] = blue;
      pixel[0] = alpha;
#endif
   } 
   else if(pixelFormat == Ogre::PF_B8G8R8A8)
   {
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
      pixel[0] = blue;
      pixel[1] = green;
      pixel[2] = red;
      pixel[3] = alpha;
#else
      pixel[3] = blue;
      pixel[2] = green;
      pixel[1] = red;
      pixel[0] = alpha;
#endif
   }
   else if(pixelFormat == Ogre::PF_A8R8G8B8)
   {
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
      pixel[0] = alpha;
      pixel[1] = red;
      pixel[2] = green;
      pixel[3] = blue;
#else
      pixel[3] = alpha;
      pixel[2] = red;
      pixel[1] = green;
      pixel[0] = blue;
#endif
   }
   else if(pixelFormat == Ogre::PF_A8B8G8R8)
   {
#if OGRE_ENDIAN == OGRE_ENDIAN_BIG
      pixel[0] = alpha;
      pixel[1] = blue;
      pixel[2] = green;
      pixel[3] = red;
#else
      pixel[3] = alpha;
      pixel[2] = blue;
      pixel[1] = green;
      pixel[0] = red;
#endif
   }
   else
   {
      Ogre::LogManager::getSingleton().getDefaultLog()->stream()
         << "Unsupported setPixel format :" << pixelFormat;
   }
}

/******************************************************************
 *                            setPixel                           *
 ******************************************************************/
void OgreDraw::setPixel(Surface* surface, int x, int y, 
      Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
   OgreSurface* ogreSurface = (OgreSurface*) surface;

   const Ogre::PixelBox pixelBox = ogreSurface->getPixelBox();
   Uint8* pDest = static_cast<Uint8*>(pixelBox.data);
   int bpp = Ogre::PixelUtil::getNumElemBytes(pixelBox.format);

   pDest += y * ((pixelBox.getWidth() * bpp) + pixelBox.getRowSkip()) + x * bpp;
   setPixel(pDest, ogreSurface->getPixelFormat(), red, green, blue, alpha);
}

/******************************************************************
 *                            getPixel                            *
 ******************************************************************/
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
      alpha = pixel[3];
      red = pixel[2];
      green = pixel[1];
      blue = pixel[0];
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
      alpha = pixel[3];
      blue = pixel[2];
      green = pixel[1];
      red = pixel[0];
#endif
   }
   else
   {
      Ogre::LogManager::getSingleton().getDefaultLog()->stream()
         << "Unsupported getPixel format :" << pixelFormat;
   }
}

/******************************************************************
 *                            pixel_Get                           *
 ******************************************************************/
void OgreDraw::getPixel(Surface* surface, int x, int y, 
      Uint8& red, Uint8& green, Uint8& blue, Uint8& alpha)
{
   OgreSurface* ogreSurface = (OgreSurface*) surface;

   const Ogre::PixelBox pixelBox = ogreSurface->getPixelBox();
   Uint8* pDest = static_cast<Uint8*>(pixelBox.data);
   int bpp = Ogre::PixelUtil::getNumElemBytes(pixelBox.format);

   pDest += y * ((pixelBox.getWidth() * bpp) + pixelBox.getRowSkip()) + x * bpp;
   getPixel(pDest, ogreSurface->getPixelFormat(), red, green, blue, alpha);
}

/******************************************************************
 *                         rectangle_Fill                         *
 ******************************************************************/
void OgreDraw::doFilledRectangle(Surface* surface, int x1, int y1, 
      int x2, int y2)
{
   OgreSurface* ogreSurface = (OgreSurface*) surface;

   const Ogre::PixelBox pixelBox = ogreSurface->getPixelBox();
   Ogre::PixelFormat pixelFormat = ogreSurface->getPixelFormat();
   Uint8* pDest = static_cast<Uint8*>(pixelBox.data);
   int pixelsAfterX2 = pixelBox.getWidth() - x2 - 1;
   
   /* Define surface's bytes per pixel */
   int bpp = Ogre::PixelUtil::getNumElemBytes(pixelBox.format);

   /* Go to the desired first line, at first X. */
   pDest += y1 * ((pixelBox.getWidth() * bpp) + pixelBox.getRowSkip()) + 
      x1 * bpp;

   for(int y = y1; y <= y2; y++)
   {
      for(int x = x1; x <= x2; x++)
      {
         /* Set the pixel */
         setPixel(pDest, pixelFormat, curColor.red, curColor.green,
               curColor.blue, curColor.alpha);

         pDest += bpp;
      }
      /* Positionate on next line init */
      pDest += pixelBox.getRowSkip() + (pixelsAfterX2 + x1) * bpp;
   }
}

/******************************************************************
 *                         doStampFill                            *
 ******************************************************************/
void OgreDraw::doStampFill(Surface* target, int tx1, int ty1, int tx2, int ty2,
      Surface* source, int sx1, int sy1, int sx2, int sy2,
      StampFillType stampType)
{
   OgreSurface* ogreSource = (OgreSurface*) source;
   OgreSurface* ogreTarget = (OgreSurface*) target;

   /* Check and adjust rectangle's definition */
   if(tx2 < tx1)
   {
      int tmp = tx1;
      tx1 = tx2;
      tx2 = tmp;
   }
   if(ty2 < ty1)
   {
      int tmp = ty1;
      ty1 = ty2;
      ty2 = tmp;
   }
   if(sx2 < sx1)
   {
      int tmp = sx1;
      sx1 = sx2;
      sx2 = tmp;
   }
   if(sy2 < sy1)
   {
      int tmp = sy1;
      sy1 = sy2;
      sy2 = tmp;
   }

   /* Check if images aren't the same (they couldn't be, as we do 
    * memcpy on them and the behaviour will be unexpected). */
   assert( (target != source) && 
           (target->getTextureName() != source->getTextureName()) );

   /* Check pixel formats compatibility. */
   if(ogreSource->getPixelFormat() != ogreTarget->getPixelFormat())
   {
      Ogre::LogManager::getSingleton().getDefaultLog()->stream()
         << "ERROR: Couldn't doStampFill with different PixelFormats: '"
         << ogreSource->getPixelFormat() << "' and '"
         << ogreTarget->getPixelFormat() << "'";
      return;
   }
   Ogre::PixelFormat pixelFormat = ogreSource->getPixelFormat();

   /* Get pixel box and data pointers. */
   const Ogre::PixelBox sourceBox = ogreSource->getPixelBox();
   const Ogre::PixelBox targetBox = ogreTarget->getPixelBox();

   Uint8* pDest = static_cast<Uint8*>(targetBox.data);
   Uint8* pSrc = static_cast<Uint8*>(sourceBox.data);

   int sdX = sx2 - sx1 + 1;
   int sdY = sy2 - sy1 + 1;
   int tdX = tx2 - tx1 + 1;
   int tdY = ty2 - ty1 + 1;

   /* Check and adjust dimensions (will make easy the memcpy) */
   if(sdX > tdX)
   {
      sx2 = sx1 + tdX - 1;
      sdX = tdX;
   }
   if(sdY > tdY)
   {
      sy2 = sy1 + tdY - 1;
      sdY = tdY;
   }
   
   assert((sdX <= (int)sourceBox.getWidth()) && 
          (sdY <= (int)sourceBox.getHeight()));
   assert((tdX <= (int)targetBox.getWidth()) && 
          (tdY <= (int)targetBox.getHeight()));

   /* Define surface's bytes per pixel */
   int bpp = Ogre::PixelUtil::getNumElemBytes(sourceBox.format);

   /* Define the 'jump' after each valid line */
   int sourceLineJump = ((sourceBox.getWidth())) * bpp +
      sourceBox.getRowSkip();
   int targetLineJump = ((targetBox.getWidth() - tx2 - 1) + tx1) * bpp +
      targetBox.getRowSkip();

   /* Define the 'jump' to each rectangle init. */
   int incToSourceInit = sy1 * ((sourceBox.getWidth() * bpp) + 
         sourceBox.getRowSkip()) + sx1 * bpp;
   int incToTargetInit = ty1 * ((targetBox.getWidth() * bpp) + 
         targetBox.getRowSkip()) + tx1 * bpp;

   /* The loop will be through target rectangle. Let's get the pointer
    * to the first source position (as we'll need to repositionate the
    * source pointer at init when hit source rectangle's end). */
   pSrc += incToSourceInit;
   Uint8* pSrcInit = pSrc;
   int curSrcLine = sy1;

   /* Positionate the target pointer and iterate. */
   pDest += incToTargetInit;
   
   for(int y = ty1; y <= ty2; y++)
   {

      Uint8* pSrcLineInit = pSrc;
      /* At TYPE_COPY we just copy the whole line
       * from source to target, until fill the desired width. */
      if(stampType == STAMP_TYPE_COPY)
      {
         /* Let's memcpy the source to target. */
         int remainX = tdX;
         while(remainX > 0)
         {
            if(remainX >= sdX)
            {
               /* copy the full line from source */
               memcpy(pDest, pSrc, sdX * bpp);

               /* Go next position on pDest */
               pDest += sdX * bpp;
               remainX -= sdX;
            }
            else
            {
               /* Copy just the remaining pixels. */
               memcpy(pDest, pSrc, remainX * bpp);

               /* Go after on pDest */
               pDest += remainX * bpp;
               remainX = 0;
            }

            /* Repositionate source at its line init */
            pSrc = pSrcLineInit;
         }
      }
      /* At TYPE_BLEND we must use alphas from source and target to calculate
       * new target pixel value. */
      else if(stampType == STAMP_TYPE_BLEND)
      {
         Uint8 sr, sg, sb, sa;
         Uint8 tr, tg, tb, ta;
         int curSrcX = sx1;
         for(int x = tx1; x <= tx2; x++)
         {
            /* Get pixels */
            getPixel(pSrc, pixelFormat, sr, sg, sb, sa);
            getPixel(pDest, pixelFormat, tr, tg, tb, ta);

            /* Blend them and set on target */
            blendColor(sr, sg, sb, sa, tr, tg, tb, ta);
            setPixel(pDest, pixelFormat, tr, tg, tb, ta);  

            curSrcX++;
            if(curSrcX > sx2)
            {
               pSrc = pSrcLineInit;
               curSrcX = sx1;
            }
            else
            {
               pSrc += bpp;
            }
            pDest += bpp;
         }

         /* Repositionate source at its line init */
         pSrc = pSrcLineInit;
      }

      
      /* Positionate target on next line init */
      pDest += targetLineJump;

      /* Positionate source on next line init */
      curSrcLine++;
      if(curSrcLine > sy2)
      {
         /* Must repositionate at source's init */
         pSrc = pSrcInit;
         curSrcLine = sy1;
      }
      else
      {
         pSrc += sourceLineJump;
      }
   }

}

/******************************************************************
 *                         doStampFill                            *
 ******************************************************************/
void OgreDraw::doFreeTypeStamp(Surface* target, int x, int y, FT_Bitmap* bitmap,
                           int left, int top)
{
   OgreSurface* ogreTarget = (OgreSurface*) target;

   /* Get target pixel box and its data */
   const Ogre::PixelBox targetBox = ogreTarget->getPixelBox();
   Uint8* pDest = static_cast<Uint8*>(targetBox.data);
   Uint8* pSrc = bitmap->buffer;

   /* Make sure the full glyph fits */
   assert((x + left + bitmap->width) < targetBox.getWidth());
   assert((y - top + bitmap->rows) < targetBox.getHeight());
   assert((x + left) >= 0);
   assert((y - top) >= 0);

   /* Define surface's bytes per pixel */
   int bpp = Ogre::PixelUtil::getNumElemBytes(targetBox.format);
   Ogre::PixelFormat pixelFormat = ogreTarget->getPixelFormat();

   /* Define glyph bytes per pixel */
   int glyphBpp = 1;
   switch(bitmap->pixel_mode)
   {
      case FT_PIXEL_MODE_GRAY:
         glyphBpp = 1;
      break;
      default:
         Ogre::LogManager::getSingleton().getDefaultLog()->stream()
            << "WARN: unsupported glyph bitmap format: " 
            << bitmap->pixel_mode;
         return;
      break;
   }

   /* Define the 'jump' after each valid line */
   int glyphLineJump = (Ogre::Math::Abs(bitmap->pitch) - 
         (bitmap->width * glyphBpp));
   int targetLineJump = ((targetBox.getWidth() - bitmap->width)) * bpp +
         targetBox.getRowSkip();

   /* Define the 'jump' to initial target pos. */
   int incToTargetInit = (y - top) * ((targetBox.getWidth() * bpp) + 
         targetBox.getRowSkip()) + (x + left) * bpp;
 
   /* Positionate target at the desired pixel, and glyph at top left */
   pDest += incToTargetInit;

   for(unsigned int v = 0; v < bitmap->rows; v++)
   {
      for(unsigned int u = 0; u < bitmap->width; u++)
      {
         if(glyphBpp == 1)
         {
            Uint8 tr, tg, tb, ta;
            
            /* Get pixel from target to blend it with glyph */
            getPixel(pDest, pixelFormat, tr, tg, tb, ta);
            blendColor(curColor.red, curColor.green, curColor.blue, pSrc[0], 
                  tr, tg, tb, ta);

            /* Define it at target */
            setPixel(pDest, pixelFormat, tr, tg, tb, ta);
         }

         /* Advance to next pixel */
         if(bitmap->pitch > 0)
         {
            /* Pitch signal will define if will upward or backward on it */
            pSrc += glyphBpp;
         }
         else
         {
            pSrc -= glyphBpp;
         }
         pDest += bpp;
      }

      pDest += targetLineJump;
      if(bitmap->pitch > 0)
      {
         pSrc += glyphLineJump;
      }
      else
      {
         pSrc -= glyphLineJump;
      }
   }

}

