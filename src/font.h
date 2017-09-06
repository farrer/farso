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

#ifndef _farso_font_h
#define _farso_font_h

#include <kobold/kstring.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

#include "farsoconfig.h"
#include "rect.h"
#include "surface.h"
#include "colors.h"

namespace Farso
{

#define FONT_GLYPH_CACHE_SIZE   128
/*! Delta for min horizontal distance to keep from write area X axys border. */
#define FONT_HORIZONTAL_DELTA   2

/*! A single font representation. */
class Font
{
   public:

      /*! Alignment for text render */
      enum Alignment
      {
         TEXT_LEFT,
         TEXT_CENTERED,
         TEXT_RIGHT
         //TODO: TEXT_JUSTIFIED
      };

      /*! Font constructor
       * \param lib freetype2 library used.
       * \param filename font's filename to load. */
      Font(Kobold::String filename, FT_Library* lib);
      /*! Font destructor */
      ~Font();

      /*! Set current text size to use.
       * \param pt text size in points. */
      void setSize(int pt);
 
      /*! \return current text size (in points) used */
      int getSize();

      /*! Set current text alignment to use */
      void setAlignment(Alignment align);

      /*! \return current alignment in use */
      Alignment getAlignment();

      /*! Write a textual UTF8* string on the surface, starting at x,y 
       * position, limiting it to the area defined by the rectangle.
       * \note this function will stop to write when the next character 
       *       to render won't fit either horizontally or vertically the
       *       rectangle area defined.
       * \note the rectangle are MUST be inside the surface. Weird and 
       *       unexpected behaviours (segfaults are the best to expect)
       *       will happen if this rule is not respected.
       * \note the surface must be already locked by the caller.
       * \param surface to write to
       * \param x X coordinate to start the write.
       * \param y Y coordinate to start the write.
       * \param area rectangle area to fit the characters to.
       * \return number of characters written with success. */
      int write(Surface* surface, int x, int y, Rect area, Kobold::String text);
      int write(Surface* surface, Rect area, Kobold::String text, 
            int outline = 0);
      
      int write(Surface* surface, Rect area, Kobold::String text, 
                Color outlineColor, int outline);

      /*! Write, trying to break only on spaces (avoiding breaking inner a 
       * word, when possible. */
      int writeBreakingOnSpaces(Surface* surface, Rect area, 
            Kobold::String text, Color outlineColor, int outline);

      /*! Get the width, in pixels, to write the text with current font
       * at its current size.
       * \param text to get width to write.
       * \param outline if will use an outine border, its width.
       * \return width in pixels. */
      int getWidth(Kobold::String text, int outline=0);

      /*! Get part (or the whole) string that fits an width, breaking
       * the string, if possible, on last space (ie: on words).
       * \param text text to get its part that fits width
       * \param fit will receive part of text that fits
       * \param wontFit will receive part of text that won't fit
       * \param width width where text should be.
       * \return width of 'fit' */
      int getWhileFits(Kobold::String text, Kobold::String& fit,
                       Kobold::String& wontFit, int width);

      /*! Get the needed height, in pixels, to write the text with current
       * font at its current size on an area of defined width.
       * \param areaWidth width of the area where will write the font to
       * \param text text to get needed height
       * \param breakOnSpace if try to break font on space.
       * \return needed height in pixels. */
      int getHeight(int areaWidth, Kobold::String text, 
            bool breakOnSpace);
      
      /*! Get default height for a line at current font size */
      int getDefaultHeight();

   private:

      /*! Class used to cache a glyph */
      class CachedGlyph
      {
         public:
            /*! Constructor */
            CachedGlyph();
            /*! Destructor */
            ~CachedGlyph();

            /*! Load a glyph from slot to the cache structure */
            void load(FT_Bitmap slotBitmap, Uint16 c, int outline,
                  int left, int top, int advanceX);

            Uint16 getChar() { return character; };
            int getAdvanceX() { return advanceX; };
            int getBitmapTop() { return bitmapTop; };
            int getBitmapLeft() { return bitmapLeft; };
            int getOutline() { return outline; };
            FT_Bitmap* getBitmap() { return &bitmap; };

         private:
            /*! Delete current bitmap buffer, if any */
            void clearBuffer();

            FT_Bitmap bitmap;
            int advanceX;
            int bitmapTop;
            int bitmapLeft;
            Uint16 character;
            int outline;
      };

      /*! Class for keeping each face, with glyphs cache. */
      class FaceInfo
      {
         public:
            /*! Constructor */
            FaceInfo(FT_Face* face, int size);
            /*! Destructor */
            ~FaceInfo();
            /*! \return pointer to the respective face */
            FT_Face* getFace() { return face; };
            int getIncY() { return incY; };
            int getFontHeight() { return fontHeight; };
            /*! Get a glyph bitmap, using cache */
            CachedGlyph* getGlyph(Uint16 c, int outline, 
                  FT_Library* freeTypeLib);

         private:
            FT_Face* face;
            CachedGlyph cache[FONT_GLYPH_CACHE_SIZE];
            int incY; /**< Amount to increment to Y coordinate
                           for each line. */
            int fontHeight; /**< Max height that fits all characters */
            int size; /**< Text size of the font for this face. */
      };

      /*! Get (if already exists) or create a new face for the font
       * for desired font size (in points). 
       * \param size desired font size (in points)
       * \return pointer to FaceInfo for the desired font size */
      FaceInfo* getFace(int size);

      /*! Load the font data from file 
       * \return if load was successfull. */
      bool load();

      /*! Gets a unicode value from a UTF-8 encoded string and advance 
       * the string.
       * \note this function is based on SDL_TTF code. */
      Uint16 getchUTF8(const Uint8** src, size_t *srclen);

      int write(Surface* surface, int x, int y, Rect area, const Uint8* utf8,
                int outline);

      /*! Write the text centered or at right.  */
      int centeredOrRightWrite(Surface* surface, int x, int y, Rect area, 
            const Uint8* utf8, int outline);

      /*! Flush glyphs of a line to the surface.
       * \note: must check if fits before call. */
      void flushLine(Surface* surface, int x, int y, Uint16* chars, 
            int lastIndex, int areaWidth, int textWidth, int outline);

      /*! Check if the glyph will fits inside the area at current position */
      bool willGlyphFits(int x, int y, Rect area, CachedGlyph* glyph);

      std::map<int, FaceInfo*> faces; /**< Map of font faces for each size */
      Kobold::String filename; /**< Filename of the font used */
      FT_Byte* data; /**< the font data loaded from file. */
      size_t dataSize; /**< the data font size */
      FT_Library* freeTypeLib; /**< The FreeType context to use */

      int curTextSize;  /**< Current text size to use on write */
      FaceInfo* curFace; /**< Current face (of curTextSize size) to use. */
      Alignment curAlign; /**< Alignment to use */
};

/*! Farso's font manager: manages the creation and load of fonts, and also
 * defined the freetype context to use. */
class FontManager
{
   public:
      /*! Init the font manager system */
      static void init();
      /*! Finish with the font manager system */
      static void finish();

      /*! Retrieve or create a font defined by filename file.
       * \param filename name of the file with the font definition.
       * \return Font pointer relative to this filename. */
      static Font* getFont(Kobold::String filename);

      /*! Set the default font to use.
       * \param filename filename of the font to be default's one.
       * \return pointer to the font (or NULL if something wrong).
       * \note: It's mandatory to call this function when not using a skin. */
      static Font* setDefaultFont(Kobold::String filename);

      /*! \return pointer to the default font to use. */
      static Font* getDefaultFont();
      /*! \return filename of the default font to use. */
      static Kobold::String getDefaultFontFilename();

      /*! Unload all current loaded fonts. Usefull when the system is
       * low in memory to de-alloc some no more used bytes.
       * TODO: make this function tread safe (as unload could be called on
          * another thread if used, for example, on iOS)! */
      static void unloadAllFonts();

   private:
      /*! No instances allowed */
      FontManager(){};

      static FT_Library freeTypeLib; /**< The FreeType context to use */
      static std::map<Kobold::String, Font*> fonts; /**< Current loaded fonts */
      static Kobold::String defaultFont; /**< Default font to use */
};

}


#endif

