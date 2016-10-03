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

#ifndef _farso_label_h
#define _farso_label_h

#include "widget.h"
#include "font.h"
#include "skin.h"

namespace Farso
{

/*! Label widget implementation. A label is a simple box with a small amount
 * of text inside. */
class Label: public Widget
{
   public:
      /*! Constructor 
       * \param x coordinate
       * \param y coordinate
       * \param width label's width
       * \param height label's height
       * \param caption text caption to be at label.
       * \param parent pointer to parent, if any. */
      Label(int x, int y, int width, int height, Kobold::String caption, 
            Widget* parent);
      /*! Destructor */
      ~Label();

      /*! Override skin font for current label.
       * \note This is only effective when actually using a skin. */
      void setSkinType(Skin::SkinElementType type); 

      /*! Set a font to use, instead of the default's one.
       * \param fontName font's filename */
      void setFont(Kobold::String fontName);
      /*! Set size of the font to use, instead of default.
       * \param size font's size. */
      void setFontSize(int size);
      /*! Set color of the text, instead of default.
       * \param color color to use for text. */
      void setFontColor(Farso::Color color);
      /*! Set font alignment of the text, instead of the default TEXT_LEFT */
      void setFontAlignment(Farso::Font::Alignment align);

      /*! \return pointer to the used font of this element */
      Font* getFont();
      /*! \return font size to use */
      int getFontSize();


      Farso::Rect getBody();

   protected:
      
      void doDraw(Rect pBody);
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY, int mrX, int mrY);
      void doAfterChildTreat();

   private:
      Kobold::String fontName;    /**< Font to use. If Empty, use default. */
      int fontSize;             /**< Font size to use. -1 for default. */
      Farso::Color fontColor;   /**< Color of the font to use. */
      Farso::Rect body;         /**< Label's body */
      Farso::Font::Alignment fontAlign; /**< Alignment to use. */
      bool definedColor;        /**< If color is defined */
      bool pressStarted;        /**< Flag for press control */
      Skin::SkinElementType skinType; /**< Type of label to use with skins */
};

}

#endif

