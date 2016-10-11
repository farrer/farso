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
}

/******************************************************************
 *                           Destructor                           *
 ******************************************************************/
Label::~Label()
{
}

/******************************************************************
 *                             setSkinType                        *
 ******************************************************************/
void Label::setSkinType(Skin::SkinElementType type)
{
   this->skinType = type;
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
 *                              doDraw                            *
 ******************************************************************/
void Label::doDraw(Rect pBody)
{
   Farso::Skin* skin = Farso::Controller::getSkin();
   Farso::Surface* surface = getWidgetRenderer()->getSurface();

   int x1 = getX();
   int y1 = getY();
   int x2 = x1 + getWidth() - 1;
   int y2 = y1 + getHeight() - 1;
   body.set(x1, y1, x2, y2);

   if(skin != NULL)
   {
      int rx1 = pBody.getX1() + x1;
      int ry1 = pBody.getY1() + y1;
      int rx2 = pBody.getX1() + x2;
      int ry2 = pBody.getY1() + y2;

      //TODO: when font was defined to override the default element's one!
      
      skin->drawElement(surface, skinType, rx1, ry1, rx2, ry2, 
                        Rect(rx1, ry1, rx2, ry2), getCaption());
   }
   else
   {
      Farso::Draw* draw = Farso::Controller::getDraw();

      /* define font to use */
      Font* font = getFont();

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
      if(definedColor)
      {
         draw->setActiveColor(fontColor);
      }
      else
      {
         draw->setActiveColor(Colors::colorText);
      }
      
      /* Write the text */
      font->setAlignment(fontAlign);
      font->write(surface, Rect(x1 + pBody.getX1(), y1 + pBody.getY1(), 
               x2 + pBody.getX1(), y2 + pBody.getY1()), getCaption());
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

