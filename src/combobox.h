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

#ifndef _farso_combo_box_h
#define _farso_combo_box_h

#include <kobold/platform.h>
#include "widget.h"
#include "button.h"
#include "menu.h"

#include <vector>

namespace Farso
{
   /*! A simple textual combo box widget */
   class ComboBox : public Widget
   {
      public:
         /*! Constructor
          * \param x X coordinate on parent
          * \param y Y coordinate on parent
          * \param width its width
          * \param height its height
          * \param options vector with options to add
          * \param parent parent widget or NULL */
         ComboBox(int x, int y, int width, int height, 
               std::vector<Kobold::String> options, Widget* parent);
         /*! Destructor */
         ~ComboBox();
         
         /*! Set selected option caption 
          * \note it won't check if the option actually exists */
         void setCaption(const Kobold::String& text);

         /*! Set current size */
         void setSize(int width, int height);

         /*! @copydoc Widget::getBody */
         const Rect& getBody() { return body; };
         
         /*! @copydoc Widget::doDraw */
         void doDraw(const Rect& pBody);

         /*! @copydoc Widget::doTreat */
         bool doTreat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY, int mrX, int mrY);
         
         /*! @copydoc Widget::doAfterChildTreat */
         void doAfterChildTreat();

         /*! \return selected combo item, if any */
         const Menu::MenuItem* getSelected() { return selected; };

      private:
         Rect body; /**< Its body */
         Button* button; /**< Button with combo text. Used to open the menu */
         Menu* menu; /**< The selection options */
         const Menu::MenuItem* selected; /**< Pointer to the selected option */
   };

}

#endif

