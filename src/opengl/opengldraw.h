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

#ifndef _farso_opengl_draw_h
#define _farso_opengl_draw_h

#include "../draw.h"

namespace Farso
{

/*! Draw implementation for SDL/OpenGL */
class OpenGLDraw : public Draw
{
   public:
      /*! Constructor */
      OpenGLDraw();
      /*! Destructor */
      ~OpenGLDraw();

      /*! Get the surface (x,y) pixel color
       * \param surface -> bitmap to draw
       * \param x -> x coordinate of the pixel
       * \param y -> y coordinate of the pixel
       * \param red -> pixel red component return  
       * \param green -> pixel green component return  
       * \param blue -> pixel blue component return  
       * \param alpha -> pixel alpha component return */ 
      void getPixel(Surface* surface, int x, int y, 
            Uint8& red, Uint8& green, Uint8& blue, Uint8& alpha);

      /*! Draw and Fill a rectangle on surface
       * \param surface -> bitmap to draw to
       * \param x1 -> x initial coordinate
       * \param y1 -> y initial coordinate
       * \param x2 -> x final coordinate
       * \param y2 -> y final coordinate */
      void doFilledRectangle(Surface* surface, int x1, int y1, 
            int x2, int y2);

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
      void doStampFill(Surface* target, int tx1, int ty1,
            int tx2, int ty2, Surface* source, int sx1, int sy1,
            int sx2, int sy2, StampFillType stampType = STAMP_TYPE_BLEND);

      /*! Stamp the bitmap defined for a FreeType's glyph on a surface, at
       * x,y position, using the current active color.
       * \param target surface where will put the glyph into.
       * \param x left coordinate on target.
       * \param y base-line Y coordinate on target.
       * \param bitmap FreeType with glyph's bitmap to stamp
       * \param left where left starts at the bitmap
       * \param top where top starts at the bitmap */
      void doFreeTypeStamp(Surface* target, int x, int y, 
            FT_Bitmap* bitmap, int left, int top);

   protected:

      /*! Set the surface (x,y) pixel color.
       * \param surface -> bitmap to draw
       * \param x -> x coordinate of the pixel
       * \param y -> y coordinate of the pixel
       * \param red -> red  value
       * \param green -> green value
       * \param blue -> blue value
       * \param alpha -> alpha value */ 
      void setPixel(Surface* surface, int x, int y, 
            Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
      
};

}

#endif

