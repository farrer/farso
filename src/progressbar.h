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

#ifndef _farso_progress_bar_h
#define _farso_progress_bar_h

#include "widget.h"
#include "rect.h"

namespace Farso
{

class ProgressBar : public Widget
{
   public:
      ProgressBar(int x, int y, int width, int height, Widget* parent);
      ~ProgressBar();

      /*! Set current max value (default: 100). */
      void setMaxValue(int max);
      /*! Set current value (default: 0). */
      void setValue(int val);

      void setColor(Color color);

      Rect getBody();

   protected:

      void doDraw(Rect pBody);
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed,
                  int mouseX, int mouseY, int mrX, int mrY);
      void doAfterChildTreat();

   private:
      int maxValue; /**< Max value */
      int curValue; /**< Current value */
      Color color;  /**< Color of the current value filled rectangle */
      Rect body; /**< Current body */
};

}

#endif

