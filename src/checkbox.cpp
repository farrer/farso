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
CheckBox::CheckBox(int x, int y, int width, Kobold::String label, bool checked, 
                   Widget* parent)
         :Widget(Widget::WIDGET_TYPE_CHECK_BOX, x, y, width, 20, parent)
{
   this->setCaption(label);
   this->checked = checked;
   this->pressStarted = false;
   this->body.set(getX() + 20, getY(), getX() + width - 1, getY() + 22);

   /* Note: let's make the label a child of the same parent (instead of
    * a child of the checkBox. It's just to avoid rerendering the label
    * on check box togle states (as the toggle will only affect the
    * the box draw, not the label). The afterTreatChild call will
    * be than uneffective, but here will check the whole check box area
    * (which includes the label), creating events, and the checkbox is 
    * always before the label on parent's children list, so, once the click
    * is got here, it won't check the label's one after.
    * The downside is that we must assure to set label's position 
    * always right of the 'box', which is not so trivial due to containers. */
   this->label = new Label(0, 0, width, 20, label, parent);
   setLabelPosition(15);
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
 *                         setLabelPosition                       *
 ******************************************************************/
void CheckBox::setLabelPosition(int labelX)
{
   Container* pCont = (Container*) getParentContainer();

   if((pCont == NULL) || 
      (pCont->getContainerType() == Container::TYPE_TOP_LEFT) ||
      (pCont->getContainerType() == Container::TYPE_BOTTOM_LEFT))
   {
      /* left oriented */
      label->setPosition(labelX + getXWithoutTransform(), 
            getYWithoutTransform());
   }
   else 
   {
      /* Centered oriented or right oriented*/
      label->setPosition(getXWithoutTransform() - labelX, 
            getYWithoutTransform());
   }
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
 *                               doDraw                           *
 ******************************************************************/
void CheckBox::doDraw(Rect pBody)
{
   Farso::Skin* skin = Farso::Controller::getSkin();
   Farso::Surface* surface = getWidgetRenderer()->getSurface();

   int rx1 = pBody.getX1() + getX();
   int ry1 = pBody.getY1() + getY();
   int rx2 = pBody.getX1() + getX() + getWidth() - 1;
   int ry2 = pBody.getY1() + getY() + getHeight() - 1;
   int labelX;

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
      Rect bounds = skin->getSkinElement(skType).getBounds(0,0);
      labelX = bounds.getX2() + 2;
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
      labelX = 15;
   }

   /* Redefine label position, due to potential on-the-fly skin change. */
   setLabelPosition(labelX);
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
Farso::Rect CheckBox::getBody()
{
   return body;
}


