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

#ifndef _farso_text_selector_h
#define _farso_text_selector_h

#include "widget.h"
#include "label.h"
#include "rect.h"
#include <kobold/list.h>
#include <kobold/kstring.h>

namespace Farso
{

/*! The text selector is a widget that display some text options enumerated 
 * to be selected by the user. Usually used for dialog options on RPG games,
 * for example. */
class TextSelector : public Widget
{
   public:
      /*! Constructor. The Text Selector will always take full parent's body
       * to itself.
       * \param parent pointer to parent. Must not be NULL. */
      TextSelector(Widget* parent);
      /*! Destructor */
      ~TextSelector();

      /*! Add an option that can be selected by the user */
      void addOption(Kobold::String text);

      /*! \return index [0, max-1] of the selected option, if any.
       * If no option was selected, will return a value < 0 */
      int getSelectedOption();
      
      Rect getBody();

      /*! Unselect any selected option */
      void unselect();

      /*! Force selection of an option 
       * \param option to select, range [0, max-1] */
      void forceSelection(int option);

   protected:
      void doDraw(Rect pBody);
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY, int mrX, int mrY);
      void doAfterChildTreat();

      /*! Set the label to be of unselected style */
      void setUnselectedStyle(Label* label);
      /*! Set the label to be of unselected style */
      void setSelectedStyle(Label* label);


   private:
      /*! Each option definition */
      class TextOption : public Kobold::ListElement
      {
         public:
            TextOption(int x, int y, int width, int height, 
                       Kobold::String caption, Widget* parent);
            ~TextOption();

            Label* label;
      };

      Kobold::List options; /**< List of options to select */
      int selected; /**< Index of current selected option */
      int over; /**< Index of current on over option */
      TextOption* selectedOption; /**< Current option selected */
      Rect body; /**< Options body */
};

}

#endif

