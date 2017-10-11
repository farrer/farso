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
Button::Button(int x, int y, int width, int height, 
      const Kobold::String& caption, Widget* parent)
      :Widget(Widget::WIDGET_TYPE_BUTTON, x, y, width, height, parent)
{
   this->menu = NULL;
   this->pressStarted = false;
   this->pressed = false;
   this->useRoundedEdges = false;
   this->useDecorations = true;
   this->fontName = FontManager::getDefaultFontFilename();
   this->fontSize = 10;
   this->enabledType = Skin::SKIN_TYPE_BUTTON_ENABLED;
   this->disabledType = Skin::SKIN_TYPE_BUTTON_DISABLED;
   this->onOverType = Skin::SKIN_TYPE_BUTTON_ON_OVER;
   this->pressedType = Skin::SKIN_TYPE_BUTTON_PRESSED;
   this->body.set(getX(), getY(), getX() + width - 1, getY() + height - 1);
   this->setCaption(caption);
}

/******************************************************************
 *                            destructor                          *
 ******************************************************************/
Button::~Button()
{
}

/******************************************************************
 *                       setWithoutDecorations                    *
 ******************************************************************/     
void Button::setWithoutDecorations()
{
   setDirty();
   this->useDecorations = false;
}

/******************************************************************
 *                         setRoundedEdges                        *
 ******************************************************************/
void Button::setRoundedEdges()
{
   this->useRoundedEdges = true;
   setDirty();
}

/******************************************************************
 *                            setDirty                            *
 ******************************************************************/
void Button::setDirty()
{
   if(!useDecorations)
   {
      setDirtyWithParent();
   }
   else
   {
      Widget::setDirty();
   }
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
void Button::setFont(const Kobold::String& fontName, int fontSize)
{
   this->fontName = fontName;
   this->fontSize = fontSize;
}

/******************************************************************
 *                             setMenu                            *
 ******************************************************************/
void Button::setMenu(Menu* menu)
{
   this->menu = menu;
}

/******************************************************************
 *                              press                             *
 ******************************************************************/
void Button::press()
{
   if(!pressed)
   {
      if(useDecorations)
      {
         setDirty();
      }
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
      if(useDecorations)
      {
         setDirty();
      }
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
      bool shouldRelease = true;
      pressStarted = false;
      
      /* Let's see if was a valid release or a cancelled one. */
      if(isInner(mrX, mrY))
      {
         if(menu)
         {
            /* We have a menu, let's open it. But before, we need to define
             * the coordinate just after this button. */
            Rect pbody(0,0,0,0);
            WidgetRenderer* renderer = getWidgetRenderer(); 
            if(getParent() != NULL)
            {
               pbody = getParent()->getBodyWithParentsApplied();
            }
            menu->open(renderer->getPositionX() + pbody.getX1() + getX(), 
                       renderer->getPositionY() + pbody.getY1() + getY() + 
                       getHeight(), this);

            Controller::setEvent(this, EVENT_MENU_OPENED);
            /* Let's keep the button pressed */
            shouldRelease = false;
         }
         else
         {
            Controller::setEvent(this, EVENT_BUTTON_RELEASED);
         }
      }
      else
      {
         Controller::setEvent(this, EVENT_BUTTON_PRESS_CANCELLED);
      }

      if(shouldRelease)
      {
         release();
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
const Farso::Rect& Button::getBody()
{
   return body;
}

/******************************************************************
 *                              doDraw                            *
 ******************************************************************/
void Button::doDraw(const Rect& pBody)
{
   Farso::Skin* skin = Farso::Controller::getSkin();
   Farso::Surface* surface = getWidgetRenderer()->getSurface();

   int xt = getX();
   int yt = getY();

   /* Define real coordinates on surface. */
   int rx1 = pBody.getX1() + xt;
   int ry1 = pBody.getY1() + yt;
   int rx2 = pBody.getX1() + xt + getWidth() - 1;
   int ry2 = pBody.getY1() + yt + getHeight() - 1;

   bool specialCaption = ( (getCaption() == BUTTON_SPECIAL_CAPTION_LEFT) ||
                           (getCaption() == BUTTON_SPECIAL_CAPTION_RIGHT) ||
                           (getCaption() == BUTTON_SPECIAL_CAPTION_UP) ||
                           (getCaption() == BUTTON_SPECIAL_CAPTION_DOWN) );

   if(skin != NULL)
   {
      Kobold::String useCaption = (specialCaption) ? "" : getCaption();

      if(useDecorations)
      {
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
      }
      else if(!specialCaption)
      {
         /* Without decorations and not a special caption, just act
          * as a common label. */
         skin->drawElement(surface, Skin::SKIN_TYPE_LABEL, rx1, ry1, rx2, ry2, 
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

         if(pressed && useDecorations)
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

      if(useDecorations)
      {
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
         if(!getCaption().empty())
         {
            /* Draw the button caption */
            Font* font = FontManager::getFont(fontName);
            font->setSize(fontSize);
            font->setAlignment(Font::TEXT_CENTERED);
            if(pressed && useDecorations)
            {
               font->write(surface, Rect(rx1 + 1, ry1 + 1, rx2, ry2), 
                     getCaption());
            }
            else 
            {
               font->write(surface, Rect(rx1, ry1, rx2, ry2), getCaption());
            }
         }
      }
      else
      {
         Farso::Draw* draw = Farso::Controller::getDraw();
         draw->setActiveColor(Colors::colorText);
         if(pressed && useDecorations)
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

   /* Set body (when pressed and with decorations, a bit changed) */
   if(pressed && useDecorations)
   {
      this->body.set(getX() + 1, getY() + 1, 
                     getX() + getWidth() - 2, getY() + getHeight() - 2);
   }
   else
   {
      this->body.set(getX(), getY(), 
                     getX() + getWidth() - 1, 
                     getY() + getHeight() - 1);
   }
}


