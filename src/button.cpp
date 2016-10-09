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

#include "button.h"
#include "font.h"
#include "controller.h"
using namespace Farso;

/******************************************************************
 *                           constructor                          *
 ******************************************************************/
Button::Button(int x, int y, int width, int height, Kobold::String caption, 
      Widget* parent)
      :Widget(Widget::WIDGET_TYPE_BUTTON, x, y, width, height, parent)
{
   this->setCaption(caption);
   this->pressStarted = false;
   this->pressed = false;
   this->useRoundedEdges = false;
   this->fontName = FontManager::getDefaultFontFileName();
   this->fontSize = 10;
   this->enabledType = Skin::SKIN_TYPE_BUTTON_ENABLED;
   this->disabledType = Skin::SKIN_TYPE_BUTTON_DISABLED;
   this->onOverType = Skin::SKIN_TYPE_BUTTON_ON_OVER;
   this->pressedType = Skin::SKIN_TYPE_BUTTON_PRESSED;
}

/******************************************************************
 *                            destructor                          *
 ******************************************************************/
Button::~Button()
{
}

/******************************************************************
 *                         setRoundedEdges                        *
 ******************************************************************/
void Button::setRoundedEdges()
{
   this->useRoundedEdges = true;
}

/******************************************************************
 *                          setSkinTypes                          *
 ******************************************************************/
void Button::setSkinTypes(Skin::SkinElementType enabledType, 
      Skin::SkinElementType pressedType, Skin::SkinElementType onOverType,
      Skin::SkinElementType disabledType)
{
   this->enabledType = enabledType;
   this->disabledType = disabledType;
   this->pressedType = pressedType;
   this->onOverType = onOverType;
}


/******************************************************************
 *                             setFont                            *
 ******************************************************************/
void Button::setFont(Kobold::String fontName, int fontSize)
{
   this->fontName = fontName;
   this->fontSize = fontSize;
}

/******************************************************************
 *                              press                             *
 ******************************************************************/
void Button::press()
{
   if(!pressed)
   {
      setDirty();
      pressed = true;
   }
}

/******************************************************************
 *                            release                             *
 ******************************************************************/
void Button::release()
{
   if(pressed)
   {
      setDirty();
      pressed = false;
   }
}

/******************************************************************
 *                             doTreat                            *
 ******************************************************************/
bool Button::doTreat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY, int mrX, int mrY)
{
   if(leftButtonPressed)
   {
      if(isInner(mrX, mrY))
      {
         if(!pressStarted)
         {
            /* Start the button press */
            pressStarted = true;
            press();
            Controller::setEvent(this, EVENT_BUTTON_PRESS_INIT);
            return true;
         }
         else
         {
            /* Is pressing */
            press();
            Controller::setEvent(this, EVENT_BUTTON_PRESSING);
            return true; 
         }
      }
      else if(pressStarted)
      {
         /* No more inner, but still with active press procedure. */
         release();
         Controller::setEvent(this, EVENT_BUTTON_NOT_PRESSING);
         return true; 
      }
   }
   else if(pressStarted)
   {
      pressStarted = false;
      /* released the button when press procedure is active. */
      release();
      
      /* Let's see if was a valid release or a cancelled one. */
      if(isInner(mrX, mrY))
      {
         Controller::setEvent(this, EVENT_BUTTON_RELEASE);
      }
      else
      {
         Controller::setEvent(this, EVENT_BUTTON_PRESS_CANCELLED);
      }

      return true;
   }
   return false;
}

/******************************************************************
 *                        doAfterChildTreat                       *
 ******************************************************************/
void Button::doAfterChildTreat()
{
}

/******************************************************************
 *                              getBody                           *
 ******************************************************************/
Rect Button::getBody()
{
   return body;
}

/******************************************************************
 *                              doDraw                            *
 ******************************************************************/
void Button::doDraw(Rect pBody)
{
   Farso::Skin* skin = Farso::Controller::getSkin();
   Farso::Surface* surface = getWidgetRenderer()->getSurface();

   int xt = getX();
   int yt = getY();
   body.set(xt, yt, xt + getWidth() - 1, yt + getHeight() - 1);

   /* Define real coordinates on surface. */
   int rx1 = pBody.getX1() + body.getX1();
   int ry1 = pBody.getY1() + body.getY1();
   int rx2 = pBody.getX1() + body.getX2();
   int ry2 = pBody.getY1() + body.getY2();

   bool specialCaption = ( (getCaption() == BUTTON_SPECIAL_CAPTION_LEFT) ||
                           (getCaption() == BUTTON_SPECIAL_CAPTION_RIGHT) ||
                           (getCaption() == BUTTON_SPECIAL_CAPTION_UP) ||
                           (getCaption() == BUTTON_SPECIAL_CAPTION_DOWN) );

   if(skin != NULL)
   {
      Kobold::String useCaption = (specialCaption) ? "" : getCaption();

      /* Use the skin */
      if(pressed)
      {
         skin->drawElement(surface, pressedType, rx1, ry1, rx2, ry2, 
               Rect(rx1, ry1, rx2, ry2), useCaption);
      }
      else if(isAvailable())
      {
         skin->drawElement(surface, enabledType, rx1, ry1, rx2, ry2, 
               Rect(rx1, ry1, rx2, ry2), useCaption);
      } 
      else if(!isAvailable())
      {
         skin->drawElement(surface, disabledType, rx1, ry1, rx2, ry2, 
               Rect(rx1, ry1, rx2, ry2), useCaption);
      }

      if(specialCaption)
      {
         Skin::SkinElementType captionSkin;

         /* Must write the special caption skin element */
         if(getCaption() == BUTTON_SPECIAL_CAPTION_LEFT)
         {
            captionSkin = Skin::SKIN_TYPE_CAPTION_LEFT;
         }
         else if(getCaption() == BUTTON_SPECIAL_CAPTION_RIGHT)
         {
            captionSkin = Skin::SKIN_TYPE_CAPTION_RIGHT;
         }
         else if(getCaption() == BUTTON_SPECIAL_CAPTION_UP)
         {
            captionSkin = Skin::SKIN_TYPE_CAPTION_UP;
         }
         else /*if(getCaption() == BUTTON_SPECIAL_CAPTION_DOWN)*/
         {
            captionSkin = Skin::SKIN_TYPE_CAPTION_DOWN;
         }

         /* Calculate to set at middle (on both axys) */
         Rect bounds = skin->getSkinElement(captionSkin).getBounds(0, 0);
         rx1 = (((getWidth()) - bounds.getWidth()) / 2) + rx1;
         ry1 = (((getWidth()) - bounds.getHeight()) / 2) + ry1;

         if(pressed)
         {
            rx1++;
            ry1++;
         }

         skin->drawElement(surface, captionSkin, 
                  rx1, ry1, rx2, ry2); 

      }
   }
   else
   {
      /* No skins: default draw. */
      Farso::Draw* draw = Farso::Controller::getDraw();

      /* Define contorn colors (depends on if pressed or not) */
      Farso::Color cont1;
      Farso::Color cont2;
      if(pressed)
      {
         cont1 = Colors::colorCont[1];
         cont2 = Colors::colorCont[0];
      }
      else
      {
         cont1 = Colors::colorCont[0];
         cont2 = Colors::colorCont[1];
      }
      
      /* Draw the background */
      draw->setActiveColor(Colors::colorButton);
      draw->doFilledRectangle(surface, rx1+1, ry1+1, rx2-1, ry2-1);

      /* Draw the edges */
      draw->setActiveColor(cont1);
      if(useRoundedEdges)
      {
         draw->doRoundedRectangle(surface, rx1, ry1, rx2, ry2, cont2);
      }
      else
      {
         draw->doTwoColorsRectangle(surface, rx1, ry1, rx2, ry2, cont2);
      }

      /* Write the text */
      if(isAvailable())
      {
         draw->setActiveColor(Colors::colorText);
      }
      else
      {
         draw->setActiveColor(Colors::colorCont[1]);
      }

      if(!specialCaption)
      {
         /* Draw the button caption */
         Font* font = FontManager::getFont(fontName);
         font->setSize(fontSize);
         font->setAlignment(Font::TEXT_CENTERED);
         if(pressed)
         {
            font->write(surface, Rect(rx1 + 1, ry1 + 1, rx2, ry2), 
                        getCaption());
         }
         else 
         {
            font->write(surface, Rect(rx1, ry1, rx2, ry2), getCaption());
         }
      }
      else
      {
         Farso::Draw* draw = Farso::Controller::getDraw();
         draw->setActiveColor(Colors::colorText);
         if(pressed)
         {
            ry1++;
            ry2++;
         }
         if(getCaption() == BUTTON_SPECIAL_CAPTION_LEFT)
         {
            draw->doFilledTriangle(surface, rx1 + 5, (ry2 - ry1) / 2 + ry1,
                  rx2 - 5, ry1 + 5, rx2 - 5, ry2 - 5);
         }
         else if(getCaption() == BUTTON_SPECIAL_CAPTION_RIGHT)
         {
            draw->doFilledTriangle(surface, rx2 - 5, (ry2 - ry1) / 2 + ry1,
                  rx1 + 5, ry1 + 5, rx1 + 5, ry2 - 5);
         }
         else if(getCaption() == BUTTON_SPECIAL_CAPTION_UP)
         {
            draw->doFilledTriangle(surface, (rx2 - rx1) / 2 + rx1, ry1 + 5,
                  rx1 + 5, ry2 - 5, rx2 - 5, ry2 - 5);
         }
         else if(getCaption() == BUTTON_SPECIAL_CAPTION_DOWN)
         {
            draw->doFilledTriangle(surface, (rx2 - rx1) / 2 + rx1, ry2 - 5,
                  rx1 + 5, ry1 + 5, rx2 - 5, ry1 + 5);
         }
      }
   }
}


