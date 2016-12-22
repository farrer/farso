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

#include "sdldraw.h"
#include "sdlsurface.h"
#include <kobold/log.h>
#include <stdlib.h>
using namespace Farso;

/************************************************************************
 *                               SDLDraw                             *
 ************************************************************************/
SDLDraw::SDLDraw()
{
}

/************************************************************************
 *                              ~SDLDraw                             *
 ************************************************************************/
SDLDraw::~SDLDraw()
{
}

/************************************************************************
 *                                setPixel                              *
 ************************************************************************/
void SDLDraw::setPixel(Surface* surface, int x, int y, 
      Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
   SDL_Surface* sdlSurf = ((SDLSurface*) surface)->getSurface();
   int bpp = sdlSurf->format->BytesPerPixel;

   /* Verify Limits */
   if((x > sdlSurf->w - 1) || (y > sdlSurf->h - 1) || (x < 0) || (y < 0))
   {
      return;
   }

   Uint32 color;

   /* Map RGB(A) color to the SDL Color */
   if(bpp == 4)
   {
      color = SDL_MapRGBA(sdlSurf->format, red, green, blue, alpha);
   }
   else
   {
      color = SDL_MapRGB(sdlSurf->format, red, green, blue);
   }

   /* Here p is the address to the pixel we want to set */
   Uint8 *p = (Uint8 *)sdlSurf->pixels + y * sdlSurf->pitch + x * bpp;

   switch(bpp)
   {
      case 1:
         *p = color;
      break;

      case 2:
         *(Uint16 *)p = color;
      break;

      case 3:
      {
         if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
         {
            p[0] = (color >> 16) & 0xff;
            p[1] = (color >> 8) & 0xff;
            p[2] = color & 0xff;
         }
         else
         {
            p[0] = color & 0xff;
            p[1] = (color >> 8) & 0xff;
            p[2] = (color >> 16) & 0xff;
         }
      }
      break;

      case 4:
         *(Uint32 *)p = color;
      break;
   }
}

/************************************************************************
 *                                getPixel                              *
 ************************************************************************/
void SDLDraw::getPixel(Surface* surface, int x, int y, 
      Uint8& red, Uint8& green, Uint8& blue, Uint8& alpha)
{
   SDL_Surface* sdlSurf = ((SDLSurface*) surface)->getSurface();

   /* Verify Limits */
   if((x > sdlSurf->w - 1) || (y > sdlSurf->h - 1) || (x < 0) || (y < 0))
   {
      return;
   }

   int bpp = sdlSurf->format->BytesPerPixel;

   Uint32 color = 0;
   
   /* Here p is the address to the pixel we want to retrieve */
   Uint8 *p = (Uint8 *)sdlSurf->pixels + y * sdlSurf->pitch + x * bpp;

   switch(bpp)
   {
      case 1:
         color = *p;
      break;

      case 2:
         color = *(Uint16 *)p;
      break;

      case 3:
         if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            color =  (p[0] << 16 | p[1] << 8 | p[2]);
         else
            color = (p[0] | p[1] << 8 | p[2] << 16);
      break;

      case 4:
         color = *(Uint32 *)p;
      break;

      default:
         color = 0;       /* shouldn't happen, but avoids warnings */
      break;
   }

   /* Convert Uint32 color to RGBA */
   SDL_GetRGBA(color, sdlSurf->format, &red, &green, &blue, &alpha);
}

/************************************************************************
 *                          doFilledRectangle                           *
 ************************************************************************/
void SDLDraw::doFilledRectangle(Surface* surface, int x1, int y1, 
      int x2, int y2)
{
   /* Retrieve SDL surface */
   SDL_Surface* sdlSurf = static_cast<SDLSurface*>(surface)->getSurface();
   
   /* Define fill rectangle */
   SDL_Rect ret;
   ret.x = x1;
   ret.y = y1;
   ret.w = (x2 - x1) + 1;
   ret.h = (y2 - y1) + 1;

   int bpp = sdlSurf->format->BytesPerPixel;
   Uint32 color;

   if(bpp == 4)
   {
      color = SDL_MapRGBA(sdlSurf->format, curColor.red, curColor.green,
            curColor.blue, curColor.alpha);
   }
   else
   { 
      color = SDL_MapRGB(sdlSurf->format, curColor.red, curColor.green,
            curColor.blue);
   }

   SDL_FillRect(sdlSurf, &ret, color);
}

/************************************************************************
 *                              doStampFill                             *
 ************************************************************************/
void SDLDraw::doStampFill(Surface* target, int tx1, int ty1,
      int tx2, int ty2, Surface* source, int sx1, int sy1,
      int sx2, int sy2, StampFillType stampType)
{
   /* Retrieve SDL surfaces from our surfaces */
   SDL_Surface* targetSdl = ((SDLSurface*) target)->getSurface();
   SDL_Surface* sourceSdl = ((SDLSurface*) source)->getSurface();

   int sourceWidth = (sx2 - sx1) + 1;
   int sourceHeight = (sy2 - sy1) + 1;

   /* Define Blit rectangles */
   SDL_Rect targetRect;
   targetRect.x = tx1;
   targetRect.y = ty1;
   
   SDL_Rect sourceRect;
   sourceRect.x = sx1;
   sourceRect.y = sy1;
   sourceRect.w = sourceWidth;
   sourceRect.h = sourceHeight;

   /* Check each stamp type */
   if(stampType == Draw::STAMP_TYPE_BLEND) 
   {
      SDL_SetSurfaceBlendMode(sourceSdl, SDL_BLENDMODE_BLEND);
   }
   else if(stampType == Draw::STAMP_TYPE_COPY)
   {
      SDL_SetSurfaceBlendMode(sourceSdl, SDL_BLENDMODE_NONE);
   }

   /* Blit until filled the whole target rectangle */
   bool moreToBlit = true;
   while(moreToBlit)
   {
      /* Let's check if we need to redefine source width and height to keep
       * the blit at the defined area (and not blit a bit more due to source's
       * defined size). */
      if(targetRect.x + sourceWidth > tx2)
      {
         sourceRect.w = tx2 - targetRect.x + 1;
      }
      if(targetRect.y + sourceHeight > ty2)
      {
         sourceRect.h = ty2 - targetRect.y + 1;
      } 

      /* Do the actual blit */
      if(SDL_BlitSurface(sourceSdl, &sourceRect, targetSdl, &targetRect) < 0)
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, SDL_GetError());
      }

      /* Let's go ahead and see if still at targetRect */
      targetRect.x += sourceWidth;
      if(targetRect.x > tx2)
      {
         /* Must go to next line */
         targetRect.x = tx1;
         targetRect.y += sourceHeight;
      }

      /* And redefine the source's width and height that could be changed */
      sourceRect.w = sourceWidth;
      sourceRect.h = sourceHeight;

      /* Check if still need to blit */
      moreToBlit = (targetRect.y <= ty2);
   }

}

/************************************************************************
 *                          doFreeTypeStamp                             *
 ************************************************************************/
void SDLDraw::doFreeTypeStamp(Surface* target, int x, int y, 
      FT_Bitmap* bitmap, int left, int top)
{
   /* Define glyph bytes per pixel */
   int glyphBpp = 1;
   switch(bitmap->pixel_mode)
   {
      case FT_PIXEL_MODE_GRAY:
         glyphBpp = 1;
      break;
      default:
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL, 
               "WARN: unsupported glyph bitmap format: %d", bitmap->pixel_mode);
         return;
      break;
   }
   
   Uint8* pSrc = bitmap->buffer;

   /* Define the 'jump' after each valid line */
   int glyphLineJump = (int) (abs(bitmap->pitch) - 
         (bitmap->width * glyphBpp));
   int tx;
   int ty = y - top;
 
   for(unsigned int v = 0; v < bitmap->rows; v++)
   {
      tx = x + left;
      for(unsigned int u = 0; u < bitmap->width; u++)
      {
         if(glyphBpp == 1)
         {
            Uint8 tr, tg, tb, ta;
            
            /* Get pixel from target to blend it with glyph */
            getPixel(target, tx, ty, tr, tg, tb, ta);
            blendColor(curColor.red, curColor.green, curColor.blue, pSrc[0], 
                  tr, tg, tb, ta);

            /* Define it at target */
            setPixel(target, tx, ty, tr, tg, tb, ta);
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
         tx++;
      }
      
      ty++;
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


