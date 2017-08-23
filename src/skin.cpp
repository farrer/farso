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

#include "skin.h"
#include "controller.h"
#include "font.h"

#include <kobold/defparser.h>
#include <kobold/log.h>
#include <stdio.h>
#include <assert.h>

using namespace Farso;

#define SKIN_KEY_ATLAS               "atlas"
#define SKIN_KEY_DEFAULT_FONT        "defaultFont"
#define SKIN_KEY_DEFAULT_FONT_SIZE   "defaultFontSize"
#define SKIN_KEY_DEFAULT_FONT_COLOR  "defaultFontColor"
#define SKIN_KEY_ELEMENT             "element"
#define SKIN_KEY_BACKGROUND          "background"
#define SKIN_KEY_BACKGROUND_DELTA    "backgroundDelta"
#define SKIN_KEY_BORDER_DELTA        "borderDelta"
#define SKIN_KEY_TOP_BORDER_DELTA    "topBorderDelta"
#define SKIN_KEY_LEFT_BORDER_DELTA   "leftBorderDelta"
#define SKIN_KEY_RIGHT_BORDER_DELTA  "rightBorderDelta"
#define SKIN_KEY_BOTTOM_BORDER_DELTA "bottomBorderDelta"
#define SKIN_KEY_LEFT_BORDER         "leftBorder"
#define SKIN_KEY_RIGHT_BORDER        "rightBorder"
#define SKIN_KEY_TOP_BORDER          "topBorder"
#define SKIN_KEY_BOTTOM_BORDER       "bottomBorder"
#define SKIN_KEY_CORNER_DELTA        "cornerDelta"
#define SKIN_KEY_TOP_LEFT_CORNER     "topLeftCorner"
#define SKIN_KEY_TOP_RIGHT_CORNER    "topRightCorner"
#define SKIN_KEY_BOTTOM_LEFT_CORNER  "bottomLeftCorner"
#define SKIN_KEY_BOTTOM_RIGHT_CORNER "bottomRightCorner"
#define SKIN_KEY_TEXT_AREA_DELTA     "textAreaDelta"
#define SKIN_KEY_TEXT_FONT           "textFont"
#define SKIN_KEY_TEXT_FONT_SIZE      "textFontSize"
#define SKIN_KEY_TEXT_FONT_ALIGNMENT "textAlignment"
#define SKIN_KEY_TEXT_FONT_COLOR     "textFontColor"

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
Skin::SkinElement::SkinElement()
{
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
Skin::SkinElement::~SkinElement()
{
}

/***********************************************************************
 *                                  set                                *
 ***********************************************************************/
void Skin::SkinElement::set(const Kobold::String& key, 
      int x1, int y1, int x2, int y2)
{
   /* Define element data */
   if(key == SKIN_KEY_BACKGROUND_DELTA)
   {
      backgroundDelta.set(x1, y1, x2, y2);
   }
   else if(key == SKIN_KEY_BACKGROUND)
   {
      background.set(x1, y1, x2, y2);
   }
   else if(key == SKIN_KEY_BORDER_DELTA)
   {
      borderDelta.set(x1, y1, x2, y2);
   }
   else if(key == SKIN_KEY_TOP_BORDER_DELTA)
   {
      topBorderDelta.set(x1, y1, x2, y2);
   }
   else if(key == SKIN_KEY_BOTTOM_BORDER_DELTA)
   {
      bottomBorderDelta.set(x1, y1, x2, y2);
   }
   else if(key == SKIN_KEY_LEFT_BORDER_DELTA)
   {
      leftBorderDelta.set(x1, y1, x2, y2);
   }
   else if(key == SKIN_KEY_RIGHT_BORDER_DELTA)
   {
      rightBorderDelta.set(x1, y1, x2, y2);
   }
   else if(key == SKIN_KEY_LEFT_BORDER)
   {
      leftBorder.set(x1, y1, x2, y2);
   }
   else if(key == SKIN_KEY_RIGHT_BORDER)
   {
      rightBorder.set(x1, y1, x2, y2);
   }
   else if(key == SKIN_KEY_TOP_BORDER)
   {
      topBorder.set(x1, y1, x2, y2);
   }
   else if(key == SKIN_KEY_BOTTOM_BORDER)
   {
      bottomBorder.set(x1, y1, x2, y2);
   }
   else if(key == SKIN_KEY_CORNER_DELTA)
   {
      cornerDelta.set(x1, y1, x2, y2);
   }
   else if(key == SKIN_KEY_TOP_LEFT_CORNER)
   {
      topLeftCorner.set(x1, y1, x2, y2);
   }
   else if(key == SKIN_KEY_TOP_RIGHT_CORNER)
   {
      topRightCorner.set(x1, y1, x2, y2);
   }
   else if(key == SKIN_KEY_BOTTOM_LEFT_CORNER)
   {
      bottomLeftCorner.set(x1, y1, x2, y2);
   }
   else if(key == SKIN_KEY_BOTTOM_RIGHT_CORNER)
   {
      bottomRightCorner.set(x1, y1, x2, y2);
   }
   else if(key == SKIN_KEY_TEXT_AREA_DELTA)
   {
      textAreaDelta.set(x1, y1, x2, y2);
   }
   else
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "ERROR: Unknown key '%s' at skin!", key.c_str());
   }
}

/***********************************************************************
 *                             hasBackground                           *
 ***********************************************************************/
const bool Skin::SkinElement::hasBackground() const
{
   return background.isDefined();
}

/***********************************************************************
 *                               hasBorder                             *
 ***********************************************************************/
const bool Skin::SkinElement::hasBorder() const
{
   return leftBorder.isDefined() || rightBorder.isDefined() ||
          bottomBorder.isDefined() || topBorder.isDefined();
}

/***********************************************************************
 *                         getLeftBorderDelta                          *
 ***********************************************************************/
const Farso::Rect& Skin::SkinElement::getLeftBorderDelta() const
{
   if(leftBorderDelta.isDefined())
   {
      return leftBorderDelta;
   }
   return borderDelta;
}

/***********************************************************************
 *                        getRightBorderDelta                          *
 ***********************************************************************/
const Farso::Rect& Skin::SkinElement::getRightBorderDelta() const
{
   if(rightBorderDelta.isDefined())
   {
      return rightBorderDelta;
   }
   return borderDelta;
}

/***********************************************************************
 *                          getTopBorderDelta                          *
 ***********************************************************************/
const Farso::Rect& Skin::SkinElement::getTopBorderDelta() const
{
   if(topBorderDelta.isDefined())
   {
      return topBorderDelta;
   }
   return borderDelta;
}

/***********************************************************************
 *                        getBottomBorderDelta                         *
 ***********************************************************************/
const Farso::Rect& Skin::SkinElement::getBottomBorderDelta() const
{
   if(bottomBorderDelta.isDefined())
   {
      return bottomBorderDelta;
   }
   return borderDelta;
}
 
/***********************************************************************
 *                               hasCorner                             *
 ***********************************************************************/
const bool Skin::SkinElement::hasCorner() const
{
   return topLeftCorner.isDefined() || topRightCorner.isDefined() ||
          bottomLeftCorner.isDefined() || bottomRightCorner.isDefined();
}

/***********************************************************************
 *                            setFontName                              *
 ***********************************************************************/
void Skin::SkinElement::setFontName(const Kobold::String& fontName)
{
   this->fontName = fontName;
}

/***********************************************************************
 *                            getFontName                              *
 ***********************************************************************/
const Kobold::String& Skin::SkinElement::getFontName() const
{
   return this->fontName;
}

/***********************************************************************
 *                            setFontSize                              *
 ***********************************************************************/
void Skin::SkinElement::setFontSize(int size)
{
   this->fontSize = size;
}

/***********************************************************************
 *                            fetFontSize                              *
 ***********************************************************************/
const int Skin::SkinElement::getFontSize() const
{
   return this->fontSize;
}

/***********************************************************************
 *                         setFontAlignment                            *
 ***********************************************************************/
void Skin::SkinElement::setFontAlignment(const Font::Alignment& align)
{
  this->fontAlign = align;
}

/***********************************************************************
 *                         getFontAlignment                            *
 ***********************************************************************/
const Font::Alignment& Skin::SkinElement::getFontAlignment() const
{
   return this->fontAlign;
}


/***********************************************************************
 *                           setFontColor                              *
 ***********************************************************************/
void Skin::SkinElement::setFontColor(Farso::Color color)
{
   this->fontColor = color;
}

/***********************************************************************
 *                           setFontColor                              *
 ***********************************************************************/
void Skin::SkinElement::setFontColor(Kobold::String colorStr)
{
   this->fontColor.set(colorStr);
}

/***********************************************************************
 *                           getFontColor                              *
 ***********************************************************************/
const Farso::Color& Skin::SkinElement::getFontColor() const
{
   return this->fontColor;
}

/***********************************************************************
 *                              isDefined                              *
 ***********************************************************************/
const bool Skin::SkinElement::isDefined() const
{
   return hasBackground() || hasCorner() || hasBorder();
}

/***********************************************************************
 *                         defBoundValue                               *
 ***********************************************************************/
const void Skin::SkinElement::defBoundValue(int& bx1, int& by1, 
      int& bx2, int& by2, const Rect& delta, const Rect& rect, 
      bool useDeltaX1, bool useDeltaY1, bool useDeltaX2, bool useDeltaY2, 
      int width, int height) const
{
   assert(useDeltaX1 || useDeltaX2);
   assert(useDeltaY1 || useDeltaY2);

   if(useDeltaX1)
   {
      /* Direct check with X1 element */
      if(delta.getX1() < bx1)
      {
         bx1 = delta.getX1();
      }
   }
   else if(useDeltaX2)
   {  
      /* Calculate X1 by X2 element and rect */
      int val = width - delta.getX2() - rect.getWidth();
      if(val < bx1)
      {
         bx1 = val;
      }
   }
   if(useDeltaY1)
   {
      /* Direct use Y1 */
      if(delta.getY1() < by1)
      {
         by1 = delta.getY1();
      }
   }
   else if(useDeltaY2)
   {
      /* Calculate y2 from y1 */
      int val = height - delta.getY2() - rect.getHeight();
      if(val < by1)
      {
         by1 = val;
      }
   }
   if(useDeltaX2)
   {
      /* Direct use delta x2 */
      int val = width - delta.getX2();
      if(val > bx2)
      {
         bx2 = val;
      }
   }
   else if(useDeltaX1)
   {
      /* Calculate from X1 */
      int val = delta.getX1() + rect.getWidth();
      if(val > bx2)
      {
         bx2 = val;
      }
   }
   if(useDeltaY2)
   {
      /* Direct use delta y2 */
      int val = height - delta.getY2();
      if(val > by2)
      {
         by2 = val;
      }
   }
   else if(useDeltaY1)
   {
      /* Calculate from Y1 */
      int val = delta.getY1() + rect.getHeight();
      if(val > by2)
      {
         by2 = val;
      }
   }
}

/***********************************************************************
 *                              getBody                                *
 ***********************************************************************/
const Farso::Rect Skin::SkinElement::getBody(int width, int height) const
{
   int bx1 = 0;
   int bx2 = width - 1;
   int by1 = 0;
   int by2 = height - 1;

   if(hasBorder())
   {
      if(topBorder.isDefined())
      {
         by1 = getTopBorderDelta().getY1() + topBorder.getHeight() + 1;
      }

      if(leftBorder.isDefined())
      {
         bx1 = getLeftBorderDelta().getX1() + leftBorder.getWidth() + 1;
      }

      if(bottomBorder.isDefined())
      {
         by2 = (height - getBottomBorderDelta().getY2()) - 
            bottomBorder.getHeight() - 1;
      }

      if(rightBorder.isDefined())
      {
         bx2 = (width - getRightBorderDelta().getX2()) - 
            rightBorder.getWidth() - 1;
      }
   }

   return Rect(bx1, by1, bx2, by2);
}

/***********************************************************************
 *                            getMinSize                               *
 ***********************************************************************/
const Farso::Rect Skin::SkinElement::getMinSize() const
{
   if(!minSize.isDefined())
   {
      return getBounds(0, 0);
   }

   return minSize;
}

/***********************************************************************
 *                             getBounds                               *
 ***********************************************************************/
const Farso::Rect Skin::SkinElement::getBounds(int width, int height) const
{
   int x1=10000, y1=10000, x2=-1, y2=-1;

   /* Background */
   if(hasBackground())
   {
      defBoundValue(x1, y1, x2, y2, backgroundDelta, background, 
                    true, true, true, true, width, height);
   }

   /* Border */
   if(hasBorder())
   {
      /* Top border */
      if(topBorder.isDefined())
      {
         defBoundValue(x1, y1, x2, y2, getTopBorderDelta(), topBorder, 
               true, true, true, false, width, height);
      }

      /* Bottom border */
      if(bottomBorder.isDefined())
      {
         defBoundValue(x1, y1, x2, y2, getBottomBorderDelta(), 
               bottomBorder, true, false, true, true, width, height);
      }

      /* Left border */
      if(leftBorder.isDefined())
      {
         defBoundValue(x1, y1, x2, y2, getLeftBorderDelta(), leftBorder,
                       true, true, false, true, width, height);
      }

      /* Right border */
      if(rightBorder.isDefined())
      {
         defBoundValue(x1, y1, x2, y2, getRightBorderDelta(), rightBorder,
                       false, true, true, true, width, height);
      }
   }

   /* Corner */
   if(hasCorner())
   {
      /* Top Left Corner */
      if(topLeftCorner.isDefined())
      {
         defBoundValue(x1, y1, x2, y2, cornerDelta, topLeftCorner,
                       true, true, false, false, width, height);
      }

      /* Top Right Corner */
      if(topRightCorner.isDefined())
      {
         defBoundValue(x1, y1, x2, y2, cornerDelta, topRightCorner,
                       false, true, true, false, width, height);
      }

      /* Bottom Left Corner */
      if(bottomLeftCorner.isDefined())
      {
         defBoundValue(x1, y1, x2, y2, cornerDelta, bottomLeftCorner,
                       true, false, false, true, width, height);
      }

      /* Bottom Right Corner */
      if(bottomRightCorner.isDefined())
      {
         defBoundValue(x1, y1, x2, y2, cornerDelta, bottomRightCorner,
                       false, false, true, true, width, height);
      }
   }

   return Rect(x1, y1, x2, y2);
}

/***********************************************************************
 *                                draw                                 *
 ***********************************************************************/
void Skin::SkinElement::draw(Surface* dest, Surface* src, 
      int wx1, int wy1, int wx2, int wy2)
{
   Farso::Rect rect, delta;
   Farso::Draw* fdraw = Farso::Controller::getDraw();

   /* Background */
   if(hasBackground())
   {
      delta = backgroundDelta;
      rect = background;
      fdraw->doStampFill(dest, wx1 + delta.getX1(), wy1 + delta.getY1(),
            wx2 - delta.getX2(), wy2 - delta.getY2(), 
            src, rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
   }

   /* Border */
   if(hasBorder())
   {
      /* Top border */
      rect = topBorder;
      if(rect.isDefined())
      {
         delta = getTopBorderDelta();
         fdraw->doStampFill(dest, wx1 + delta.getX1(), wy1 + delta.getY1(),
               wx2 - delta.getX2(), 
               wy1 + rect.getY2() - rect.getY1() + delta.getY1(), 
               src, rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
      }

      /* Bottom border */
      rect = bottomBorder;
      if(rect.isDefined())
      {
         delta = getBottomBorderDelta();
         fdraw->doStampFill(dest, wx1 + delta.getX1(),
               wy2 - delta.getY2() - (rect.getY2() - rect.getY1()),
               wx2 - delta.getX2(), 
               wy2 - delta.getY2(), src,
               rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
      }

      /* Left border */
      rect = leftBorder;
      if(rect.isDefined())
      {
         delta = getLeftBorderDelta();
         fdraw->doStampFill(dest, wx1 + delta.getX1(), wy1 + delta.getY1(),
               wx1 + delta.getX1() + (rect.getX2() - rect.getX1()), 
               wy2 - delta.getY2(), src,
               rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
      }

      /* Right border */
      rect = rightBorder;
      if(rect.isDefined())
      {
         delta = getRightBorderDelta();
         fdraw->doStampFill(dest, 
               wx2 - delta.getX2() - (rect.getX2() - rect.getX1()),
               wy1 + delta.getY1(), wx2 - delta.getX2(), 
               wy2 - delta.getY2(), src,
               rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
      }
   }

   /* Corner */
   if(hasCorner())
   {
      delta = cornerDelta;

      /* Top Left Corner */
      rect = topLeftCorner;
      if(rect.isDefined())
      {
         fdraw->doStampFill(dest, wx1 + delta.getX1(), wy1 + delta.getY1(),
               wx1 + delta.getX1() + (rect.getX2() - rect.getX1()),
               wy1 + delta.getY1() + (rect.getY2() - rect.getY1()), src,
               rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
      }

      /* Top Right Corner */
      rect = topRightCorner;
      if(rect.isDefined())
      {
         fdraw->doStampFill(dest, 
               wx2 - delta.getX2() - (rect.getX2() - rect.getX1()),
               wy1 + delta.getY1(), wx2 - delta.getX2(),
               wy1 + delta.getY1() + (rect.getY2() - rect.getY1()), src,
               rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
      }

      /* Bottom Left Corner */
      rect = bottomLeftCorner;
      if(rect.isDefined())
      {
         fdraw->doStampFill(dest, wx1 + delta.getX1(),
               wy2 - delta.getY2() - (rect.getY2() - rect.getY1()),
               wx1 + delta.getX1() + (rect.getX2() - rect.getX1()),
               wy2 - delta.getY2(), src,
               rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
      }

      /* Bottom Right Corner */
      rect = bottomRightCorner;
      if(rect.isDefined())
      {
         fdraw->doStampFill(dest, 
               wx2 - delta.getX2() - (rect.getX2() - rect.getX1()),
               wy2 - delta.getY2() - (rect.getY2() - rect.getY1()),
               wx2 - delta.getX2(), wy2 - delta.getY2(), 
               src, rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
      }
   }
}

/***********************************************************************
 *                                draw                                 *
 ***********************************************************************/
void Skin::SkinElement::draw(Surface* dest, Surface* src, 
      int wx1, int wy1, int wx2, int wy2, Rect bounds, Kobold::String caption)
{
   draw(dest, src, wx1, wy1, wx2, wy2, bounds, caption, fontName, 
         fontSize, fontAlign, fontColor, fontColor, 0);
}

/***********************************************************************
 *                                draw                                 *
 ***********************************************************************/
void Skin::SkinElement::draw(Surface* dest, Surface* src, 
      int wx1, int wy1, int wx2, int wy2, Rect bounds, Kobold::String caption,
      Kobold::String fontName, int fontSize, Font::Alignment align, 
      Color fontColor, Color outlineColor, int outlineWidth)
{
   /* Do the normal draw */
   draw(dest, src, wx1, wy1, wx2, wy2);

   if( (textAreaDelta.isDefined()) && (!caption.empty()) )
   {
      /* Write with the desired font */
      Font* font = Farso::FontManager::getFont(fontName);
      assert(font != NULL);
      Farso::Controller::getDraw()->setActiveColor(fontColor);
      font->setSize(fontSize);
      font->setAlignment(fontAlign);
      Rect area(bounds.getX1() + textAreaDelta.getX1(),
            bounds.getY1() + textAreaDelta.getY1(),
            bounds.getX2() - textAreaDelta.getX2(),
            bounds.getY2() - textAreaDelta.getY2());
      if(outlineWidth > 0)
      {
         font->write(dest, area, caption, outlineColor, outlineWidth);
      }
      else
      {
         font->write(dest, area, caption);
      }
   }
}

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
Skin::Skin()
     :surface(NULL),
      defaultFontSize(10),
      elements(NULL),
      filename("")
{
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
Skin::~Skin()
{
   if(surface)
   {
      delete surface;
   }
   if(elements)
   {
      delete[] elements;
   }
}

/***********************************************************************
 *                              getFilename                            *
 ***********************************************************************/
const Kobold::String& Skin::getFilename() const
{
   return filename;
}

/***********************************************************************
 *                            getSkinElement                           *
 ***********************************************************************/
const Skin::SkinElement& Skin::getSkinElement(int type) const
{
   return getInnerSkinElement(type);
}
Skin::SkinElement& Skin::getInnerSkinElement(int type) const
{
   if((type > SKIN_TYPE_UNKNOWN) && (type < total))
   {
      return elements[type];
   }

   assert(false);
   return elements[SKIN_TYPE_WINDOW];
}

/***********************************************************************
 *                                load                                 *
 ***********************************************************************/
bool Skin::load(const Kobold::String& filename)
{
   Kobold::DefParser def;

   /* Define elements vector and totals */
   total = getTotalElements();
   if(elements)
   {
      delete[] elements;
   }
   elements = new SkinElement[total];
   
   if(!def.load(Controller::getRealFilename(filename), 
            (Controller::getRendererType() != RENDERER_TYPE_OGRE3D)))
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "ERROR: Failed to load skin: '%s'", filename.c_str());
      return false;
   }
   this->filename = filename;

   Kobold::String key, value;
   int cur = -1; //current element

   while(def.getNextTuple(key, value))
   {
      if(key == SKIN_KEY_ATLAS)
      {
         if(surface == NULL)
         {
            surface = Controller::loadImageToSurface(
                  Controller::getRealFilename(value)); 
         }
      }
      else if(key == SKIN_KEY_DEFAULT_FONT)
      {
         defaultFont = value;
      }
      else if(key == SKIN_KEY_DEFAULT_FONT_SIZE)
      {
         sscanf(value.c_str(), "%d", &defaultFontSize);
      }
      else if(key == SKIN_KEY_DEFAULT_FONT_COLOR)
      {
         defaultFontColor.set(value);
      }
      else if(key == SKIN_KEY_ELEMENT)
      {
         cur = getElementType(value);
         assert(cur != SKIN_TYPE_UNKNOWN);
         elements[cur].setFontName(defaultFont);
         elements[cur].setFontSize(defaultFontSize);
         elements[cur].setFontAlignment(Font::TEXT_LEFT);
         elements[cur].setFontColor(defaultFontColor);
      }
      else
      {
         /* Check if valid */
         if(cur < 0)
         {
            /* Invalid file. */
            Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
                  "ERROR: Defined '%s' without element at skin: '%s'",
                  key.c_str(), filename.c_str());
            return false;
         }

         if(key == SKIN_KEY_TEXT_FONT)
         {
            elements[cur].setFontName(value);
         }
         else if(key == SKIN_KEY_TEXT_FONT_SIZE)
         {
            int size = 0;
            sscanf(value.c_str(), "%d", &size);
            elements[cur].setFontSize(size);
         }
         else if (key == SKIN_KEY_TEXT_FONT_ALIGNMENT)
         {
            int alignment = 0;
            sscanf(value.c_str(), "%d", &alignment);
            if(alignment <= Font::TEXT_RIGHT)
            {
               elements[cur].setFontAlignment((Font::Alignment) alignment);
            }
         }
         else if(key == SKIN_KEY_TEXT_FONT_COLOR)
         {
            elements[cur].setFontColor(value);
         }
         else
         {

            /* Parse values */
            int x1=0,y1=0,x2=0,y2=0;
            sscanf(value.c_str(), "%d,%d,%d,%d", &x1, &y1, &x2, &y2);
            elements[cur].set(key, x1, y1, x2, y2); 
         }
      }
   }

   return true;
}

/***********************************************************************
 *                            getWidgetType                            *
 ***********************************************************************/
int Skin::getElementType(Kobold::String typeName)
{
   if(typeName == "window")
   {
      return SKIN_TYPE_WINDOW;
   }
   else if(typeName == "windowActiveTitleBar")
   {
      return SKIN_TYPE_WINDOW_ACTIVE_TITLE_BAR;
   }
   else if(typeName == "windowInactiveTitleBar")
   {
      return SKIN_TYPE_WINDOW_INACTIVE_TITLE_BAR;
   }
   else if(typeName == "windowEnabledCloseButton")
   {
      return SKIN_TYPE_WINDOW_ENABLED_CLOSE_BUTTON;
   }
   else if(typeName == "windowPressedCloseButton")
   {
      return SKIN_TYPE_WINDOW_PRESSED_CLOSE_BUTTON;
   }
   else if(typeName == "windowOnOverCloseButton")
   {
      return SKIN_TYPE_WINDOW_ON_OVER_CLOSE_BUTTON;
   }
   else if(typeName == "windowDisabledCloseButton")
   {
      return SKIN_TYPE_WINDOW_DISABLED_CLOSE_BUTTON;
   }
   else if(typeName == "buttonEnabled")
   {
      return SKIN_TYPE_BUTTON_ENABLED;
   }
   else if(typeName == "buttonPressed")
   {
      return SKIN_TYPE_BUTTON_PRESSED;
   }
   else if(typeName == "buttonOnOver")
   {
      return SKIN_TYPE_BUTTON_ON_OVER;
   }
   else if(typeName == "buttonDisabled")
   {
      return SKIN_TYPE_BUTTON_DISABLED;
   }
   else if(typeName == "checkBoxEnabled")
   {
      return SKIN_TYPE_CHECKBOX_ENABLED;
   }
   else if(typeName == "checkBoxEnabledChecked")
   {
      return SKIN_TYPE_CHECKBOX_ENABLED_CHECKED;
   }
   else if(typeName == "checkBoxDisabled")
   {
      return SKIN_TYPE_CHECKBOX_DISABLED;
   }
   else if(typeName == "checkBoxDisabledChecked")
   {
      return SKIN_TYPE_CHECKBOX_DISABLED_CHECKED;
   }
   else if(typeName == "label")
   {
      return SKIN_TYPE_LABEL;
   }
   else if(typeName == "labelDisabled")
   {
      return SKIN_TYPE_LABEL_DISABLED;
   }
   else if(typeName == "textOption")
   {
      return SKIN_TYPE_TEXT_OPTION;
   }
   else if(typeName == "textOptionOver")
   {
      return SKIN_TYPE_TEXT_OPTION_OVER;
   }
   else if(typeName == "upCaption")
   {
      return SKIN_TYPE_CAPTION_UP;
   }
   else if(typeName == "downCaption")
   {
      return SKIN_TYPE_CAPTION_DOWN;
   }
   else if(typeName == "leftCaption")
   {
      return SKIN_TYPE_CAPTION_LEFT;
   }
   else if(typeName == "rightCaption")
   {
      return SKIN_TYPE_CAPTION_RIGHT;
   }
   else if(typeName == "progressBar")
   {
      return SKIN_TYPE_PROGRESS_BAR;
   }
   else if(typeName == "progressBarFill")
   {
      return SKIN_TYPE_PROGRESS_BAR_FILL;
   }
   else if(typeName == "borderLeft")
   {
      return SKIN_TYPE_BORDER_LEFT;
   }
   else if(typeName == "borderRight")
   {
      return SKIN_TYPE_BORDER_RIGHT;
   }
   else if(typeName == "borderTop")
   {
      return SKIN_TYPE_BORDER_TOP;
   }
   else if(typeName == "borderBottom")
   {
      return SKIN_TYPE_BORDER_BOTTOM;
   }
   else if(typeName == "textEntry")
   {
      return SKIN_TYPE_TEXTENTRY;
   }
   else if(typeName == "textEntryDisabled")
   {
      return SKIN_TYPE_TEXTENTRY_DISABLED;
   }
   else if(typeName == "cursorTextualTip")
   {
      return SKIN_TYPE_CURSOR_TEXTUAL_TIP;
   }
   else if(typeName == "gridFill")
   {
      return SKIN_TYPE_GRID_FILL;
   }
   else if(typeName == "gridBorder")
   {
      return SKIN_TYPE_GRID_BORDER;
   }
   else if(typeName == "fileLabel")
   {
      return SKIN_TYPE_FILE_LABEL;
   }
   else if(typeName == "directoryLabel")
   {
      return SKIN_TYPE_DIRECTORY_LABEL;
   }
   else if(typeName == "menu")
   {
      return SKIN_TYPE_MENU;
   }
   else if(typeName == "menuSeparator")
   {
      return SKIN_TYPE_MENU_SEPARATOR;
   }

   /* Try to get from user defined ones */
   int elementType = getExtendedElementType(typeName);

   if(elementType == SKIN_TYPE_UNKNOWN)
   {
      /* Unknown widget. */
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
            "ERROR: Unknow widget name '%s' on skin definition file!",
            typeName.c_str());
   }

   return elementType;
}

/***********************************************************************
 *                           getTotalElements                          *
 ***********************************************************************/
int Skin::getTotalElements()
{
   return TOTAL_BASIC_SKIN_ELEMENT_TYPES;
}

/***********************************************************************
 *                        getExtendedElementType                       *
 ***********************************************************************/
int Skin::getExtendedElementType(Kobold::String typeName)
{
   return SKIN_TYPE_UNKNOWN;
}

/***********************************************************************
 *                              getSurface                             *
 ***********************************************************************/
Surface* Skin::getSurface()
{
   return surface;
}

/***********************************************************************
 *                             drawElement                             *
 ***********************************************************************/
void Skin::drawElement(Surface* dest, int type, 
      int wx1, int wy1, int wx2, int wy2)
{
   getInnerSkinElement(type).draw(dest, surface, wx1, wy1, wx2, wy2); 
}

/***********************************************************************
 *                             drawElement                             *
 ***********************************************************************/
void Skin::drawElement(Surface* dest, int type, 
      int wx1, int wy1, int wx2, int wy2, Rect bounds, Kobold::String caption)
{
   getInnerSkinElement(type).draw(dest, surface, wx1, wy1, wx2, wy2, bounds, 
         caption);
}

/***********************************************************************
 *                             drawElement                             *
 ***********************************************************************/
void Skin::drawElement(Surface* dest, int type, int wx1, int wy1, 
      int wx2, int wy2, Rect bounds, Kobold::String caption, 
      Kobold::String fontName, int fontSize, Font::Alignment align,
      Color fontColor, Color outlineColor, int outlineWidth)
{
   getInnerSkinElement(type).draw(dest, surface, wx1, wy1, wx2, wy2, bounds,
         caption, fontName, fontSize, align, fontColor, outlineColor,
         outlineWidth);
}

/***********************************************************************
 *                          isElementDefined                           *
 ***********************************************************************/
const bool Skin::isElementDefined(int type) const
{
   if((type > SKIN_TYPE_UNKNOWN) && (type < total))
   {
      return getSkinElement(type).isDefined();
   }

   return false;
}

/***********************************************************************
 *                         getDefaultFontInfo                          *
 ***********************************************************************/
const void Skin::getDefaultFontInfo(Kobold::String& fontFilename, int& fontSize,
      Farso::Color& fontColor) const
{
   fontFilename = defaultFont;
   fontSize = defaultFontSize;
   fontColor = defaultFontColor;
}

