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

#ifndef _farso_text_entry_h
#define _farso_text_entry_h

#include <kobold/platform.h>

#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID && \
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS

#include "widget.h"
#include "font.h"
#include <kobold/keyboard.h>

namespace Farso
{

/*! Widget to edit single line strings. */
class TextEntry : public Kobold::KeyboardTextEditor, public Widget
{
   public:
      
      /*! Constructor
       * \param x x position
       * \param y y position
       * \param width width of the text entry
       * \param height of the text entry
       * \param parent pointer to its parent, if any */
      TextEntry(int x, int y, int width, int height, Widget* parent);
      /*! Destructor */
      ~TextEntry();

      /* Functions from KeyboardTextEditor */
      void appendToCursor(const Kobold::String& textToAppend);
      void deleteCharacterFromCursor(bool after);
      void sendCursorToInit();
      void sendCursorToEnd();
      void changeCursorPosition(int delta);
      void doneWithEditing();

      /* Function from Widget */
      const Rect& getBody();
      void setCaption(const Kobold::String& text);

   protected:

      /* Functions from Widget */
      void doDraw(const Rect& pBody);
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY, int mrX, int mrY);
      void doAfterChildTreat(); 

   private:

      /*! Get the prepared font to use */
      Font* getFont();

      /*! Get index of next character start (utf8) */
      int getNextCharacterInit(const Kobold::String& str, int cur);
      /*! Get index one previous character start (utf8) */
      int getPreviousCharacterInit(const Kobold::String& str, int cur);
      /*! Get the number of characters of the text (utf8) */
      int getStringLength(const Kobold::String& s);

      /*! Calculate cursor position, based on relative mouse position */
      void calculateCursorPosition(int mrX, int mrY);

      Rect body;     /**< Current body */
      bool editing; /**< If is actually editing the text entry */
      bool shouldStopEdition; /**< If received to stop edition */
      unsigned int cursorIndex; /**< Current cursor index on the string */
      Kobold::String visibleText; /**< Current visible text */
      int curInit; /**< Current visible text first index on whole text */
      int curEnd; /**< Current visible text last index on whole text */
      bool redefineVisible; /**< Used to redefine or not visible text */
      int lastCheckX; /**< last relative mouse X position checked for cursor */

};

}

#endif

#endif


