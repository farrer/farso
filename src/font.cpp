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

#include "font.h"
#include "controller.h"
#include <math.h>
#include <kobold/log.h>

#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID && \
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS
    #include "cursor.h"
#endif

#include <fstream>
#include <sys/stat.h>

#include FT_STROKER_H

#include <assert.h>
using namespace Farso;

#define UNICODE_SPACE_CHARACTER   0x20

/***********************************************************************
 *                                 init                                *
 ***********************************************************************/
void FontManager::init()
{
   FontManager::defaultFont = "";
   int error = FT_Init_FreeType(&freeTypeLib);
   if(error)
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "ERROR: Couldn't init freetype2 context! Error code: %d", error);
   }
}

/***********************************************************************
 *                                finish                               *
 ***********************************************************************/
void FontManager::finish()
{
   defaultFont = "";
   unloadAllFonts();
   FT_Done_FreeType(freeTypeLib);
}

/***********************************************************************
 *                          setDefaultFont                             *
 ***********************************************************************/
Font* FontManager::setDefaultFont(const Kobold::String& filename)
{
   assert(filename != "");

   Font* f = getFont(filename);
   if(f != NULL)
   {
      defaultFont = filename;
   }

   return f;
}

/***********************************************************************
 *                          getDefaultFont                             *
 ***********************************************************************/
Font* FontManager::getDefaultFont()
{
   assert(defaultFont != "");
   return getFont(defaultFont);
}
const Kobold::String& FontManager::getDefaultFontFilename()
{
   return defaultFont;
}

/***********************************************************************
 *                              getFont                                *
 ***********************************************************************/
Font* FontManager::getFont(const Kobold::String& filename)
{
   mutex.lock();
   std::map<Kobold::String, Font*>::iterator it = fonts.find(filename);

   if(it == fonts.end())
   {
      /* Font not yet loaded, let's create it */
      Font* f = new Font(filename, &freeTypeLib);
      if(f->load()) 
      {
         fonts[filename] = f;
         mutex.unlock();
         return f;
      }
      else 
      {
         /* Couldn't load font */
         delete f;
         mutex.unlock();
         return NULL;
      }
   }
 
   Font* f = it->second;
   mutex.unlock();
   return f;
}

/***********************************************************************
 *                            unloadAllFonts                           *
 ***********************************************************************/
void FontManager::unloadAllFonts()
{
   mutex.lock();
   for(std::map<Kobold::String, Font*>::iterator it = fonts.begin(); 
         it != fonts.end(); ++it)
   {
      delete it->second;
   }
   fonts.clear();
   mutex.unlock();
}

/***********************************************************************
 *                               Members                               *
 ***********************************************************************/
FT_Library FontManager::freeTypeLib;
std::map<Kobold::String, Font*> FontManager::fonts;
Kobold::String FontManager::defaultFont;
Kobold::Mutex FontManager::mutex;

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
Font::FaceInfo::FaceInfo(FT_Face* face, int size)
{
   assert(face != NULL);
   this->face = face;
   this->size = size;

   /* Define some variables */
   this->incY = (((*face)->height + (*face)->ascender + (*face)->descender) / 
                  (float)(*face)->units_per_EM) * size;
   this->fontHeight = (*face)->size->metrics.height >> 6;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Font::FaceInfo::~FaceInfo()
{
   if(face != NULL)
   {
      FT_Done_Face((*face));
      delete face;
      face = NULL;
   }
}

/***********************************************************************
 *                               getGlyph                              *
 ***********************************************************************/
Font::CachedGlyph* Font::FaceInfo::getGlyph(Uint16 c, int outline,
      FT_Library* freeTypeLib)
{
   assert(face != NULL);
   int charIndex = FT_Get_Char_Index((*face), c);
   int index = charIndex % FONT_GLYPH_CACHE_SIZE;

   /* Check if the character at the cache index is the same of the 
    * desired one */
   if( (cache[index].getChar() == c) && 
       (cache[index].getOutline() == outline) )
   {
      /* Already in cache, let's just use it. */
      return &cache[index];
   }

   /* Cache miss: must load the character to the cache */

   if(outline == 0)
   {
      /* No outline, notmal character load */
      if(FT_Load_Char((*face), c, FT_LOAD_RENDER) != 0)
      {
         return NULL;
      }
      cache[index].load((*face)->glyph->bitmap, c, outline, 
                        (*face)->glyph->bitmap_left, 
                        (*face)->glyph->bitmap_top,
                        (*face)->glyph->advance.x);
   }
   else
   {
      /* Must load with outline effect */
      if(FT_Load_Glyph((*face), charIndex, FT_LOAD_NO_BITMAP) != 0)
      {
         return NULL;
      }
      FT_Stroker stroker;
      FT_Stroker_New((*freeTypeLib), &stroker);
      FT_Stroker_Set(stroker, (int)(outline * 64), FT_STROKER_LINECAP_ROUND,
             FT_STROKER_LINEJOIN_ROUND, 0);
      
      FT_Glyph glyph;
      if(FT_Get_Glyph((*face)->glyph, &glyph) == 0)
      {
         FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
         FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1 );
         FT_BitmapGlyph glyphBitmap = (FT_BitmapGlyph)glyph;

         cache[index].load(glyphBitmap->bitmap, c, outline, 
                        glyphBitmap->left, glyphBitmap->top,
                        (*face)->glyph->advance.x);

         FT_Done_Glyph(glyph);
      }
      FT_Stroker_Done(stroker);
   }

   /* Return the just loaded glyph on the cache */
   return &cache[index];
}

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
Font::CachedGlyph::CachedGlyph()
{
   bitmap.buffer = NULL;
   advanceX = 0;
   bitmapTop = 0;
   bitmapLeft = 0;
   character = 0;
   outline = 0;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Font::CachedGlyph::~CachedGlyph()
{
   clearBuffer();
}

/***********************************************************************
 *                             clearBuffer                             *
 ***********************************************************************/
void Font::CachedGlyph::clearBuffer()
{
   if(bitmap.buffer != NULL)
   {
      delete[] bitmap.buffer;
      bitmap.buffer = NULL;
   }
}

/***********************************************************************
 *                                load                                 *
 ***********************************************************************/
void Font::CachedGlyph::load(FT_Bitmap slotBitmap, Uint16 c, int outline,
                             int left, int top, int advanceX)
{
   /* Clear any pre-existent buffer, and create a new one, with same
    * content from slot bitmap. */
   clearBuffer();
   size_t size = slotBitmap.pitch * slotBitmap.rows;
   bitmap.buffer = new unsigned char[size];
   memcpy(&bitmap.buffer[0], &slotBitmap.buffer[0], size);

   /* Copy the info that we use from bitmap */
   bitmap.width = slotBitmap.width;
   bitmap.rows = slotBitmap.rows;
   bitmap.pitch = slotBitmap.pitch;
   bitmap.pixel_mode = slotBitmap.pixel_mode;

   /* Define some often used variables */
   this->advanceX = (advanceX >> 6) /*+ outline*/;
   bitmapLeft = left;
   bitmapTop = top;
   this->outline = outline;

   /* Set the glyph cache is now related to this character. */
   character = c;
}


/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
Font::Font(const Kobold::String& filename, FT_Library* lib)
{
   this->filename = filename;
   this->data = NULL;
   this->dataSize = 0;
   this->freeTypeLib = lib;
   this->curTextSize = 0;
   this->curFace = NULL;
   this->curAlign = TEXT_LEFT;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Font::~Font()
{
   /* Free all loaded faces */
   for(std::map<int, FaceInfo*>::iterator it = faces.begin(); 
       it != faces.end(); ++it)
   {
      delete it->second;
   }
   faces.clear();

   /* Free the font data loaded, if defined */
   if(data != NULL)
   {
      delete[] data;
      data = NULL;
   }
}

/***********************************************************************
 *                                 load                                *
 ***********************************************************************/
bool Font::load()
{
   return Controller::getLoader()->loadFont(this);
}

/***********************************************************************
 *                                 load                                *
 ***********************************************************************/
bool Font::load(Kobold::FileReader& fileReader)
{
   /* try to open file */
   if(!fileReader.open(Controller::getRealFilename(filename)))
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "ERROR: Couldn't open font file '%s'", filename.c_str());
      return false;
   }

   /* Get its size */
   dataSize = fileReader.getSize();
   if(dataSize == 0)
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "ERROR: file with size 0: '%s'", filename.c_str());
      fileReader.close();
      return false;
   }

   /* Alloc buffer and load to it */
   data = new FT_Byte[dataSize];
   fileReader.read((char*) &data[0], dataSize);

   /* Done */
   fileReader.close();

   return true;
}

/***********************************************************************
 *                               getFace                               *
 ***********************************************************************/
Font::FaceInfo* Font::getFace(int size)
{
   std::map<int, FaceInfo*>::iterator it = faces.find(size);

   if(it == faces.end())
   {
      /* No Face exists for the desired size yet, must create it */
      FT_Face* face = new FT_Face();
      int error = FT_New_Memory_Face((*freeTypeLib), data, dataSize, 0, face);
      if(error)
      {
         Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            (Kobold::String("ERROR: couldn't create face for '%s'.") + 
            Kobold::String(" Error code: %d. Datasize: %d")).c_str(), 
            filename.c_str(), error, dataSize);
         
         delete face;
         return NULL;
      }
      /* Set the size */
      error = FT_Set_Char_Size(*face, 0, size * 64, 96, 96);
      if(error)
      {
         Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "ERROR: couldn't define face char size for '%s' Size was: %d", 
            filename.c_str(), size);
         
         FT_Done_Face(*face);
         delete face;
         return NULL;
      }
      /* Add to map and done */
      Font::FaceInfo* faceInfo = new Font::FaceInfo(face, size);
      faces[size] = faceInfo;
      return faceInfo;
   }

   /* A face was found: return it. */
   return it->second;
}

/***********************************************************************
 *                              setSize                                *
 ***********************************************************************/
void Font::setSize(int pt)
{
   if((curTextSize != pt) && (pt > 0))
   {
      /* Set the new text size, and retrieve (or load) the respective face. */
      curTextSize = pt;
      curFace = getFace(pt);
      if(curFace == NULL)
      {
         /* Couldn't define the text size, so must revert to 0. */
         curTextSize = 0;
      }
   }
   else if(pt <= 0)
   {
      /* No render. */
      curTextSize = 0;
      curFace = NULL;
   }
}

/***********************************************************************
 *                              getSize                                *
 ***********************************************************************/
const int Font::getSize() const
{
   return curTextSize;
}

/***********************************************************************
 *                           setAlignment                              *
 ***********************************************************************/
void Font::setAlignment(const Font::Alignment& align)
{
   curAlign = align;
}

/***********************************************************************
 *                           getAlignment                              *
 ***********************************************************************/
const Font::Alignment& Font::getAlignment() const
{
   return curAlign;
}

/***********************************************************************
 *                            getHeight                                *
 ***********************************************************************/
int Font::getHeight(int areaWidth, const Kobold::String& text, 
      bool breakOnSpace)
{
   if(curFace == NULL)
   {
      return 0;
   }

   int totalLines = 0;

   if(!breakOnSpace)
   {
      /* Direct calculate by string size */
      int width = getWidth(text);
      totalLines = (int) ceil(width / (float) areaWidth);
   }
   else
   {
      /* Calculate by breaking lines on space. */
      Kobold::String fit;
      Kobold::String wontFit = text;
      bool brokeOnSpace = false;
      while(!wontFit.empty())
      {
         Kobold::String cur = wontFit;
         getWhileFits(cur, fit, wontFit, areaWidth, brokeOnSpace);

         if(fit.empty())
         {
            /* Text will never fits! */
            Kobold::Log::add(Kobold::LOG_LEVEL_NORMAL,
               "WARN: text '%s' will never fits desired area size '%d'",
               text.c_str(), areaWidth);
            break;
         }
         totalLines++;
      }
   }
   
   return (totalLines * curFace->getIncY() + 1);
}

/***********************************************************************
 *                        getDefaultHeight                             *
 ***********************************************************************/
const int Font::getDefaultHeight() const
{
   if(curFace == NULL)
   {
      return 0;
   }
   return curFace->getFontHeight();
}

/***********************************************************************
 *                             getWidth                                *
 ***********************************************************************/
int Font::getWidth(const Kobold::String& text, int outline)
{
   int curSize = 0;
   const Uint8* utf8 = (Uint8*) text.c_str();
   size_t texlen = strlen((char*) utf8);

   while(texlen > 0)
   {
      /* Get the character and its glyph */
      Uint16 c = getchUTF8(&utf8, &texlen);
      Font::CachedGlyph* glyph = curFace->getGlyph(c, outline, freeTypeLib);
      if(glyph == NULL)
      {
         return 0;
      }

      curSize += glyph->getAdvanceX();
   }

   return curSize;
}

/***********************************************************************
 *                          getWhileWidth                              *
 ***********************************************************************/
int Font::getWhileFits(const Kobold::String& text, Kobold::String& fit,
      Kobold::String& wontFit, int width, bool& brokeOnSpace)
{
   int curSize = 0;
   const Uint8* utf8 = (Uint8*) text.c_str();
   size_t fulllen = strlen((char*) utf8);
   size_t texlen = fulllen;
   size_t lastlen;
   size_t lastSpace = Kobold::String::npos;
   brokeOnSpace = false;
   
   fit = "";
   wontFit = "";
   
   while(texlen > 0)
   {
      lastlen = texlen;
      /* Get the character and its glyph */
      Uint16 c = getchUTF8(&utf8, &texlen);

      if(c == UNICODE_SPACE_CHARACTER)
      {
         lastSpace = texlen;
      }

      Font::CachedGlyph* glyph = curFace->getGlyph(c, 0, freeTypeLib);
      if(glyph == NULL)
      {
         return 0;
      }

      curSize += glyph->getAdvanceX();
      if(curSize > width)
      {
         /* Try to break on last space */
         if(lastSpace != Kobold::String::npos)
         {
            lastlen = lastSpace;
            brokeOnSpace = true;
         }

         /* String greater than the maximun available width, get each of its
          * parts (fits up to last character 'eaten', wontFit from that on) */
         fit = text.substr(0, fulllen - lastlen);
         wontFit = text.substr(fulllen - lastlen);

         /* Return last size */
         return curSize - glyph->getAdvanceX();
      }
   }

   /* The whole string fits */
   fit = text;
   wontFit = "";
   return curSize;
}

/***********************************************************************
 *                                write                                *
 ***********************************************************************/
int Font::write(Surface* surface, int x, int y, const Rect& area, 
      const Kobold::String& text)
{
   return write(surface, x, y, area, (Uint8*) text.c_str(), 0);
}

/***********************************************************************
 *                                write                                *
 ***********************************************************************/
int Font::write(Surface* surface, const Rect& area, const Kobold::String& text,
      int outline)
{
   if(curFace == NULL)
   {
      return 0;
   }
   
   /* Let's go down by the maximum height, to make sure all glyph will
    * fit at the start position. */
   return write(surface, area.getX1() + FONT_HORIZONTAL_DELTA, 
         area.getY1() + curFace->getFontHeight(), area, (Uint8*) text.c_str(),
         outline);
}

/***********************************************************************
 *                               write                                 *
 ***********************************************************************/
int Font::write(Surface* surface, const Rect& area, const Kobold::String& text, 
      const Color& outlineColor, int outline)
{
   Draw* draw = Farso::Controller::getDraw();
   Color curColor = draw->getActiveColor();

   /* First, let's write with outline color */
   draw->setActiveColor(outlineColor);
   write(surface, area, text, outline);

   /* Now, write with active color, to be the front */
   draw->setActiveColor(curColor);
   return write(surface, area, text, 0);
}

/***********************************************************************
 *                        writeBreakingOnSpaces                        *
 ***********************************************************************/
int Font::writeBreakingOnSpaces(Surface* surface, const Rect& area, 
      const Kobold::String& text, const Color& outlineColor, int outline)
{
   Kobold::String wontFit = text;
   Kobold::String fit;
   Kobold::String curText = text;

   /* Get current width to break things */
   int width = area.getWidth() - (2 * FONT_HORIZONTAL_DELTA);

   /*  define initial position */
   int curWrote = 0;
   int curY = area.getY1();
   int incY = curFace->getFontHeight();
   bool brokeOnSpace = false;

   /* Let's write it */
   while((!wontFit.empty()) && (curY <= area.getY2()))
   {
      curText = wontFit;
      getWhileFits(curText, fit, wontFit, width, brokeOnSpace);

      if(fit.empty())
      {
         /* Nothing fits, must exit. */
         return curWrote;
      }

      /* Write the text */
      if(outline > 0)
      {
         write(surface, Rect(area.getX1(), curY, area.getX2(), area.getY2()), 
               fit, outlineColor, outline);
      }
      else
      {
         write(surface, Rect(area.getX1(), curY, area.getX2(), area.getY2()),
               fit);
      }

      /* Check if we are yet inside the rectange */
      curY += incY;
   }

   return curWrote;
}

/***********************************************************************
 *                           willGlyphFits                             *
 ***********************************************************************/
bool Font::willGlyphFits(int x, int y, const Rect& area, CachedGlyph* glyph)
{
   return ((y - glyph->getBitmapTop() >= area.getY1()) &&
           (x + glyph->getBitmapLeft() >= area.getX1()) &&
           (y - glyph->getBitmapTop() + 
                (int)glyph->getBitmap()->rows <= area.getY2()) &&
           (x + glyph->getBitmapLeft() + 
                (int)glyph->getBitmap()->width <= area.getX2()));
}

/***********************************************************************
 *                                write                                *
 ***********************************************************************/
int Font::write(Surface* surface, int x, int y, const Rect& area, 
      const Uint8* utf8, int outline)
{
   /* make sure surface is valid */
   assert(surface != NULL);
   /* make sure rectangle area is inside the surface */
   assert((area.getX1() >= 0) && (area.getX1() < surface->getWidth()));
   assert((area.getX2() >= 0) && (area.getX2() < surface->getWidth()));
   assert((area.getY1() >= 0) && (area.getY1() < surface->getHeight()));
   assert((area.getY2() >= 0) && (area.getY2() < surface->getHeight()));

   if(curFace == NULL)
   {
      return 0;
   }

   if((curAlign == TEXT_RIGHT) || (curAlign == TEXT_CENTERED))
   {
      return centeredOrRightWrite(surface, x, y, area, utf8, outline);
   }

   /* Calculate ammount to 'jump' on each line */
   int incY = curFace->getFontHeight(); 

   int renderedChars = 0;
   size_t texlen = strlen((char*) utf8);

   while(texlen > 0)
   {
      Uint16 c = getchUTF8(&utf8, &texlen);

      Font::CachedGlyph* glyph = curFace->getGlyph(c, outline, freeTypeLib);
      
      if(glyph == NULL)
      {
         Kobold::Log::add(Kobold::LOG_LEVEL_ERROR, 
               "Warn: couldn't find glyph %d on font %s", 
               c, filename.c_str()); 
         return 0;
      }

      /* Test if currently fits on line, or need a next line. */
      if(x + glyph->getAdvanceX() >= area.getX2() - FONT_HORIZONTAL_DELTA)
      {
         /* go to next potential line. */
         y += incY;
         x = area.getX1() + FONT_HORIZONTAL_DELTA;
      }

      /* Let's see if the glyph actually fits on the defined area */
      if(!willGlyphFits(x, y, area, glyph))
      {
         /* Current character won't fit. Let's stop. */
         return renderedChars;
      }

      Farso::Controller::getDraw()->doFreeTypeStamp(surface, x, y, 
            glyph->getBitmap(), glyph->getBitmapLeft(), 
            glyph->getBitmapTop());

      x += glyph->getAdvanceX();

      renderedChars++;
   }

   return renderedChars;
}

/***********************************************************************
 *                                flushLine                            *
 ***********************************************************************/
void Font::flushLine(Surface* surface, int x, int y, Uint16* chars, 
      int lastIndex, int areaWidth, int textWidth, int outline)
{
   if(lastIndex < 0)
   {
      /* Nothing to flush */
      return;
   }

   /* Define initial position */
   if(curAlign == TEXT_RIGHT)
   {
      x += areaWidth - textWidth - FONT_HORIZONTAL_DELTA;
   }
   else if(curAlign == TEXT_CENTERED)
   {
      x += ((areaWidth - textWidth - FONT_HORIZONTAL_DELTA) / 2);
   }


   Font::CachedGlyph* glyph;

   for(int i = 0; i <= lastIndex; i++)
   {
      glyph = curFace->getGlyph(chars[i], outline, freeTypeLib);
      Farso::Controller::getDraw()->doFreeTypeStamp(surface, x, y, 
            glyph->getBitmap(), glyph->getBitmapLeft(), 
            glyph->getBitmapTop());

      x += glyph->getAdvanceX();
   }
}

/***********************************************************************
 *                          centeredOrRightWrite                       *
 ***********************************************************************/
int Font::centeredOrRightWrite(Surface* surface, int x, int y, 
      const Rect& area, const Uint8* utf8, int outline)
{
   int renderedChars = 0;
   size_t texlen = strlen((char*) utf8);

   /* let's allocate the 'pending-to-render' characters, in worst case 
    * scenario: all characters as a single byte ones, and all on a single
    * line. */
   Uint16* toRender = new Uint16[texlen];
   int curToRender = -1;
   int curX = x;
   int curWidth = 0;

   while(texlen > 0)
   {
      Uint16 c = getchUTF8(&utf8, &texlen);

      Font::CachedGlyph* glyph = curFace->getGlyph(c, outline, freeTypeLib);
      
      if(glyph == NULL)
      {
         delete[] toRender;
         return 0;
      }

      /* Test if currently fits on line, or need a next line. */
      if(curX + glyph->getAdvanceX() >= 
            area.getX2() - FONT_HORIZONTAL_DELTA)
      {
         /* Won't fit, must flush the current toRender glyphs */
         flushLine(surface, x, y, &toRender[0], curToRender, 
               area.getWidth(), curWidth, outline);

         /* go to next potential line. */
         y += curFace->getIncY();
         x = area.getX1() + FONT_HORIZONTAL_DELTA;
         curX = x;
         curWidth = 0;
         curToRender = -1;
      }

      /* Let's see if the glyph actually fits on the defined area */
      if(!willGlyphFits(x, y, area, glyph))
      {
         /* Current character won't fit. Let's stop. */
         /*printf("glyph: %d,%d %d,%d area: %d,%d %d,%d", 
               x + glyph->getBitmapLeft(),
               y - glyph->getBitmapTop(), 
               x + glyph->getBitmapLeft() + (int) glyph->getBitmap()->width,
               y - glyph->getBitmapTop() + (int) glyph->getBitmap()->rows,
                area.getX1(), area.getY1(), area.getX2(), area.getY2());*/
         flushLine(surface, x, y, &toRender[0], curToRender, 
               area.getWidth(), curWidth, outline);
         delete[] toRender;
         return renderedChars;
      }

      /* Buffer to render latter when got full line. */
      curToRender++;
      toRender[curToRender] = c;

      curWidth += glyph->getAdvanceX();
      curX += glyph->getAdvanceX();

      renderedChars++;
   }

   flushLine(surface, x, y, &toRender[0], curToRender, 
         area.getWidth(), curWidth, outline);
   delete[] toRender;
   return renderedChars;

}

/***********************************************************************
 *                          SDL_TTF: getchUTF8                         *
 ***********************************************************************/
#define UNKNOWN_UNICODE 0xFFFD
Uint16 Font::getchUTF8(const Uint8** src, size_t *srclen)
{
   const Uint8 *p = *(const Uint8**)src;
   size_t left = 0;
   bool overlong = false;
   bool underflow = false;
   Uint16 ch = UNKNOWN_UNICODE;

   if (*srclen == 0) 
   {
      return UNKNOWN_UNICODE;
   }
   if (p[0] >= 0xFC) 
   {
      if ((p[0] & 0xFE) == 0xFC) 
      {
         if (p[0] == 0xFC && (p[1] & 0xFC) == 0x80) 
         {
            overlong = true;
         }
         ch = (Uint32) (p[0] & 0x01);
         left = 5;
      }
   } else if (p[0] >= 0xF8) 
   {
      if ((p[0] & 0xFC) == 0xF8) 
      {
         if (p[0] == 0xF8 && (p[1] & 0xF8) == 0x80) 
         {
            overlong = true;
         }
         ch = (Uint32) (p[0] & 0x03);
         left = 4;
      }
   } 
   else if (p[0] >= 0xF0) 
   {
      if ((p[0] & 0xF8) == 0xF0) 
      {
         if (p[0] == 0xF0 && (p[1] & 0xF0) == 0x80) 
         {
            overlong = true;
         }
         ch = (Uint32) (p[0] & 0x07);
         left = 3;
      }
   } 
   else if (p[0] >= 0xE0) 
   {
      if ((p[0] & 0xF0) == 0xE0) 
      {
         if (p[0] == 0xE0 && (p[1] & 0xE0) == 0x80) 
         {
            overlong = true;
         }
         ch = (Uint32) (p[0] & 0x0F);
         left = 2;
      }
   } 
   else if (p[0] >= 0xC0) 
   {
      if ((p[0] & 0xE0) == 0xC0) 
      {
         if ((p[0] & 0xDE) == 0xC0) 
         {
            overlong = true;
         }
         ch = (Uint32) (p[0] & 0x1F);
         left = 1;
      }
   } 
   else 
   {
      if ((p[0] & 0x80) == 0x00) 
      {
         ch = (Uint32) p[0];
      }
   }
   ++*src;
   --*srclen;
   while (left > 0 && *srclen > 0) 
   {
      ++p;
      if ((p[0] & 0xC0) != 0x80) 
      {
         ch = UNKNOWN_UNICODE;
         break;
      }
      ch <<= 6;
      ch |= (p[0] & 0x3F);
      ++*src;
      --*srclen;
      --left;
   }
   if (left > 0) 
   {
      underflow = true;
   }
   if (overlong) return UNKNOWN_UNICODE;
   if (underflow ||
         (ch >= 0xD800 && ch <= 0xDFFF) ||
         (ch == 0xFFFE || ch == 0xFFFF) || ch > 0x10FFFF) 
   {
      ch = UNKNOWN_UNICODE;
   }
   return ch;
}




