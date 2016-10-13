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
   cursorPos = 0;
   cursorIndex = 0;
   editing = false;
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
TextEntry::~TextEntry()
{
}


/***********************************************************************
 *                          getStringLength                            *
 ***********************************************************************/
int TextEntry::getStringLength(Kobold::String s)
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
int TextEntry::getNextCharacterInit(Kobold::String str, int cur)
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
int TextEntry::getPreviousCharacterInit(Kobold::String str, int cur)
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
void TextEntry::appendToCursor(Kobold::String textToAppend)
{
   Kobold::String text = getCaption();
   text.insert(cursorIndex, textToAppend);
   cursorPos += getStringLength(textToAppend);
   cursorIndex += textToAppend.length();
   setCaption(text);
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
         setCaption(text);
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
         setCaption(text);
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
      cursorPos = 0;
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
      cursorPos = getStringLength(text);
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
            cursorPos++;
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
            cursorPos--;
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
      editing = false;
      setDirty();
   }
}

/***********************************************************************
 *                              getBody                                *
 ***********************************************************************/
Rect TextEntry::getBody()
{
   return body;
}

/***********************************************************************
 *                                doDraw                               *
 ***********************************************************************/
void TextEntry::doDraw(Rect pBody)
{
   Skin* skin = Controller::getSkin();
   Draw* draw = Controller::getDraw();
   Surface* surface = getWidgetRenderer()->getSurface();
   Font* font;
   Color fontColor;

   /* define coordinates on parent's surface */
   int x1 = getX() + pBody.getX1();
   int x2 = x1 + getWidth() - 1;
   int y1 = getY() + pBody.getY1();
   int y2 = y1 + getHeight() - 1;

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
      font = FontManager::getFont(skEl.getFontName());
      font->setSize(skEl.getFontSize());
      font->setAlignment(Font::TEXT_LEFT);
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

      font = FontManager::getDefaultFont();
      font->setSize(10);
      font->setAlignment(Font::TEXT_LEFT);
      fontColor = Colors::colorCont[1];
   }

   /* Draw the text */
   if(isAvailable())
   {
      draw->setActiveColor(fontColor);
   }
   else
   {
      draw->setActiveColor(Colors::colorText);
   }

   /* Let's define visible string */
   int init = 0;
   int end = (int)getCaption().length();
   Kobold::String writeText = getCaption();
   int maxWidth = getWidth() - 6;
   while(font->getWidth(writeText) > maxWidth )
   {
      if(end > (int) cursorIndex)
      {     
         int lastEnd = end;
         end = getPreviousCharacterInit(writeText, lastEnd);
         writeText.erase(writeText.length() - (lastEnd - end));
      }
      else
      {
         int newInit = getNextCharacterInit(writeText, 0);
         writeText.erase(0, newInit);
         init += newInit;
      }
   }

   font->write(surface, Rect(x1, y1, x2, y2), writeText);

   if(editing)
   {
      /* Must draw cursor */
      Kobold::String s = getCaption().substr(init, end).substr(0, 
            cursorIndex - init);
      int x = x1 + 2 + font->getWidth(s);
      draw->doLine(surface, x, y1+3, x, y2-3);
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
      if((leftButtonPressed) && (!isInner(mrX, mrY)))
      {
         editing = false;
         Controller::setEvent(this, EVENT_TEXTENTRY_EDITION_DONE);
         Kobold::Keyboard::stopEditingText();
         setDirty();
      }
      else
      {
         /* Still editing */
         Controller::setEvent(this, EVENT_TEXTENTRY_EDITING);
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


