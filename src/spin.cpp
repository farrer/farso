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

#include "spin.h"
#include "controller.h"

#include <sstream>

#include <assert.h>

using namespace Farso;

#define FARSO_SPIN_HEIGHT        21
#define FARSO_SPIN_WIDTH        100
#define FARSO_SPIN_BUTTON_WIDTH  20
#define FARSO_SPIN_LABEL_WIDTH (FARSO_SPIN_WIDTH - (2*FARSO_SPIN_BUTTON_WIDTH))
#define FARSO_SPIN_RANGE_DIV    10.0f

#define FARSO_SPIN_TIME_INC_VALUE_MS  50  
#define FARSO_SPIN_TIME_INC_DELTA_MS  200

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
Spin::Spin(ValueType type, int x, int y, Widget* parent)
     :Widget(Widget::WIDGET_TYPE_SPIN, x, y, FARSO_SPIN_WIDTH, 
             FARSO_SPIN_HEIGHT, parent)
{
   this->type = type;
   this->value = 0.0f;
   this->delta = 1.0f;
   this->curDelta = this->delta;
   this->range[0] = -40.000f;
   this->range[1] = 40.000f;
   this->accelerate = true;

   this->body.set(getX(), getY(), getX() + getWidth() - 1, getY() +
            getHeight() - 1);

   this->buttonPrevious = new Farso::Button(0, 0, FARSO_SPIN_BUTTON_WIDTH, 
         FARSO_SPIN_HEIGHT, BUTTON_SPECIAL_CAPTION_LEFT, this);
   this->currentLabel =  new Farso::Label(FARSO_SPIN_BUTTON_WIDTH, 0, 
         FARSO_SPIN_LABEL_WIDTH, FARSO_SPIN_HEIGHT, "0", this);
   this->currentLabel->enableBorder();
   this->currentLabel->setFontAlignment(Font::TEXT_CENTERED);
   this->buttonNext = new Farso::Button(FARSO_SPIN_LABEL_WIDTH + 
         FARSO_SPIN_BUTTON_WIDTH, 0, FARSO_SPIN_BUTTON_WIDTH,
         FARSO_SPIN_HEIGHT, BUTTON_SPECIAL_CAPTION_RIGHT, this);

   defineMaxDelta();
   setValueText();

   this->incTimer.reset();
   this->incDeltaTimer.reset();
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Spin::~Spin()
{
}

/***********************************************************************
 *                             setValueText                            *
 ***********************************************************************/
void Spin::setValueText()
{
   std::stringstream ss;
   if(type == VALUE_TYPE_INTEGER)
   {
      ss << static_cast<int>(value);
   }
   else
   {
      ss << value;
   }
   currentLabel->setCaption(ss.str());
}

/***********************************************************************
 *                            defineMaxDelta                           *
 ***********************************************************************/
void Spin::defineMaxDelta()
{
   maxDelta = (range[1] - range[0]) / FARSO_SPIN_RANGE_DIV;

   /* Check if no lesser than a unit when using integer values */
   if((type == VALUE_TYPE_INTEGER) && (maxDelta < 1.0f))
   {
      maxDelta = 1.0f;
   }
   /* Check if maxDelta is no lesser than current defined delta */
   if(maxDelta < delta)
   {
      maxDelta = delta;
   }
}

/***********************************************************************
 *                             setFloatDelta                           *
 ***********************************************************************/
void Spin::setFloatDelta(float delta, bool accelerate)
{
   assert(type == VALUE_TYPE_FLOAT);
   
   this->delta = delta;
   this->curDelta = delta;
   this->accelerate = accelerate;
   defineMaxDelta();
}

/***********************************************************************
 *                             setIntDelta                             *
 ***********************************************************************/
void Spin::setIntDelta(int delta, bool accelerate)
{
   assert(type == VALUE_TYPE_INTEGER);
   
   this->delta = delta;
   this->curDelta = delta;
   this->accelerate = accelerate;
   defineMaxDelta();

}

/***********************************************************************
 *                               setRange                              *
 ***********************************************************************/
void Spin::setRange(float min, float max)
{
   range[0] = min;
   range[1] = max;
}

/***********************************************************************
 *                               setValue                              *
 ***********************************************************************/
void Spin::setValue(float value)
{
   this->value = value;
   setValueText();
}

/***********************************************************************
 *                              getIntValue                            *
 ***********************************************************************/
int Spin::getIntValue()
{
   return static_cast<int>(value);
}

/***********************************************************************
 *                            getFloatValue                            *
 ***********************************************************************/
float Spin::getFloatValue()
{
   return value;
}

/***********************************************************************
 *                                getBody                              *
 ***********************************************************************/
Farso::Rect Spin::getBody()
{
   return body;
}

/***********************************************************************
 *                                 enable                              *
 ***********************************************************************/
void Spin::enable()
{
   buttonNext->enable();
   buttonPrevious->enable();
   currentLabel->enable();
   Widget::enable();
}

/***********************************************************************
 *                                disable                              *
 ***********************************************************************/
void Spin::disable()
{
   buttonNext->disable();
   buttonPrevious->disable();
   currentLabel->disable();
   Widget::disable();
}

/***********************************************************************
 *                                setDirty                             *
 ***********************************************************************/
void Spin::setDirty()
{
   if(getParent())
   {
      getParent()->setDirty();
   }
   Widget::setDirty();
}

/***********************************************************************
 *                                 doDraw                              *
 ***********************************************************************/
void Spin::doDraw(Rect pBody)
{
   this->body.set(getX(), getY(), getX() + getWidth() - 1, getY() +
         getHeight() - 1);
}

/***********************************************************************
 *                                doTreat                              *
 ***********************************************************************/
bool Spin::doTreat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY, int mrX, int mrY)
{
   return false;
}

/***********************************************************************
 *                            checkDeltaTimer                          *
 ***********************************************************************/
void Spin::checkDeltaTimer()
{
   /* Check if it's time to increment delta value */
   if((accelerate) && 
      (incDeltaTimer.getMilliseconds() >= FARSO_SPIN_TIME_INC_DELTA_MS))
   {
      incDeltaTimer.reset();
      curDelta += delta;
      if(curDelta > maxDelta)
      {
         curDelta = maxDelta;
      }
   }
}

/***********************************************************************
 *                          doAfterTreatTreat                          *
 ***********************************************************************/
void Spin::doAfterChildTreat()
{
   Event event = Controller::getLastEvent();
  
   /* Check if user is pressing our buttons */
   if(event.getType() == EVENT_BUTTON_PRESSING)
   {
      if(incTimer.getMilliseconds() >= FARSO_SPIN_TIME_INC_VALUE_MS)
      {
         /* It's time to incremente/decrement value */
         incTimer.reset();

         if(event.getWidget() == buttonNext)
         {
            /* Try to increment value (at range) */
            value += curDelta;
            if(value > range[1])
            {
               value = range[1];
            }
            setValueText();
            /* Set event and check if need to increment delta */
            Controller::setEvent(this, EVENT_SPIN_CHANGED);
            checkDeltaTimer();
         }
         else if(event.getWidget() == buttonPrevious)
         {
            /* Try to decrement value (at range) */
            value -= curDelta;
            if(value < range[0])
            {
               value = range[0];
            }
            setValueText();
            Controller::setEvent(this, EVENT_SPIN_CHANGED);
            checkDeltaTimer();
         }
      }
   }
   else
   {
      /* Not pressing, should set delta to its default */
      curDelta = delta;
   }
}


