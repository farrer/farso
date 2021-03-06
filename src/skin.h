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

#ifndef _farso_skin_h
#define _farso_skin_h

#include <kobold/kstring.h>
#include <kobold/defparser.h>

#include "farsoconfig.h"
#include "font.h"
#include "rect.h"
#include "surface.h"
#include "widget.h"

namespace Farso
{

/*! A skin is an image atlas with elements to define the style of
 * Farso Widgets.
 * \note One could extend from this class, supporting own skin elements
 *       besides the basic defined ones, by reimplementing the function
 *       getTotalElements (only called once) and getExtendedElementType.
 *       Also, one should then set the skin by Controller::setSkin,
 *       loading it elsewhere with the new inherited class, instead of 
 *       the usual Controller::loadSkin call. */
class Skin
{
   public:
      /*! Element types for skin definitions */
      enum SkinElementType
      {
         SKIN_TYPE_UNKNOWN = -1,
         SKIN_TYPE_WINDOW,
         SKIN_TYPE_WINDOW_ACTIVE_TITLE_BAR,
         SKIN_TYPE_WINDOW_INACTIVE_TITLE_BAR,
         SKIN_TYPE_WINDOW_ENABLED_CLOSE_BUTTON,
         SKIN_TYPE_WINDOW_ON_OVER_CLOSE_BUTTON,
         SKIN_TYPE_WINDOW_PRESSED_CLOSE_BUTTON,
         SKIN_TYPE_WINDOW_DISABLED_CLOSE_BUTTON,
         SKIN_TYPE_BUTTON_ENABLED,
         SKIN_TYPE_BUTTON_PRESSED,
         SKIN_TYPE_BUTTON_ON_OVER,
         SKIN_TYPE_BUTTON_DISABLED,
         SKIN_TYPE_LABEL,
         SKIN_TYPE_LABEL_DISABLED,
         SKIN_TYPE_CHECKBOX_ENABLED,
         SKIN_TYPE_CHECKBOX_ENABLED_CHECKED,
         SKIN_TYPE_CHECKBOX_DISABLED,
         SKIN_TYPE_CHECKBOX_DISABLED_CHECKED,
         SKIN_TYPE_TEXT_OPTION,
         SKIN_TYPE_TEXT_OPTION_OVER,
         SKIN_TYPE_CAPTION_UP,
         SKIN_TYPE_CAPTION_DOWN,
         SKIN_TYPE_CAPTION_LEFT,
         SKIN_TYPE_CAPTION_RIGHT,
         SKIN_TYPE_PROGRESS_BAR,
         SKIN_TYPE_PROGRESS_BAR_FILL,
         SKIN_TYPE_BORDER_LEFT,
         SKIN_TYPE_BORDER_RIGHT,
         SKIN_TYPE_BORDER_TOP,
         SKIN_TYPE_BORDER_BOTTOM,
         SKIN_TYPE_TEXTENTRY,
         SKIN_TYPE_TEXTENTRY_DISABLED,
         SKIN_TYPE_CURSOR_TEXTUAL_TIP,
         SKIN_TYPE_GRID_FILL,
         SKIN_TYPE_GRID_BORDER,
         SKIN_TYPE_FILE_LABEL,
         SKIN_TYPE_DIRECTORY_LABEL,
         SKIN_TYPE_MENU,
         SKIN_TYPE_MENU_SEPARATOR,
         TOTAL_BASIC_SKIN_ELEMENT_TYPES
      };

      /*! Basic skin defintion for an element. A element is basicaly
       * a border, its corners and its background. Could have or not
       * some of those. */
      class SkinElement
      {
         public:
            /*! Constructor */
            SkinElement();
            /*! Destructor */
            ~SkinElement();

            /*! Set an element value based on its key */
            void set(const Kobold::String& key, int x1, int y1, int x2, int y2);

            /*! \return if this widget has a background definition */
            const bool hasBackground() const;

            /*! \return if this widget has a boder definition */
            const bool hasBorder() const;

            const Farso::Rect& getTopBorderDelta() const; 
            const Farso::Rect& getRightBorderDelta() const;
            const Farso::Rect& getLeftBorderDelta() const;
            const Farso::Rect& getBottomBorderDelta() const;
            const Farso::Rect& getBorderDelta() const { return borderDelta; };

            const Farso::Rect& getTopBorder() const { return topBorder; };
            const Farso::Rect& getLeftBorder() const { return leftBorder; };
            const Farso::Rect& getBottomBorder() const { return bottomBorder; };
            const Farso::Rect& getRightBorder() const { return rightBorder; };
            
            const Farso::Rect& getCornerDelta() const { return cornerDelta; };
            const Farso::Rect& getTopLeftCorner() const 
            { return topLeftCorner; };
            const Farso::Rect& getTopRightCorner() const 
            { return topRightCorner; };
            const Farso::Rect& getBottomLeftCorner() const 
            { return bottomLeftCorner; };
            const Farso::Rect& getBottomRightCorner() const 
            { return bottomRightCorner; };

            /*! \return if this widget has a corner definition */
            const bool hasCorner() const;

            /*! \return text area delta for text rendering on element */
            const Farso::Rect& getTextAreaDelta() const {return textAreaDelta;};
            /*! Set the name of the font to use for its texts */
            void setFontName(const Kobold::String& fontName);
            /*! \return font file name */
            const Kobold::String& getFontName() const;
            /*! Set font size to use for its texts */
            void setFontSize(int size);
            /*! \return font size to use */
            const int getFontSize() const;
            /*! Set the font alignment to use */
            void setFontAlignment(const Font::Alignment& align);
            /* \return font alignment to use */
            const Font::Alignment& getFontAlignment() const;
            /*! Set font color to use for element's text */
            void setFontColor(const Farso::Color& color);
            /*! Set font color to use for element's text */
            void setFontColor(const Kobold::String& colorStr);
            /*! \return font color */
            const Farso::Color& getFontColor() const;

            /*! Draw the Skin for this element on a Surface. 
             * \param dest drawable surface where will draw to
             * \param src surface with the texture atlas of this skin.
             * \param wx1 widget left coordinate 
             * \param wy2 widget top coordinate 
             * \param wx2 widget right coordinate 
             * \param wy2 widget bottom coordinate 
             * \note: both dest and src surface must be locked. */ 
            void draw(Surface* dest, Surface* src, 
                  int wx1, int wy1, int wx2, int wy2);
            /*! Same as #draw, but writing caption at text area, if defined */
            void draw(Surface* dest, Surface* src, 
                  int wx1, int wy1, int wx2, int wy2, 
                  const Rect& bounds, const Kobold::String& caption);
            /*! Same as #draw, but writing caption with an specific font */
            void draw(Surface* dest, Surface* src, 
                  int wx1, int wy1, int wx2, int wy2, 
                  const Rect& bounds, const Kobold::String& caption,
                  const Kobold::String& fontName, int fontSize, 
                  const Font::Alignment& align, const Color& fontColor,
                  const Color& outlineColor, int outlineWidth);
            /*! \return if this SkinElement is defined or not. */
            const bool isDefined() const;

            /*! \return minimun size to use this skin element
             * \note: the min size is only calculated once - and on demand -
             *        per skin element. */
            const Rect getMinSize() const;

            /*! Get the rectangle who defines the bounds of this element, 
             * in widget coordinates. 
             * \param width widget's width 
             * \param height widget's height */
            const Farso::Rect getBounds(int width, int height) const;

            /*! Get the rectangle who defines the body of this element,
             * in wiget coordinates.
             * \param width widget's width
             * \param height widget's height */
            const Farso::Rect getBody(int width, int height) const;

         private:

            const void defBoundValue(int& bx1, int& by1, int& bx2, int& by2,
                  const Rect& delta, const Rect& rect, 
                  bool useDeltaX1, bool useDeltaY1, 
                  bool useDeltaX2, bool useDeltaY2, int width, 
                  int height) const;

            Farso::Rect borderDelta;
            Farso::Rect leftBorderDelta;
            Farso::Rect rightBorderDelta;
            Farso::Rect topBorderDelta;
            Farso::Rect bottomBorderDelta;
            Farso::Rect leftBorder;
            Farso::Rect topBorder;
            Farso::Rect bottomBorder;
            Farso::Rect rightBorder;
           
            Farso::Rect cornerDelta;
            Farso::Rect topLeftCorner;
            Farso::Rect topRightCorner;
            Farso::Rect bottomLeftCorner;
            Farso::Rect bottomRightCorner;

            Farso::Rect backgroundDelta;
            Farso::Rect background;

            /*! The text area delta is relative to element's bounds */
            Farso::Rect textAreaDelta;
            /*! Font to use for text area */
            Kobold::String fontName;
            /*! Font size to use for text area */
            int fontSize;
            /*! Color to use for the font */
            Farso::Color fontColor;
            /*! Alignment to use for font */
            Farso::Font::Alignment fontAlign;
            /*! Minimun size to this skin element be valid */
            Farso::Rect minSize;
      };

      /*! Constructor. */
      Skin();
      /*! Destructor */
      virtual ~Skin();

      /*! Load the skin definition file, using current Controller::loader.
       * \param filename skin defintion file.
       * \return if load was successfull */
      bool load(const Kobold::String& filename);

      /*! Load the skin definition file, with specific DefParser. 
       * \param filename skin defintion file.
       * \param def DefParser implementation instance to use.
       * \return if load was successfull */
      bool load(const Kobold::String& filename, Kobold::DefParser& def);

      /*! Get SkinElement for a basic type. 
       * \return SkinElement definition for specific widget type */
      const SkinElement& getSkinElement(int type) const;

      /*! \return surface with texture atlas */
      Surface* getSurface();

      /*! Draw the Skin for an specifc element on a Surface. 
       * \param dest drawable surface where will draw to
       * \param type type of the element to be draw 
       * \param wx1 widget left coordinate 
       * \param wy2 widget top coordinate 
       * \param wx2 widget right coordinate 
       * \param wy2 widget bottom coordinate 
       * \note: both dest and this->surface surfaces must be locked before
       *        calling this function. */ 
      void drawElement(Surface* dest, int type, 
            int wx1, int wy1, int wx2, int wy2);
      /*! Same as #drawElement, but writing caption at element's text area, 
       * if defined.
       * \param bounds Pre-calculated element bounds for current widget.
       * \param caption to write. */
      void drawElement(Surface* dest, int type, 
            int wx1, int wy1, int wx2, int wy2, const Rect& bounds, 
            const Kobold::String& caption);
      /*! Same as #drawElement, but writing caption at element's text area, 
       * if defined, with an specific font .
       * \param bounds Pre-calculated element bounds for current widget.
       * \param caption to write. */
      void drawElement(Surface* dest, int type, 
            int wx1, int wy1, int wx2, int wy2, const Rect& bounds, 
            const Kobold::String& caption, const Kobold::String& fontName, 
            int fontSize, const Font::Alignment& align, const Color& fontColor,
            const Color& outlineColor = Colors::black, 
            int outlineWidth = 0);

      /*! \return if the Element type is defined or not on this skin. */
      const bool isElementDefined(int type) const;

      /*! \return filename of the skin. */
      const Kobold::String& getFilename() const;

      /*! Get the default font information */
      const void getDefaultFontInfo(Kobold::String& fontFilename, int& fontSize,
            Farso::Color& fontColor) const;

      /*! \return SkinElementType constant based on its name.
       * For example, "window" will return ELEMENT_TYPE_WINDOW and
       * so on. */
      int getElementType(const Kobold::String& typeName);

   protected:

      /*! \return total distinct skin elements supported. */
      virtual int getTotalElements();

      /*! Get extended element type constant.
       * \param typeName name
       * \return extended element type constant ( >= 
       * TOTAL_BASIC_SKIN_ELEMENT_TYPES && < getTotalElements )
       * or UNKNOW_ELEMENT when typeName isn't defined. */
      virtual int getExtendedElementType(const Kobold::String& typeName);

   private:

      SkinElement& getInnerSkinElement(int type) const;

      Surface* surface; /** The surface with the skin texture atlas. */

      Kobold::String defaultFont; /**< Default font to use. */
      int defaultFontSize; /**< Default font size to use. */
      Farso::Color defaultFontColor; /**< Default font color to use. */
      
      SkinElement* elements; /** Elements vector */
      int total; /** Total elements */

      Kobold::String filename; /**< Skin filename */
};

}

#endif

