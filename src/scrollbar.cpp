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

#include "scrollbar.h"
#include "controller.h"
#include "event.h"
#include "math.h"
using namespace Farso;

#define DEFAULT_SIZE  20
#define UNDEFINED_REFERENCE 200000

/************************************************************************
 *                            Constructor                               *
 ************************************************************************/
ScrollBar::ScrollBar(ScrollType type, int x, int y, int widthOrHeight, 
      Widget* parent)
          :Widget(WIDGET_TYPE_SCROLL_BAR, x, y, 
                  (type == TYPE_HORIZONTAL) ? widthOrHeight : DEFAULT_SIZE,
                  (type == TYPE_VERTICAL) ? widthOrHeight : DEFAULT_SIZE,
                  parent)
{
   this->scrollType = type;
   this->initial = 0;
   this->total = 0;
   this->maxDisplayed = 0;
   this->scrollButtonPressing = false;
   this->scrollReference[0] = UNDEFINED_REFERENCE;
   this->scrollReference[1] = UNDEFINED_REFERENCE;
   this->widthOrHeight = widthOrHeight;
   this->body.set(0, 0, getWidth() - 1, getHeight() - 1);
   this->realWidthOrHeight = (widthOrHeight - (2 * DEFAULT_SIZE) - 3);

   /* Let's create and define its buttons */
   int rx  = getX(), ry = getY();
   if(type == TYPE_VERTICAL)
   {
      upButton = new Farso::Button(rx, ry, DEFAULT_SIZE, DEFAULT_SIZE, 
            BUTTON_SPECIAL_CAPTION_UP, this);
      scrollButton = new Farso::Button(rx + 1, ry + DEFAULT_SIZE + 1, 
            DEFAULT_SIZE - 2, widthOrHeight - (2 * DEFAULT_SIZE) - 3, "",
            this);
      downButton = new Farso::Button(rx, ry + widthOrHeight - DEFAULT_SIZE - 1,
            DEFAULT_SIZE, DEFAULT_SIZE, BUTTON_SPECIAL_CAPTION_DOWN, this);
   }
   else
   {
      upButton = new Farso::Button(rx, ry, DEFAULT_SIZE, DEFAULT_SIZE, 
            BUTTON_SPECIAL_CAPTION_LEFT, this);
      scrollButton = new Farso::Button(rx + DEFAULT_SIZE + 1, ry + 1, 
            widthOrHeight - (2 * DEFAULT_SIZE) - 3, DEFAULT_SIZE - 2, 
            "", this);
      downButton = new Farso::Button(rx + widthOrHeight - DEFAULT_SIZE - 1, ry,
            DEFAULT_SIZE, DEFAULT_SIZE, BUTTON_SPECIAL_CAPTION_RIGHT, this);
   }
}

/************************************************************************
 *                             Destructor                               *
 ************************************************************************/
ScrollBar::~ScrollBar()
{
   /* Note that our buttons are deleted on list destructor. */
}

/************************************************************************
 *                         definePositionAndSize                        *
 ************************************************************************/
void ScrollBar::definePositionAndSize()
{
   /* Define coordinates */
   int rx  = getX(), ry = getY();
   
   /* Calculate variation for each position */
   realWidthOrHeight = (widthOrHeight - (2 * DEFAULT_SIZE) - 3);
   float deltaInit = (initial) / (float)total;
   float deltaEnd = (initial + maxDisplayed) / (float)total;

   /* Make sure variations are lesser than 1 (could be greater in case
    * that the ammount displayed (total) is lesser than the maxDisplayed) */
   if(deltaInit > 1)
   {
      deltaInit = 1.0f;
   }
   if(deltaEnd > 1)
   {
      deltaEnd = 1.0f;
   }

   /* Define initial position (make sure will never go after down button) */
   int posInit = (int) ceil(deltaInit * realWidthOrHeight);
   if(posInit > realWidthOrHeight - DEFAULT_SIZE + 2)
   {
      posInit = realWidthOrHeight - DEFAULT_SIZE + 2;
   }

   /* Define Size (width or height), making sure to mantain a minimun size. */
   int size = (int) ceil((deltaEnd) * realWidthOrHeight) - posInit;
   if(size < DEFAULT_SIZE - 2)
   {
      size = DEFAULT_SIZE - 2;
   }
   
   /* Finally, set the scroll possition and size, depending on its type */
   if(scrollType == TYPE_VERTICAL)
   {
      scrollButton->setPosition(rx + 1, ry + DEFAULT_SIZE + 1 + posInit);
      scrollButton->setSize(DEFAULT_SIZE - 2, size); 
   }
   else
   {
      scrollButton->setPosition(rx + DEFAULT_SIZE + 1 + posInit, ry + 1);
      scrollButton->setSize(size, DEFAULT_SIZE - 2);
   }

   /* As we have no background for scroll button, we must dirty our parent */
   if(getParent() != NULL)
   {
      getParent()->setDirty();
   }
}

/************************************************************************
 *                              setTotals                               *
 ************************************************************************/
void ScrollBar::setTotals(int maxDisplayed, int total)
{
   this->initial = 0;
   this->maxDisplayed = maxDisplayed;
   this->total = total;

   definePositionAndSize();
}

/************************************************************************
 *                              getCurrent                              *
 ************************************************************************/
int ScrollBar::getCurrent()
{
   return initial;
}

/************************************************************************
 *                               getBody                                *
 ************************************************************************/
Rect ScrollBar::getBody()
{
   return body;
}

/************************************************************************
 *                              doDraw                                  *
 ************************************************************************/
void ScrollBar::doDraw(Rect pBody)
{
}

/************************************************************************
 *                              doTreat                                 *
 ************************************************************************/
bool ScrollBar::doTreat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY, int mrX, int mrY)
{
   if(scrollButtonPressing)
   {
      if(!leftButtonPressed)
      {
         /* No more scrollButtonPressing. will abort the procedure
          * on afterChildTreat. */
         return false;
      }

      if((scrollReference[0] != UNDEFINED_REFERENCE) &&
         (scrollReference[1] != UNDEFINED_REFERENCE))
      {
         /* Let's recalculate initial, based on reference (and type, for which
          * reference to use) */
         int delta;
         if(scrollType == TYPE_VERTICAL)
         {
            /* Use Y */
            delta = mrY - scrollReference[1];
         }
         else
         {
            /* Use X */
            delta = mrX - scrollReference[0];
         }
         int varInitial = (delta / realWidthOrHeight) * total;

         if(varInitial != 0)
         {
            initial += varInitial;
            if(initial < 0)
            {
               initial = 0;
            }
            else if(initial > total - maxDisplayed - 1)
            {
               initial = total - maxDisplayed - 1;
            }
            Controller::setEvent(this, EVENT_SCROLLBAR_CHANGED);
            definePositionAndSize();
         }
         else
         {
            Controller::setEvent(this, EVENT_SCROLLBAR_UNCHANGED);
         }
      }
      /* Let's define the new reference. */
      scrollReference[0] = mrX;
      scrollReference[1] = mrY;

      return true;
   }

   return false;
}

/************************************************************************
 *                           doAfterChildTreat                          *
 ************************************************************************/
void ScrollBar::doAfterChildTreat()
{
   Event ev = Controller::getLastEvent();

   if(ev.getWidget() == this)
   {
      /* event already happened on the scroll bar, let's pass. */
      return;
   }

   /* Let's check each button press */
   scrollButtonPressing = false;
   if(ev.getWidget() == scrollButton)
   {
      if(ev.getType() == EVENT_BUTTON_PRESS_INIT)
      {
         scrollButtonPressing = true;
         scrollReference[0] = UNDEFINED_REFERENCE;
         scrollReference[1] = UNDEFINED_REFERENCE;
      }
      else
      {
         /* Note: as press started, we just not moving it after canceled or
          * release. So, even if mouse is no more inside the button we still
          * move the scroll. */
         scrollButtonPressing = (ev.getType() == EVENT_BUTTON_PRESSING) ||
                   (ev.getType() == EVENT_BUTTON_NOT_PRESSING);
      }
   }
   else if(ev.getType() == EVENT_BUTTON_PRESSING)
   {
      if(ev.getWidget() == upButton)
      {
         /* Let's try to move the scroll up (or left) */
         if(initial > 0)
         {
            initial--;
            definePositionAndSize();
            Controller::setEvent(this, EVENT_SCROLLBAR_CHANGED);
         }
      }
      else if(ev.getWidget() == downButton)
      {
         /* Let's try to move the scroll down (or right) */
         if(initial + maxDisplayed < total - 1)
         {
            initial++;
            definePositionAndSize();
            Controller::setEvent(this, EVENT_SCROLLBAR_CHANGED);
         }
      }
   }
}

