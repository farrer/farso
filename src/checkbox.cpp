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

#include "checkbox.h"
#include "controller.h"
#include "skin.h"
using namespace Farso;

/******************************************************************
 *                           Constructor                          *
 ******************************************************************/
CheckBox::CheckBox(int x, int y, int width, const Kobold::String& label, 
      bool checked, Widget* parent)
         :Widget(Widget::WIDGET_TYPE_CHECK_BOX, x, y, width, 20, parent)
{
   this->setCaption(label);
   this->checked = checked;
   this->pressStarted = false;
   this->body.set(getX(), getY(), 
         getX() + getWidth() - 1, getY() + getHeight() -1);

   this->label = new Label(15, 0, width - 15, 20, label, this);
}

/******************************************************************
 *                            Destructor                          *
 ******************************************************************/
CheckBox::~CheckBox()
{
   /* No need to delete label, as it will be deleted on
    * checkbox's children list destructor. */
}

/******************************************************************
 *                               check                            *
 ******************************************************************/
void CheckBox::check()
{
   if(!checked)
   {
      checked = true;
      setDirty();
   }
}

/******************************************************************
 *                              uncheck                           *
 ******************************************************************/
void CheckBox::uncheck()
{
   if(checked)
   {
      checked = false;
      setDirty();
   }
}

/******************************************************************
 *                             isChecked                          *
 ******************************************************************/
bool CheckBox::isChecked()
{
   return checked;
}

/******************************************************************
 *                              disable                           *
 ******************************************************************/
void CheckBox::disable()
{
   label->disable();
   Widget::disable();
}

/******************************************************************
 *                              enable                            *
 ******************************************************************/
void CheckBox::enable()
{
   label->enable();
   Widget::enable();
}

/******************************************************************
 *                               doDraw                           *
 ******************************************************************/
void CheckBox::doDraw(const Rect& pBody)
{
   Farso::Skin* skin = Farso::Controller::getSkin();
   Farso::Surface* surface = getWidgetRenderer()->getSurface();

   body.set(getX(), getY(), getX() + getWidth() - 1, getY() + getHeight() -1);

   int rx1 = pBody.getX1() + getX();
   int ry1 = pBody.getY1() + getY();
   int rx2 = pBody.getX1() + getX() + getWidth() - 1;
   int ry2 = pBody.getY1() + getY() + getHeight() - 1;

   if(skin != NULL)
   {
      /* Determine which skin element to use for checkbox, according to 
       * its current state */
      Skin::SkinElementType skType;
      if(isAvailable())
      {
         if(checked)
         {
            skType = Skin::SKIN_TYPE_CHECKBOX_ENABLED_CHECKED;
         }
         else
         {
            skType = Skin::SKIN_TYPE_CHECKBOX_ENABLED;
         }
      }
      else
      {
         if(checked)
         {
            skType = Skin::SKIN_TYPE_CHECKBOX_DISABLED_CHECKED;
         }
         else
         {
            skType = Skin::SKIN_TYPE_CHECKBOX_DISABLED;
         }
      }
      /* Draw the checkbox */
      skin->drawElement(surface, skType, rx1, ry1, rx2, ry2);
      Rect bounds = skin->getSkinElement(skType).getBounds(1,1);
      label->setPosition(bounds.getX2() + 1, 0);
      label->setSize(getWidth() - bounds.getX2() - 2, 20);
   }
   else
   {
      Farso::Draw* draw = Farso::Controller::getDraw();

      /* Draw checkbox border */
      draw->setActiveColor(Colors::colorCont[0]);
      draw->doTwoColorsRectangle(surface, rx1, ry1+3, rx1+13, ry1+16, 
                                 Colors::colorCont[1]);
      /* Fill checkbox interior */
      if(isAvailable())
      {
         draw->setActiveColor(Colors::colorText);
      }
      else
      {
         draw->setActiveColor(Colors::colorCont[2]);
      }
      draw->doFilledRectangle(surface, rx1+1, ry1+4, rx1+12, ry1+15);

      /* Draw its inner 'X' mark, if needed */
      if(checked)
      {
         if(isAvailable())
         {
            draw->setActiveColor(Colors::colorBar);
         }
         else
         {
            draw->setActiveColor(Colors::colorCont[1]);
         }
         draw->doLine(surface, rx1+2, ry1+5, rx1+11, ry1+14);
         draw->doLine(surface, rx1+1, ry1+5, rx1+10, ry1+14);
         draw->doLine(surface, rx1+3, ry1+5, rx1+12, ry1+14);
         draw->doLine(surface, rx1+11, ry1+5, rx1+2, ry1+14);
         draw->doLine(surface, rx1+10, ry1+5, rx1+1, ry1+14);
         draw->doLine(surface, rx1+12, ry1+5, rx1+3, ry1+14);
      }
      label->setPosition(15, 0);
      label->setSize(getWidth() - 15, 20);
   }
}

/******************************************************************
 *                             doTreat                            *
 ******************************************************************/
bool CheckBox::doTreat(bool leftButtonPressed, bool rightButtonPressed,
      int mouseX, int mouseY, int mrX, int mrY)
{
   if(leftButtonPressed)
   {
      /* Let's see if started to press or just continued. */
      if((!pressStarted) && (isInner(mrX, mrY)))
      {
         pressStarted = true;
         Controller::setEvent(this, EVENT_CHECKBOX_PRESSING);
         return true;
      } 
      else if(pressStarted)
      {
         Controller::setEvent(this, EVENT_CHECKBOX_PRESSING);
         return true;
      }
   }
   else if(pressStarted)
   {
      pressStarted = false;
      if(isInner(mrX, mrY))
      {
         toggle();
         return true;
      }
   }

   return false;
}

/******************************************************************
 *                           doAfterChildTreat                    *
 ******************************************************************/
void CheckBox::doAfterChildTreat()
{
   /* No need to check it's label clicks, as will check the whole
    * body area. */
}

/******************************************************************
 *                              toggle                            *
 ******************************************************************/
void CheckBox::toggle()
{
   if(checked)
   {
      uncheck();
      Controller::setEvent(this, EVENT_CHECKBOX_UNCHECKED);
   }
   else
   {
      check();
      Controller::setEvent(this, EVENT_CHECKBOX_CHECKED);
   }
}

/******************************************************************
 *                            getBody                             *
 ******************************************************************/
const Farso::Rect& CheckBox::getBody()
{
   return body;
}


