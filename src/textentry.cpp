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

#include <kobold/platform.h>

#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID && \
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS

#include "textentry.h"
#include "controller.h"
using namespace Farso;

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
TextEntry::TextEntry(int x, int y, int width, int height, Widget* parent)
          :Widget(Widget::WIDGET_TYPE_TEXT_ENTRY, x, y, width, height, parent),
           body(x, y, x + width - 1, y + height - 1)
{
   cursorIndex = 0;
   curInit = 0;
   curEnd = 0;
   visibleText = "";
   editing = false;
   shouldStopEdition = false;
   redefineVisible = true;
   lastCheckX = -1;
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
TextEntry::~TextEntry()
{
}

/***********************************************************************
 *                             setCaption                              *
 ***********************************************************************/
void TextEntry::setCaption(const Kobold::String& text)
{
   if(getCaption() != text)
   {
      Widget::setCaption(text);
      cursorIndex = 0;
   }
}

/***********************************************************************
 *                          getStringLength                            *
 ***********************************************************************/
int TextEntry::getStringLength(const Kobold::String& s)
{
   const char* asChar = s.c_str();
   int i = 0;
   int j = 0;
   while(asChar[i])
   {
      if((s[i] & 0xC0) != 0x80)
      {
         /* UTf8 character init or acii/latin, count a new character */
         j++;
      }
      i++;
   }
   return j;
}

/***********************************************************************
 *                       getNextCharacterInit                          *
 ***********************************************************************/
int TextEntry::getNextCharacterInit(const Kobold::String& str, int cur)
{
   cur += 1;
   const char* asChar = str.c_str();
   while(cur < (int) str.length())
   {
      /* Check if is asc or UTF8 character init. */
      if((asChar[cur] & 0xC0) != 0x80)
      {
         return cur;
      }
      cur++;
   }

   /* No more characters, must be at \0 */
   return str.length();
}

/***********************************************************************
 *                     getPreviousCharacterInit                        *
 ***********************************************************************/
int TextEntry::getPreviousCharacterInit(const Kobold::String& str, int cur)
{
   cur -= 1;
   const char* asChar = str.c_str();
   while(cur > 0)
   {
      /* Check if is asc or UTF8 character init. */
      if((asChar[cur] & 0xC0) != 0x80)
      {
         return cur;
      }
      cur--;
   }

   /* No more characters before, must be the first */
   return 0;
}

/***********************************************************************
 *                          appendToCursor                             *
 ***********************************************************************/
void TextEntry::appendToCursor(const Kobold::String& textToAppend)
{
   Kobold::String text = getCaption();
   text.insert(cursorIndex, textToAppend);
   cursorIndex += textToAppend.length();
   Widget::setCaption(text);
}

/***********************************************************************
 *                       deleteFromCursor                              *
 ***********************************************************************/
void TextEntry::deleteCharacterFromCursor(bool after)
{
   Kobold::String text = getCaption();
   if(after)
   {
      /* Delete the character after cursor */
      if(cursorIndex < text.length())
      {   
         text.erase(cursorIndex, 
               getNextCharacterInit(text, cursorIndex) - cursorIndex);
         Widget::setCaption(text);
      }
   }
   else
   {
      /* Delete the character before cursor */
      if(cursorIndex > 0)
      {
         int indexOfPrev = getPreviousCharacterInit(text, cursorIndex);
         text.erase(indexOfPrev, cursorIndex - indexOfPrev);
         cursorIndex = indexOfPrev;
         Widget::setCaption(text);
      }
   }
}

/***********************************************************************
 *                          sendCursorToInit                           *
 ***********************************************************************/
void TextEntry::sendCursorToInit()
{
   if(cursorIndex != 0)
   {
      cursorIndex = 0;
      setDirty();
   }
}

/***********************************************************************
 *                          sendCursorToEnd                            *
 ***********************************************************************/
void TextEntry::sendCursorToEnd()
{
   Kobold::String text = getCaption();
   if(cursorIndex != text.length())
   {
      cursorIndex = text.length();
      setDirty();
   }
}

/***********************************************************************
 *                      changeCursorPosition                           *
 ***********************************************************************/
void TextEntry::changeCursorPosition(int delta)
{
   Kobold::String text = getCaption();
   while(delta != 0)
   {
      if(delta > 0)
      {
         /* Moving forward */
         if(cursorIndex < text.length())
         {
            cursorIndex = getNextCharacterInit(text, cursorIndex);
            setDirty();
         }
         delta--;
      }
      else
      {
         /* Moving backwards */
         if(cursorIndex > 0)
         {
            cursorIndex = getPreviousCharacterInit(text, cursorIndex);
            setDirty();
         }
         delta++;
      }
   }
}

/***********************************************************************
 *                          doneWithEditing                            *
 ***********************************************************************/
void TextEntry::doneWithEditing()
{
   if(editing)
   {
      shouldStopEdition = true;
   }
}

/***********************************************************************
 *                              getBody                                *
 ***********************************************************************/
const Rect& TextEntry::getBody()
{
   return body;
}

/***********************************************************************
 *                                doDraw                               *
 ***********************************************************************/
void TextEntry::doDraw(const Rect& pBody)
{
   Skin* skin = Controller::getSkin();
   Draw* draw = Controller::getDraw();
   Surface* surface = getWidgetRenderer()->getSurface();
   Font* font = getFont();
   Color fontColor;

   /* define coordinates on parent's surface */
   int x1 = getX() + pBody.getX1();
   int x2 = x1 + getWidth() - 1;
   int y1 = getY() + pBody.getY1();
   int y2 = y1 + getHeight() - 1;

   Rect textAreaDelta(0,0,0,0);

   if(skin)
   {
      Skin::SkinElement skEl;
      if(isAvailable())
      {
         skin->drawElement(surface, Skin::SKIN_TYPE_TEXTENTRY, 
               x1, y1, x2, y2);
         skEl = skin->getSkinElement(Skin::SKIN_TYPE_TEXTENTRY);  
      } 
      else
      {
         skin->drawElement(surface, Skin::SKIN_TYPE_TEXTENTRY_DISABLED, 
               x1, y1, x2, y2);
         skEl = skin->getSkinElement(Skin::SKIN_TYPE_TEXTENTRY_DISABLED);  
      }
      textAreaDelta = skEl.getTextAreaDelta();
      fontColor = skEl.getFontColor();
   }  
   else
   {
      /* Draw contorns */
      draw->setActiveColor(Colors::colorCont[0]);
      draw->doTwoColorsRectangle(surface, x1, y1, x2, y2, 
            Colors::colorCont[1]);
      /* Fill interior */
      if(isAvailable())
      {
         draw->setActiveColor(Colors::colorCont[2]);
      }
      else
      {
         draw->setActiveColor(Colors::colorCont[0]);
      }
      draw->doFilledRectangle(surface, x1 + 1, y1 + 1, x2 - 1, y2 - 1);

      fontColor = Colors::colorCont[1];
   }

   /* Draw the text */
   if((isAvailable()) || (skin))
   {
      draw->setActiveColor(fontColor);
   }
   else
   {
      draw->setActiveColor(Colors::colorText);
   }

   if(redefineVisible)
   {
      /* Let's define visible string */
      curInit = 0;
      curEnd = (int)getCaption().length();
      visibleText = getCaption();
      int maxWidth = getWidth() - 6 - 
         (textAreaDelta.getX1() + textAreaDelta.getX2());
      while(font->getWidth(visibleText) > maxWidth )
      {
         if(curEnd > (int) cursorIndex)
         {     
            int lastEnd = curEnd;
            curEnd = getPreviousCharacterInit(visibleText, lastEnd);
            visibleText.erase(visibleText.length() - (lastEnd - curEnd));
         }
         else
         {
            int newInit = getNextCharacterInit(visibleText, 0);
            visibleText.erase(0, newInit);
            curInit += newInit;
         }
      }
   }
   redefineVisible = true;

   font->write(surface, Rect(x1 + textAreaDelta.getX1(), 
                             y1 + textAreaDelta.getY1(), 
                             x2 - textAreaDelta.getX2(), 
                             y2 - textAreaDelta.getY2()), visibleText);

   if(editing)
   {
      /* Must draw cursor */
      Kobold::String s = visibleText.substr(0, cursorIndex - curInit);
      int x = x1 + 2 + font->getWidth(s) + textAreaDelta.getX1();
      draw->doLine(surface, x, y1 + 3 + textAreaDelta.getY1(), 
                            x, y2 - 3 - textAreaDelta.getY2());
   }

}

/***********************************************************************
 *                              doTreat                                *
 ***********************************************************************/
bool TextEntry::doTreat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY, int mrX, int mrY)
{
   if(editing)
   {
      /* Let's see if edition done by pressing outside the entry */
      if((shouldStopEdition) || ((leftButtonPressed) && (!isInner(mrX, mrY))))
      {
         redefineVisible = false;
         shouldStopEdition = false;
         editing = false;
         Controller::setEvent(this, EVENT_TEXTENTRY_EDITION_DONE);
         Kobold::Keyboard::stopEditingText();
         setDirty();
      }
      else
      {
         /* Still editing */
         Controller::setEvent(this, EVENT_TEXTENTRY_EDITING);
         if((leftButtonPressed) && (isInner(mrX, mrY)))
         {
            if(lastCheckX != mrX)
            {
               calculateCursorPosition(mrX, mrY);
               setDirty();
            }
         }
         else
         {
            lastCheckX = -1;
         }
      }

      return true;
   }
   else if(!Kobold::Keyboard::isEditingText())
   {
      /* Not editing this text entry and neither any other type of edit 
       * by keyboard: check if clicked on the text entry to start edition */
      if((leftButtonPressed) && (isInner(mrX, mrY)))
      {
         editing = true;
         Kobold::Keyboard::startEditingText((Kobold::KeyboardTextEditor*)this);
         Controller::setEvent(this, EVENT_TEXTENTRY_EDITION_START);
         /* Calculate cursor position, based on clicked */
         calculateCursorPosition(mrX, mrY);
         setDirty();
         return true;
      }
   }
   
   return false;
}

/***********************************************************************
 *                         doAfterChildTreat                           *
 ***********************************************************************/
void TextEntry::doAfterChildTreat()
{
   /* Nothing to do */
}

/***********************************************************************
 *                              getFont                                *
 ***********************************************************************/
Font* TextEntry::getFont()
{
   Font* font = NULL;
   Skin* skin = Controller::getSkin();

   if(skin)
   {
      Skin::SkinElement skEl;
      if(isAvailable())
      {
         skEl = skin->getSkinElement(Skin::SKIN_TYPE_TEXTENTRY);  
      } 
      else
      {
         skEl = skin->getSkinElement(Skin::SKIN_TYPE_TEXTENTRY_DISABLED);  
      }
      font = FontManager::getFont(skEl.getFontName());
      font->setSize(skEl.getFontSize());
      font->setAlignment(Font::TEXT_LEFT);
   }  
   else
   {
      font = FontManager::getDefaultFont();
      font->setSize(10);
      font->setAlignment(Font::TEXT_LEFT);
   }

   return font;
}

/***********************************************************************
 *                      calculateCursorPosition                        *
 ***********************************************************************/
void TextEntry::calculateCursorPosition(int mrX, int mrY)
{
   int lastIndex = curInit;
   cursorIndex = 0;
   int pos = 0;
   int size = visibleText.length();
   Font* font = getFont();
   lastCheckX = mrX;
   /* Transform mouse coordinate from relative to parent to the text entry. */
   int relX = mrX - getX();

   while((pos < relX) && ((lastIndex - curInit) < size))
   {
      cursorIndex = lastIndex;
      lastIndex = getNextCharacterInit(getCaption(), lastIndex);
      pos = font->getWidth(visibleText.substr(0, lastIndex - curInit));
   }

   /* Check if after visible */
   if((pos < relX) && (lastIndex >= size))
   {
      cursorIndex = size;
   }
   redefineVisible = false;
}

#endif

