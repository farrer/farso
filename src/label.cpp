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

#include "label.h"
#include "controller.h"
#include "skin.h"
#include "font.h"
#include <assert.h>
using namespace Farso;

/******************************************************************
 *                           Constructor                          *
 ******************************************************************/
Label::Label(int x, int y, int width, int height, Kobold::String caption, 
             Widget* parent)
      : Widget(Widget::WIDGET_TYPE_LABEL, x, y, width, height, parent)
{
   this->setCaption(caption);
   this->fontSize = -1;
   this->fontAlign = Font::TEXT_LEFT;
   this->definedColor = false;
   this->pressStarted = false;
   this->body.set(getX(), getY(), getX() + width - 1, getY() + width -1);
   this->skinType = Skin::SKIN_TYPE_LABEL;
   this->definedSkin = false;
   this->useBorder = false;
   this->outline = 0;
   this->outlineColor = Farso::Color(0, 0, 0, 255);
   this->breakLineOnSpace = false;
}

/******************************************************************
 *                           Destructor                           *
 ******************************************************************/
Label::~Label()
{
}

/******************************************************************
 *                            seDirty                             *
 ******************************************************************/
void Label::setDirty()
{
   setDirtyWithParent();
}

/******************************************************************
 *                          enableBorder                          *
 ******************************************************************/
void Label::enableBorder()
{
   this->useBorder = true;
}

/******************************************************************
 *                         disableBorder                          *
 ******************************************************************/
void Label::disableBorder()
{
   this->useBorder = false;
}

/******************************************************************
 *                             setSkinType                        *
 ******************************************************************/
void Label::setSkinType(Skin::SkinElementType type)
{
   this->skinType = type;
   this->definedSkin = true;
}

/******************************************************************
 *                              setFont                           *
 ******************************************************************/
void Label::setFont(Kobold::String fontName)
{
   this->fontName = fontName;
}

/******************************************************************
 *                           setFontSize                          *
 ******************************************************************/
void Label::setFontSize(int size)
{
   assert(size > 0);
   this->fontSize = size;
}

/******************************************************************
 *                           setFontColor                         *
 ******************************************************************/
void Label::setFontColor(Farso::Color color)
{
   this->fontColor = color;
   this->definedColor = true;
}

/******************************************************************
 *                        setFontAlignment                        *
 ******************************************************************/
void Label::setFontAlignment(Farso::Font::Alignment align)
{
   this->fontAlign = align;
}

/******************************************************************
 *                       setFontOutline                           *
 ******************************************************************/
void Label::setFontOutline(int outline, Farso::Color color)
{
   this->outline = outline;
   this->outlineColor = color;
}

/******************************************************************
 *                              getFont                           *
 ******************************************************************/
Font* Label::getFont()
{
   Font* font = NULL;
   Skin* skin = Controller::getSkin();

   if(fontName.empty())
   {
      if(skin == NULL)
      {
         font = FontManager::getDefaultFont();
      }
      else
      {
         font = FontManager::getFont(skin->getSkinElement(
                  skinType).getFontName());
      }
   }
   else
   {
      font = FontManager::getFont(fontName);
   }
   assert(font != NULL);
   return font;
}

/******************************************************************
 *                            getFontSize                         *
 ******************************************************************/
int Label::getFontSize()
{
   if(fontSize != -1)
   {
      return fontSize;
   }
   
   Skin* skin = Controller::getSkin();
   if(skin)
   {
      return skin->getSkinElement(skinType).getFontSize();

   }

   return 10;
}

/******************************************************************
 *                   enableBreakLineOnSpace                       *
 ******************************************************************/
void Label::enableBreakLineOnSpace()
{
   breakLineOnSpace = true;
}

/******************************************************************
 *                              doDraw                            *
 ******************************************************************/
void Label::doDraw(Rect pBody)
{
   Farso::Skin* skin = Farso::Controller::getSkin();
   Farso::Surface* surface = getWidgetRenderer()->getSurface();
   Farso::Draw* draw = Farso::Controller::getDraw();
   
   Font* font;
   Color color = fontColor;

   int x1 = getX();
   int y1 = getY();
   int x2 = x1 + getWidth() - 1;
   int y2 = y1 + getHeight() - 1;
   body.set(x1, y1, x2, y2);

   int rx1 = pBody.getX1() + x1;
   int ry1 = pBody.getY1() + y1;
   int rx2 = pBody.getX1() + x2;
   int ry2 = pBody.getY1() + y2;

   if(skin != NULL)
   {
      /* Let's check if we need to define skin based on enable/disable state */
      if(!definedSkin)
      {
         /* Not hard defined skin, let's check which skin we should use */
         if(isAvailable())
         {
            skinType = Skin::SKIN_TYPE_LABEL;
         }
         else
         {
            skinType = Skin::SKIN_TYPE_LABEL_DISABLED;
         }
      }

      if(useBorder)
      {
         /* Let's draw its border */
         skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_LEFT,
               rx1, ry1, rx2, ry2);
         skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_RIGHT,
               rx1, ry1 , rx2, ry2);
         skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_TOP,
               rx1, ry1, rx2, ry2);
         skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_BOTTOM,
               rx1, ry1, rx2, ry2);
      }

      /* Draw the skin, without the text */
      skin->drawElement(surface, skinType, rx1, ry1, rx2, ry2); 

      /* Define font to use */
      Skin::SkinElement skEl = skin->getSkinElement(skinType);
      font = getFont();
      if(fontSize != -1)
      {
         font->setSize(fontSize);
      }
      else
      {
         font->setSize(skEl.getFontSize());
      }
      
      /* set text color */
      if(!definedColor)
      {
         color = skEl.getFontColor();
      }

   }
   else
   {
      /* let's draw its border, if needed */
      if(useBorder)
      {
         draw->setActiveColor(Colors::colorCont[0]);
         draw->doRectangle(surface, rx1, ry1, rx2, ry2);
      }

      /* define font to use */
      font = getFont();

      /* Define font size to use */
      if(fontSize != -1)
      {
         font->setSize(fontSize);
      }
      else
      {
         font->setSize(10);
      }
      
      /* set text color */
      if(!definedColor)
      {
         if(isAvailable())
         {
            color = Colors::colorText;
         }
         else
         {
            color = Colors::colorCont[1];
         }
      }
   }

   /* Let's write the caption, as defined by user or defaults. */
   if(!getCaption().empty())
   {
      draw->setActiveColor(color);
      font->setAlignment(fontAlign);

      if(breakLineOnSpace)
      {
         /* Write trying to break line only on spaces. */
         font->writeBreakingOnSpaces(surface, Rect(rx1, ry1, rx2, ry2), 
               getCaption(), outlineColor, outline);
      }
      else
      {
         /* Normal write */
         if(outline > 0)
         {
            font->write(surface, Rect(rx1, ry1, rx2, ry2), getCaption(),
                  outlineColor, outline);
         }
         else
         {
            font->write(surface, Rect(rx1, ry1, rx2, ry2), getCaption());
         }
      }
   }
}

/******************************************************************
 *                             doTreat                            *
 ******************************************************************/
bool Label::doTreat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY, int mrX, int mrY)
{
   if(leftButtonPressed)
   { 
      if((!pressStarted) && (isInner(mrX, mrY)))
      {
         pressStarted = true;
      }
   }
   else if(pressStarted)
   {
      pressStarted = false;
      Controller::setEvent(this, EVENT_LABEL_CLICKED);
      return true;
   }
   /* No event happens in a label. */
   return false;
}

/******************************************************************
 *                         doAfterChildTreat                      *
 ******************************************************************/
void Label::doAfterChildTreat()
{
   /* Nothing to do. */
}

/******************************************************************
 *                             getBody                            *
 ******************************************************************/
Farso::Rect Label::getBody()
{
   return body;
}

