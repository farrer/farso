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

#ifndef _farso_scroll_bar_h
#define _farso_scroll_bar_h

#include "widget.h"
#include "button.h"

namespace Farso
{

/*! A bar for scrolling elements. Usually used with other widgets to achive
 * some scrolling effect.  */
class ScrollBar : public Widget
{
   public:
      enum ScrollType
      {
         /*! Scroll bar draw around its Y axis */
         TYPE_VERTICAL,
         /*! Scroll bar around its X axis */
         TYPE_HORIZONTAL
      };

      /*! Constructor.
       * \param type if is a vertical or horizontal scroll 
       * \param x coordinate on parent
       * \param y coordinate on parent
       * \param widthOrHeight desired width (if horizontal scroll) or 
       *                      height (if vertical scroll) of the ScrollBar.
       * \param parent pointer to its parent */
      ScrollBar(ScrollType type, int x, int y, int widthOrHeight, 
            Widget* parent);
      /*! Destructor */
      ~ScrollBar();

      /*! Set the totals information for the scroll bar.
       * \param maxDisplayed max number of displayed elements at a single time
       * \param total current total number of elements to be displayed. */
      void setTotals(int maxDisplayed, int total);

      /*! \return index of the first displayed element [0, total - 1] */
      int getCurrent();
      
      Rect getBody();

   protected:

      void doDraw(Rect pBody);
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed, 
                   int mouseX, int mouseY, int mrX, int mrY);
      void doAfterChildTreat();

      void definePositionAndSize();

   private:
      Farso::Button* upButton;
      Farso::Button* downButton;
      Farso::Button* scrollButton;

      ScrollType scrollType;

      Rect body;

      int widthOrHeight;
      float realWidthOrHeight;
      int initial;
      int maxDisplayed;
      int total;

      bool scrollButtonPressing;
      int scrollReference[2];

};

}

#endif


