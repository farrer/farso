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

#ifndef _farso_spin_h
#define _farso_spin_h

#include "widget.h"
#include "button.h"
#include "label.h"

#include <kobold/timer.h>

namespace Farso
{
   /*! Spin: widget to retrieve an integer or floating-point number from 
    * the user */
   class Spin : public Widget
   {
      public:
         /*! Type of the value of a spin */
         enum ValueType 
         {
            VALUE_TYPE_INTEGER,
            VALUE_TYPE_FLOAT
         };

         /*! Constructor
          * \param type ValueType of the spin's value
          * \param x X coordinate
          * \param y Y coordinate 
          * \param parent pointer to its parent (if any)
          * \note the default range is -40000 to 40000. */
         Spin(ValueType type, int x, int y, Widget* parent);

         /*! Destructor */
         ~Spin();

         /*! Set current delta (ie: how much will change current value at
          * each press).
          * \note only valid for VALUE_TYPE_FLOAT 
          * \para delta new delta value
          * \param accelerate with delta will be greater when pressed for
          *        a 'long' time. */
         void setFloatDelta(float delta, bool accelerate=true);
         /*! Set current delta (ie: how much will change current value at
          * each press).
          * \note only valid for VALUE_TYPE_INTEGER 
          * \para delta new delta value
          * \param accelerate with delta will be greater when pressed for
          *        a 'long' time. */
         void setIntDelta(int delta, bool accelerate=true);

         /*! Set the valid range of values.
          * \param min minumum value
          * \param max maximum value */
         void setRange(float min, float max);

         /*! Set current spin value
          * \param value new value
          * \note on VALUE_TYPE_INTEGER, will ignore fraction part */
         void setValue(float value);
         /*! \return current value */
         int getIntValue();
         /*! \return current value */
         float getFloatValue();

         /*! \return current body */
         const Rect& getBody();
         void enable();
         void disable();
         void setDirty();

      protected:

         void doDraw(const Rect& pBody);
         bool doTreat(bool leftButtonPressed, bool rightButtonPressed, 
               int mouseX, int mouseY, int mrX, int mrY);
         void doAfterChildTreat();

      private:
         /*! Set current label caption based on current value */
         void setValueText();
         /*! Define max delta value */
         void defineMaxDelta();
         /*! Check if is delta increment time */
         void checkDeltaTimer();

         Button* buttonNext;     /**< Button to increment value */
         Button* buttonPrevious; /**< Button to decrement value */
         Label* currentLabel;    /**< Current value display */

         ValueType type; /**< Type of thevalue to spin */
         
         float value; /**< Current value */
         float delta; /**< Defined delta */
         float curDelta; /**< Current delta */
         float maxDelta; /**< Max delta value */
         float range[2]; /**< Current range */
         bool accelerate; /**< If delta will be greater after some press */

         Rect body; /**< Current body */

         Kobold::Timer incTimer;   /**< Timer to increment value */
         Kobold::Timer incDeltaTimer; /**< Timer to increment delta */
   };

}

#endif

