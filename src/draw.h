/* 
  Farso: a simple GUI.
  Copyright (C) DNTeam <dnt@dnteam.org>
 
  This file is part of Farso.
 
  Farso is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Farso is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Farso.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _farso_draw_h
#define _farso_draw_h

#include <kobold/kstring.h>

#include "farsoconfig.h"
#include "surface.h"
#include "colors.h"

#include <ft2build.h>
#include FT_IMAGE_H


namespace Farso
{

/*! Draw class is the implementation of primitive draw functions to draw on
 * a Surface.
 * \note: Before calling any o these functions passing one or more surfaces,
 *       those must be locked (and unlocked after all needed calls). */
class Draw
{
   public:
    
      enum StampFillType
      { 
         /*! Just copy the source pixels into target, overhiding. */
         STAMP_TYPE_COPY = 0,
         /*! Blend source pixels with target pixels. */
         STAMP_TYPE_BLEND
      };

      /*!Constructor */
      Draw();

      /*! Destructor */
      virtual ~Draw();

      /*! Define active Color
       * \param Ri -> Red, from 0 to 255
       * \param Gi -> Green, from 0 to 255
       * \param Bi -> Blue, from 0 to 255 
       * \param Ai -> Alpha, from 0 (transparent) to 255 (fully opaque) */
      void setActiveColor(Uint8 Ri, Uint8 Gi, Uint8 Bi, Uint8 Ai);
      void setActiveColor(Color color);

      /*! Get the Active Color
       * \param Ri -> Red
       * \param Gi -> Green
       * \param Bi -> Blue
       * \param Ai -> Alpha (0 -> transparent; 255 -> fully opaque) */
      void getActiveColor(Uint8& Ri, Uint8& Gi, Uint8& Bi, Uint8& Ai);
      Color getActiveColor();

      /*! Set the surface (x,y) pixel with current active color.
       * \param surface -> bitmap to draw
       * \param x -> x coordinate of the pixel
       * \param y -> y coordinate of the pixel */
      void setPixel(Surface* surface, int x, int y);

      /*! Get the surface (x,y) pixel color
       * \param surface -> bitmap to draw
       * \param x -> x coordinate of the pixel
       * \param y -> y coordinate of the pixel
       * \param red -> pixel red component return  
       * \param green -> pixel green component return  
       * \param blue -> pixel blue component return  
       * \param alpha -> pixel alpha component return */ 
      virtual void getPixel(Surface* surface, int x, int y, 
            Uint8& red, Uint8& green, Uint8& blue, Uint8& alpha) = 0;

      /*! Draw a line on the surface
       * \param surface -> bitmap to draw to
       * \param x1 -> x initial coordinate
       * \param y1 -> y initial coordinate
       * \param x2 -> x final coordinate
       * \param y2 -> y final coordinate */
      void doLine(Surface* surface, int x1, int y1, int x2, int y2);

      /*! Draw an antialiased line on the surface.  */
      void doAntiAliasedLine(Surface* surface, int x1, int y1, int x2, int y2);

      /*! Draw a rectangle on the surface
       * \param surface -> bitmap to draw to
       * \param x1 -> x initial coordinate
       * \param y1 -> y initial coordinate
       * \param x2 -> x final coordinate
       * \param y2 -> y final coordinate */
      void doRectangle(Surface* surface, int x1, int y1, int x2, int y2);

      /*! Draw and Fill a rectangle on surface
       * \param surface -> bitmap to draw to
       * \param x1 -> x initial coordinate
       * \param y1 -> y initial coordinate
       * \param x2 -> x final coordinate
       * \param y2 -> y final coordinate */
      virtual void doFilledRectangle(Surface* surface, int x1, int y1, 
            int x2, int y2) = 0;

      /*! Draw a rounded edge rectangle, with two colors (usefull for buttons)
       * \param surface -> bitmap to draw to
       * \param x1 -> x initial coordinate
       * \param y1 -> y initial coordinate
       * \param x2 -> x final coordinate
       * \param y2 -> y final coordinate
       * \param Ri -> second Red Color
       * \param Gi -> second Green Color
       * \param Bi -> second Blue Color
       * \param Ai -> second Alpha Color */
      void doRoundedRectangle(Surface* surface, int x1, int y1, int x2, int y2, 
            Uint8 Ri, Uint8 Gi, Uint8 Bi, Uint8 Ai);
      void doRoundedRectangle(Surface* surface, int x1, int y1, 
            int x2, int y2, Color secondColor);

      /*! Draw a rectangle with two colors (usefull for buttons)  
       * \param surface -> bitmap to draw to
       * \param x1 -> x initial coordinate
       * \param y1 -> y initial coordinate
       * \param x2 -> x final coordinate
       * \param y2 -> y final coordinate
       * \param Ri -> second Red Color
       * \param Gi -> second Green Color
       * \param Bi -> second Blue Color 
       * \param Ai -> second Alpha Color */
      void doTwoColorsRectangle(Surface* surface, int x1, int y1, 
            int x2, int y2, Uint8 Ri, Uint8 Gi, Uint8 Bi, Uint8 Ai);
      void doTwoColorsRectangle(Surface* surface, int x1, int y1, 
            int x2, int y2, Color secondColor);

      /*! Draw a triangle with edges (x1,y1) (x2, y2) and (x3, y3) 
       * to a surface. */
      void doTriangle(Surface* surface, int x1, int y1, int x2, int y2, 
            int x3, int y3);

      /*! Draw a filled triangle with edges (x1,y1) (x2, y2) and (x3, y3) 
       * to a surface. */
      void doFilledTriangle(Surface* surface, int x1, int y1, int x2, int y2, 
            int x3, int y3);

      /*! Draw a Circle to the surface
       * \param surface -> surface where draw the cricle
       * \param xC -> circle X center coordinate
       * \param yC -> circle Y center coordinate
       * \param r  -> circle radius */
      void doCircle(Surface* surface, int xC, int yC, int r);

      /*! Stamp the source surface rectangle (sx1, sy1, sx2, sy2) at 
       * target's rectangle(tx1, ty1, tx2, ty2), repeating the source 
       * rectangle if necessary (ie: source rectangle < target rectangle).
       * \note The pixel formats of source and target must be the same.
       * \param target surface where will stamp the source to.
       * \param tx1 left coordinate on target 
       * \param ty1 top coordinate on target 
       * \param tx2 right coordinate on target 
       * \param ty2 bottom coordinate on target
       * \param source surface to stamp into target. 
       * \param sx1 left coordinate on source 
       * \param sy1 top coordinate on source 
       * \param sx2 right coordinate on source 
       * \param sy2 bottom coordinate on source */
      virtual void doStampFill(Surface* target, int tx1, int ty1,
            int tx2, int ty2, Surface* source, int sx1, int sy1,
            int sx2, int sy2, StampFillType stampType = STAMP_TYPE_BLEND) = 0;

      /*! Stamp the bitmap defined for a FreeType's glyph on a surface, at
       * x,y position, using the current active color.
       * \param target surface where will put the glyph into.
       * \param x left coordinate on target.
       * \param y base-line Y coordinate on target.
       * \param bitmap FreeType with glyph's bitmap to stamp
       * \param left where left starts at the bitmap
       * \param top where top starts at the bitmap */
      virtual void doFreeTypeStamp(Surface* target, int x, int y, 
            FT_Bitmap* bitmap, int left, int top) = 0;

      /*! Return the smallest power of two greater or equal to the number
       * \param num -> bases number 
       * \return -> smallest power of two greater or equal to the number */
      int smallestPowerOfTwo(int num);

   protected:
      /*! Set the surface (x,y) pixel color.
       * \param surface -> bitmap to draw
       * \param x -> x coordinate of the pixel
       * \param y -> y coordinate of the pixel
       * \param red -> red  value
       * \param green -> green value
       * \param blue -> blue value
       * \param alpha -> alpha value */ 
      virtual void setPixel(Surface* surface, int x, int y, 
            Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) = 0;

      /*! Set pixel, with bright component, for antialiased lines */
      void setPixel(Surface* surface, int x, int y, float bright);

      /*! Blend two colors, and saving the blend on the second one */
      void blendColor(Uint8 sr, Uint8 sg, Uint8 sb, Uint8 sa, 
                      Uint8& tr, Uint8& tg, Uint8& tb, Uint8& ta);
   
   protected:
      
      Color curColor;  /**< Active color. */

};

}

#endif

