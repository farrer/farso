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

#ifndef _farso_scroll_text_h
#define _farso_scroll_text_h

#include "widget.h"
#include "scrollbar.h"

namespace Farso
{

/*! Widget to display lines scrollable text */
class ScrollText : public Widget
{
   public:
      ScrollText(int x, int y, int width, int height);
      ~ScrollText();

      void setText(Kobold::String text);

      void addText(Kobold::String text);

      void addBreakLine();

   protected:

   private:

      /*! A single text sentence. The sentence is a group of text at the same 
       * line which have the same font attributes */
      class TextSentence : public Kobold::ListElement
      {
         public:
            Kobold::String text;     /**< Text of the sentence */
            Kobold::String fontName; /**< Filename of the font to use */
            int fontSize;   /**< Font size to use for the setence */
            Font::Alignment fontAlignment; /**< Alignment to use for sentence */
            int height;  /**< Current height of the sentence */
            Color color; /**< Text color */
      };

      /*! The line is the representation of a single line of text, implemented
       * as a list of TextSentences. */
      class TextLine : public Kobold::ListElement, public Kobold::List
      {
         public:
            bool isEmptyLine();

            int height;
      };

      Farso::ScrollBar scrollBar; /**< The scroller */

      Kobold::List lines; /**< Current text lines */
};

}

#endif


