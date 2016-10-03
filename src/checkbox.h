/* 
  Farso: a simple GUI.
  Copyright (C) DNTeam <dnt@dnteam.org>
 
  This file is part of Farso.
 
  Farso is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Farso is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Farso.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _farso_checkbox_h
#define _farso_checkbox_h

#include "widget.h"
#include "label.h"

namespace Farso
{

/*! Widget that mark a boolean operation: checked or not. */
class CheckBox : public Widget
{
   public:
      /*! Constructor
       * \param x left coordinate on parent's body
       * \param y top coordinate on parent's body
       * \param width width of the check box + its label
       * \param label string with checkbox's label
       * \param checked if start checked or not
       * \param parent pointer to parent, if any */
      CheckBox(int x, int y, int width, Kobold::String label, bool checked, 
            Widget* parent);
      /*! Destructor */
      ~CheckBox();

      /*! Check the box.
       * \note This will not raise any event. */
      void check();
      /*! Uncheck the box.
       * \note This will not raise any event. */
      void uncheck();
      /*! \return if is checked or not */
      bool isChecked();
      
      /*! \return checkbox body (in this case, its full area) */
      Rect getBody();

   protected:
      /*! Draw the checkbox on parent's body */
      void doDraw(Rect pBody);
      /*! Check for clicks on itself */
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed,
            int mouseX, int mouseY, int mrX, int mrY);
      /*! Check for clicks on its label */
      void doAfterChildTreat();
      /*! Toggle check/unchecked, based on current state, raising an event */
      void toggle();

   private:
      Rect body;         /**< Body of check box. */
      bool checked;      /**< If actually checked or not */
      bool pressStarted; /**< If press actually started */
      Label* label;      /**< Checkbox label */

};

}

#endif

