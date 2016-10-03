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

#ifndef _farso_button_h
#define _farso_button_h

#include "widget.h"
#include "skin.h"

namespace Farso
{

#define BUTTON_SPECIAL_CAPTION_LEFT    "@dnt_left"
#define BUTTON_SPECIAL_CAPTION_RIGHT   "@dnt_right"
#define BUTTON_SPECIAL_CAPTION_UP      "@dnt_up"
#define BUTTON_SPECIAL_CAPTION_DOWN    "@dnt_down"

/*! Clickable widget implementation: a button. */
class Button: public Widget
{
   public:
      /*! Constructor
       * \param x coordinate on parent
       * \param y coordinate on parent
       * \param width button width
       * \param height button height
       * \param parent pointer to button's parent (if any). */
      Button(int x, int y1, int width, int height, Kobold::String caption, 
            Widget* parent);
      /*! Destructor */
      ~Button();

      /*! Set the button to be with rounded edges (instead of the default
       * rectangle).
       * \note this has no effect when using a skin. */
      void setRoundedEdges();

      /*! Set the font to be used on menu.
       * \param fontName filename of the font to be used
       * \param fontSize size of the font to use. */
      void setFont(Kobold::String fontName, int fontSize);

      /*! Override skin types for current button.
       * \note This is only effective when actually using a skin. */
      void setSkinTypes(Skin::SkinElementType enabledType, 
                        Skin::SkinElementType pressedType,
                        Skin::SkinElementType onOverType,
                        Skin::SkinElementType disabledType);
      
      Rect getBody();

   protected:
      /*! Draw the button */
      void doDraw(Rect pBody);
      /*! Button treat events */
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY, int mrX, int mrY);
      void doAfterChildTreat();

      void press();
      void release();

   private:
      bool useRoundedEdges; /**< If will use rounded edges or 90 degrees. */
      bool pressStarted;    /**< If is currently on-press procedure. This
                                 not means it's actually pressed, as the 
                                 cursor (mouse or finger) could go out
                                 button's area and released it while still
                                 on press procedure. */

      bool pressed;          /**< If button is actually pressed. */
      Kobold::String fontName; /**< Filename of the font to use. */
      int fontSize;          /**< Size of the font to use. */
      Rect body;             /**< Button's body */

      Skin::SkinElementType enabledType;  /**< Type to BUTTON_ENABLED */
      Skin::SkinElementType pressedType;  /**< Type to BUTTON_PRESSED */
      Skin::SkinElementType onOverType;   /**< Type to BUTTON_ON_OVER */
      Skin::SkinElementType disabledType; /**< Type to BUTTON_DISABLED */
};

}


#endif

