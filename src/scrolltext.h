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

#ifndef _farso_scroll_text_h
#define _farso_scroll_text_h

#include "widget.h"
#include "container.h"
#include "font.h"
#include "scrollbar.h"

namespace Farso
{

/*! Widget to display lines scrollable text */
class ScrollText : public Widget
{
   public:
      /*!  Constructor, using default font from current skin. 
       * \note only use this constructor if using a Skin. Otherwise, 
       *       the full constructor should be used.*/
      ScrollText(int x, int y, int width, int height, Widget* parent);
      /*! Constructor with default font definition */
      ScrollText(int x, int y, int width, int height,
                 Kobold::String defaultFont, int defaultFontSize,
                 Color defaultColor, Widget* parent);
      /*! Destructor */
      ~ScrollText();

      /*! Clear current text. */
      void clear();

      /*! Hide the scroll bar and do not use it.
       * \note It won't affect existing text andshould be called on an empty
       *       ScrollText to it use the new full size */
      void hideScrollBar();

      /*! Reset the entire text of the ScrollText. It's equivalent to call
       * clear() and addText(text)
       * \param text new text of the ScrollText. */
      void setText(Kobold::String text);

      /*! Add text to the current text, with default font, default color 
       * and left alignment. 
       * \param text text to add*/
      void addText(Kobold::String text);
     
      /*! Add text to current text, specifying a font */
      void addText(Kobold::String text,  Kobold::String font, int size, 
            Font::Alignment align);

      /*! Add text to the current text, specifying a font and its color */
      void addText(Kobold::String text,  Kobold::String font, int size, 
            Font::Alignment align, Color color);
      
      /*! Add a left aligned text to the current text specifying its color */
      void addText(Kobold::String text, Color color);

      /*! Add a line break to the current text */
      void addLineBreak();
      
      /* Implementation from Widget */
      Rect getBody();

      /*! Set the scroll text dirty */
      void setDirty();

   protected:
      /* Implementations from Widget */
      void doDraw(Rect pBody);
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY, int mrX, int mrY);
      void doAfterChildTreat(); 

   private:
      /*! Init ScrollText definitions */
      void init(int width, int height);

      /*! A single text sentence. The sentence is a group of text at the same 
       * line which have the same font attributes */
      class TextSentence : public Kobold::ListElement
      {
         public:
            /*! Constructor */
            TextSentence(Kobold::String fontName, int fontSize, 
                         Color color);
            /*! Destructor */
            ~TextSentence();

            /*! Check if the following arguments are compatible with
             * the current sentence or if will needed to create another 
             * sentence. */
            bool isCompatible(Kobold::String fontName, int fontSize, 
                  Color color);

            /*! Add text to the current sentence */
            void addText(Kobold::String text, int width, int height);

            /*! \return current sentence width */
            int getWidth();
            /*! \return current sentence height */
            int getHeight();

            /*! Draw current sentence at x,y position on surface */
            void draw(int x, int y, Rect area, Farso::Surface* surface); 

         private:
            Kobold::String text;     /**< Text of the sentence */
            Kobold::String fontName; /**< Filename of the font to use */
            int fontSize;   /**< Font size to use for the setence */
            int height;  /**< Current height of the sentence */
            int width; /**< Current width of the sentence */
            Color color; /**< Text color */
      };

      /*! The line is the representation of a single line of text, implemented
       * as a list of TextSentences. */
      class TextLine : public Kobold::ListElement, public Kobold::List
      {
         public:
            /*! Constructor
             * \param defaultHeight default font default height. */
            TextLine(int defaultHeight);
            /*! Destructor */
            ~TextLine();

            /*! Set line alignment */
            void setAlignment(Font::Alignment alignment);

            /*! \return if alignment is compatible with curret font's one.
             * If line is empty, all alignments are compatible, but you
             * must setAlignment before add text */
            bool isCompatible(Font::Alignment alignment);
            
            /*! \return if this line is an empty one */
            bool isEmptyLine();

            /*! Add width and height to the line (from a sentence) */
            void add(int width, int height);

            /*! \return current line width */
            int getWidth();
            /*! \return current line height */
            int getHeight();

            /*! Draw current line at x,y position on surface */
            void draw(int x, int y, Rect area, Farso::Surface* surface); 

         private: 
            Font::Alignment alignment; /**< Alignment to use for line */
            int width;   /**< current width */
            int height;  /**< curren height */
      };

      /*! Get - or create - a text sentence to write to */
      TextSentence* getSentence(Kobold::String fontName, int fontSize, 
            Font::Alignment alignment, Color color);

      /*! Append a new text line, with one new empty sentence */
      TextLine* createLine(Kobold::String fontName, int fontSize,
            Font::Alignment alignment, Color color);

      Farso::ScrollBar* scrollBar; /**< The scroller */
      Rect body; /**< Body */

      Kobold::String defaultFont;   /**< Name of the default font to use */
      int defaultFontSize; /**< Size of the font to use as default */
      Color defaultColor; /**< Color to use as default */
      int defaultFontHeight; /**< Default heightof default font. Will be
                                  used as empty lines height */

      TextLine* firstDisplayedLine; /**< First line displayed on scroll */
      int firstDisplayed;  /**< Index of first displayed line */

      Kobold::List lines; /**< Current text lines */
};

}

#endif


