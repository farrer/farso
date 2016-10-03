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

#include "font.h"
#include "controller.h"
#include <math.h>
#include <kobold/log.h>
#include <OGRE/OgreDataStream.h>
#include <OGRE/OgreResourceGroupManager.h>
#include <assert.h>
using namespace Farso;

/*! Delta for min horizontal distance to keep from write area X axys border. */
#define HORIZONTAL_DELTA   2

/***********************************************************************
 *                                 init                                *
 ***********************************************************************/
void FontManager::init()
{
   defaultFont = "";
   int error = FT_Init_FreeType(&freeTypeLib);
   if(error)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
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
Font* FontManager::setDefaultFont(Kobold::String filename)
{
   assert(filename != "");
   defaultFont = filename;
   return getFont(filename);
}

/***********************************************************************
 *                          getDefaultFont                             *
 ***********************************************************************/
Font* FontManager::getDefaultFont()
{
   assert(defaultFont != "");
   return getFont(defaultFont);
}
Kobold::String FontManager::getDefaultFontFileName()
{
   return defaultFont;
}

/***********************************************************************
 *                              getFont                                *
 ***********************************************************************/
Font* FontManager::getFont(Kobold::String filename)
{
   std::map<Kobold::String, Font*>::iterator it = fonts.find(filename);

   if(it == fonts.end())
   {
      /* Font not yet loaded, let's create it */
      Font* f = new Font(filename, &freeTypeLib);
      fonts[filename] = f;
      return f;
   }

   return it->second;

}

/***********************************************************************
 *                            unloadAllFonts                           *
 ***********************************************************************/
void FontManager::unloadAllFonts()
{
  for(std::map<Kobold::String, Font*>::iterator it = fonts.begin(); 
       it != fonts.end(); ++it)
   {
      delete it->second;
   }
   fonts.clear();
}

/***********************************************************************
 *                               Members                               *
 ***********************************************************************/
FT_Library FontManager::freeTypeLib;
std::map<Kobold::String, Font*> FontManager::fonts;
Kobold::String FontManager::defaultFont;

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
Font::CachedGlyph* Font::FaceInfo::getGlyph(Uint16 c)
{
   assert(face != NULL);
   int index = FT_Get_Char_Index((*face), c) % FONT_GLYPH_CACHE_SIZE;

   /* Check if the character at the cache index is the same of the 
    * desired one */
   if(cache[index].getChar() == c)
   {
      /* Already in cache, let's just use it. */
      return &cache[index];
   }

   /* Cache miss: must load the character to the cache */
   if(FT_Load_Char((*face), c, FT_LOAD_RENDER) != 0)
   {
      return NULL;
   }
   cache[index].load((*face)->glyph, c);

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
void Font::CachedGlyph::load(FT_GlyphSlot slot, Uint16 c)
{
   /* Clear any pre-existent buffer, and create a new one, with same
    * content from slot bitmap. */
   clearBuffer();
   size_t size = slot->bitmap.pitch * slot->bitmap.rows;
   bitmap.buffer = new unsigned char[size];
   memcpy(&bitmap.buffer[0], &slot->bitmap.buffer[0], size);

   /* Copy the info that we use from bitmap */
   bitmap.width = slot->bitmap.width;
   bitmap.rows = slot->bitmap.rows;
   bitmap.pitch = slot->bitmap.pitch;
   bitmap.pixel_mode = slot->bitmap.pixel_mode;

   /* Define some often used variables */
   advanceX = slot->advance.x >> 6;
   bitmapLeft = slot->bitmap_left;
   bitmapTop = slot->bitmap_top;

   /* Set the glyph cache is now related to this character. */
   character = c;
}


/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
Font::Font(Kobold::String filename, FT_Library* lib)
{
   this->filename = filename;
   this->data = NULL;
   this->dataSize = 0;
   this->freeTypeLib = lib;
   this->curTextSize = 0;
   this->curFace = NULL;
   this->curAlign = TEXT_LEFT;

   load();
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
   Ogre::DataStreamPtr fileData;

   /* Open the file */
   try
   {
      fileData = Ogre::ResourceGroupManager::getSingleton().openResource(
            filename);
   }
   catch(Ogre::FileNotFoundException)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
         "ERROR: Couldn't open font file from resources: '%s'", 
         filename.c_str());
      return false;
   }
   dataSize = fileData->size();
   if(dataSize == 0)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
         "ERROR: Couldn't define filesize for font: '%s'", filename.c_str());
      fileData->close();
      return false;
   }

   /* Alloc buffer and load to it */
   data = new FT_Byte[dataSize];
   fileData->read(data, dataSize);

   /* Done */
   fileData->close();

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
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            (Kobold::String("ERROR: couldn't create face for '%s'.") + 
            Kobold::String("Error code: %d. Datasize: %d")).c_str(), 
            filename.c_str(), error, dataSize);
         
         delete face;
         return NULL;
      }
      /* Set the size */
      error = FT_Set_Char_Size(*face, 0, size * 64, 96, 96);
      if(error)
      {
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
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
int Font::getSize()
{
   return curTextSize;
}

/***********************************************************************
 *                           setAlignment                              *
 ***********************************************************************/
void Font::setAlignment(Font::Alignment align)
{
   curAlign = align;
}

/***********************************************************************
 *                           getAlignment                              *
 ***********************************************************************/
Font::Alignment Font::getAlignment()
{
   return curAlign;
}

/***********************************************************************
 *                            getHeight                                *
 ***********************************************************************/
int Font::getHeight(int areaWidth, Kobold::String text)
{
   if(curFace == NULL)
   {
      return 0;
   }
   int width = getWidth(text);
   int totalLines = (int) ceil(width / (float) areaWidth);

   return (totalLines * curFace->getIncY() + 1);
}

/***********************************************************************
 *                             getWidth                                *
 ***********************************************************************/
int Font::getWidth(Kobold::String text)
{
   int curSize = 0;
   const Uint8* utf8 = (Uint8*) text.c_str();
   size_t texlen = strlen((char*) utf8);

   while(texlen > 0)
   {
      /* Get the character and its glyph */
      Uint16 c = getchUTF8(&utf8, &texlen);
      Font::CachedGlyph* glyph = curFace->getGlyph(c);
      if(glyph == NULL)
      {
         return 0;
      }

      curSize += glyph->getAdvanceX();
   }

   return curSize;
}

/***********************************************************************
 *                                write                                *
 ***********************************************************************/
int Font::write(Surface* surface, int x, int y, Rect area, Kobold::String text)
{
   return write(surface, x, y, area, (Uint8*) text.c_str());
}

/***********************************************************************
 *                                write                                *
 ***********************************************************************/
int Font::write(Surface* surface, Rect area, Kobold::String text)
{
   if(curFace == NULL)
   {
      return 0;
   }
   
   /* Let's go down by the maximum height, to make sure all glypsh will
    * fit at the start position. */
   return write(surface, area.getX1() + HORIZONTAL_DELTA, 
         area.getY1() + curFace->getFontHeight(), area, (Uint8*) text.c_str());
}

/***********************************************************************
 *                           willGlyphFits                             *
 ***********************************************************************/
bool Font::willGlyphFits(int x, int y, Rect area, CachedGlyph* glyph)
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
int Font::write(Surface* surface, int x, int y, Rect area, const Uint8* utf8)
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
      return centeredOrRightWrite(surface, x, y, area, utf8);
   }

   /* Calculate ammount to 'jump' on each line */
   int incY = curFace->getIncY(); 

   int renderedChars = 0;
   size_t texlen = strlen((char*) utf8);

   while(texlen > 0)
   {
      Uint16 c = getchUTF8(&utf8, &texlen);

      Font::CachedGlyph* glyph = curFace->getGlyph(c);
      
      if(glyph == NULL)
      {
         return 0;
      }

      /* Test if currently fits on line, or need a next line. */
      if(x + glyph->getAdvanceX() >= area.getX2() - HORIZONTAL_DELTA)
      {
         /* go to next potential line. */
         y += incY;
         x = area.getX1() + HORIZONTAL_DELTA;
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
      int lastIndex, int areaWidth, int textWidth)
{
   if(lastIndex < 0)
   {
      /* Nothing to flush */
      return;
   }

   /* Define initial position */
   if(curAlign == TEXT_RIGHT)
   {
      x += areaWidth - textWidth - HORIZONTAL_DELTA;
   }
   else if(curAlign == TEXT_CENTERED)
   {
      x += ((areaWidth - textWidth - HORIZONTAL_DELTA) / 2);
   }


   Font::CachedGlyph* glyph;

   for(int i = 0; i <= lastIndex; i++)
   {
      glyph = curFace->getGlyph(chars[i]);
      Farso::Controller::getDraw()->doFreeTypeStamp(surface, x, y, 
            glyph->getBitmap(), glyph->getBitmapLeft(), 
            glyph->getBitmapTop());

      x += glyph->getAdvanceX();
   }
}

/***********************************************************************
 *                          centeredOrRightWrite                       *
 ***********************************************************************/
int Font::centeredOrRightWrite(Surface* surface, int x, int y, Rect area, 
      const Uint8* utf8)
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

      Font::CachedGlyph* glyph = curFace->getGlyph(c);
      
      if(glyph == NULL)
      {
         delete[] toRender;
         return 0;
      }

      /* Test if currently fits on line, or need a next line. */
      if(curX + glyph->getAdvanceX() >= 
            area.getX2() - HORIZONTAL_DELTA)
      {
         /* Won't fit, must flush the current toRender glyphs */
         flushLine(surface, x, y, &toRender[0], curToRender, 
               area.getWidth(), curWidth);

         /* go to next potential line. */
         y += curFace->getIncY();
         x = area.getX1() + HORIZONTAL_DELTA;
         curX = x;
         curWidth = 0;
         curToRender = -1;
      }

      /* Let's see if the glyph actually fits on the defined area */
      if(!willGlyphFits(x, y, area, glyph))
      {
         /* Current character won't fit. Let's stop. */
         /*printf("glyph: %d,%d %d,%d area: %d,%d %d,%d", x + slot->bitmap_left,
                y - slot->bitmap_top, 
                x + slot->bitmap_left + (int)slot->bitmap.width,
                y - slot->bitmap_top + (int)slot->bitmap.rows,
                area.getX1(), area.getY1(), area.getX2(), area.getY2());*/
         flushLine(surface, x, y, &toRender[0], curToRender, 
               area.getWidth(), curWidth);
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
         area.getWidth(), curWidth);
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




