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
 *                             hasBackground                           *
 ***********************************************************************/
bool Skin::SkinElement::hasBackground()
{
   return background.isDefined();
}

/***********************************************************************
 *                          getBackgroundDelta                         *
 ***********************************************************************/
Rect& Skin::SkinElement::getBackgroundDelta()
{
   return backgroundDelta;
}

/***********************************************************************
 *                             getBackground                           *
 ***********************************************************************/
Rect& Skin::SkinElement::getBackground()
{
   return background;
}

/***********************************************************************
 *                               hasBorder                             *
 ***********************************************************************/
bool Skin::SkinElement::hasBorder()
{
   return leftBorder.isDefined() || rightBorder.isDefined() ||
          bottomBorder.isDefined() || topBorder.isDefined();
}

/***********************************************************************
 *                            getBorderDelta                           *
 ***********************************************************************/
Rect& Skin::SkinElement::getBorderDelta()
{
   return borderDelta;
}

/***********************************************************************
 *                         getLeftBorderDelta                          *
 ***********************************************************************/
Rect& Skin::SkinElement::getLeftBorderDelta(bool onlyIfDefined)
{
   if((!onlyIfDefined) || (leftBorderDelta.isDefined()))
   {
      return leftBorderDelta;
   }
   return borderDelta;
}

/***********************************************************************
 *                        getRightBorderDelta                          *
 ***********************************************************************/
Rect& Skin::SkinElement::getRightBorderDelta(bool onlyIfDefined)
{
   if((!onlyIfDefined) || (rightBorderDelta.isDefined()))
   {
      return rightBorderDelta;
   }
   return borderDelta;
}

/***********************************************************************
 *                          getTopBorderDelta                          *
 ***********************************************************************/
Rect& Skin::SkinElement::getTopBorderDelta(bool onlyIfDefined)
{
   if((!onlyIfDefined) || (topBorderDelta.isDefined()))
   {
      return topBorderDelta;
   }
   return borderDelta;
}

/***********************************************************************
 *                        getBottomBorderDelta                         *
 ***********************************************************************/
Rect& Skin::SkinElement::getBottomBorderDelta(bool onlyIfDefined)
{
   if((!onlyIfDefined) || (bottomBorderDelta.isDefined()))
   {
      return bottomBorderDelta;
   }
   return borderDelta;
}
 
/***********************************************************************
 *                            getLeftBorder                            *
 ***********************************************************************/
Rect& Skin::SkinElement::getLeftBorder()
{
   return leftBorder;
}

/***********************************************************************
 *                             getTopBorder                            *
 ***********************************************************************/       Rect& Skin::SkinElement::getTopBorder()
{
   return topBorder;
}

/***********************************************************************
 *                           getRightBorder                            *
 ***********************************************************************/ 
Rect& Skin::SkinElement::getRightBorder()
{
   return rightBorder;
}

/***********************************************************************
 *                           getBottomBorder                           *
 ***********************************************************************/
Rect& Skin::SkinElement::getBottomBorder()
{
   return bottomBorder;
}

/***********************************************************************
 *                               hasCorner                             *
 ***********************************************************************/
bool Skin::SkinElement::hasCorner()
{
   return topLeftCorner.isDefined() || topRightCorner.isDefined() ||
          bottomLeftCorner.isDefined() || bottomRightCorner.isDefined();
}

/***********************************************************************
 *                            getCornerDelta                           *
 ***********************************************************************/
Rect& Skin::SkinElement::getCornerDelta()
{
   return cornerDelta;
}
 
/***********************************************************************
 *                           getTopLeftCorner                          *
 ***********************************************************************/
Rect& Skin::SkinElement::getTopLeftCorner()
{
   return topLeftCorner;
}

/***********************************************************************
 *                          getTopRightCorner                          *
 ***********************************************************************/
Rect& Skin::SkinElement::getTopRightCorner()
{
   return topRightCorner;
}

/***********************************************************************
 *                          getBottomLeftCorner                        *
 ***********************************************************************/
Rect& Skin::SkinElement::getBottomLeftCorner()
{
   return bottomLeftCorner;
}

/***********************************************************************
 *                         getBottomRightCorner                        *
 ***********************************************************************/
Rect& Skin::SkinElement::getBottomRightCorner()
{
   return bottomRightCorner;
}

/***********************************************************************
 *                           getTextAreaDelta                          *
 ***********************************************************************/
Rect& Skin::SkinElement::getTextAreaDelta()
{
   return textAreaDelta;
}

/***********************************************************************
 *                            setFontName                              *
 ***********************************************************************/
void Skin::SkinElement::setFontName(Kobold::String fontName)
{
   this->fontName = fontName;
}

/***********************************************************************
 *                            getFontName                              *
 ***********************************************************************/
Kobold::String Skin::SkinElement::getFontName()
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
int Skin::SkinElement::getFontSize()
{
   return this->fontSize;
}

/***********************************************************************
 *                         setFontAlignment                            *
 ***********************************************************************/
void Skin::SkinElement::setFontAlignment(Font::Alignment align)
{
  this->fontAlign = align;
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
Farso::Color Skin::SkinElement::getFontColor()
{
   return this->fontColor;
}

/***********************************************************************
 *                              isDefined                              *
 ***********************************************************************/
bool Skin::SkinElement::isDefined()
{
   return hasBackground() || hasCorner() || hasBorder();
}

/***********************************************************************
 *                         defBoundValue                               *
 ***********************************************************************/
void Skin::SkinElement::defBoundValue(int& bx1, int& by1, int& bx2, int& by2,
        Rect& delta, Rect& rect, bool useDeltaX1, bool useDeltaY1, 
        bool useDeltaX2, bool useDeltaY2, int width, int height)
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
Rect Skin::SkinElement::getBody(int width, int height)
{
   int bx1 = 0;
   int bx2 = width - 1;
   int by1 = 0;
   int by2 = height - 1;

   if(hasBorder())
   {
      Rect rect, delta;

      rect = getTopBorder();
      if(rect.isDefined())
      {
         delta = getTopBorderDelta();
         by1 = delta.getY1() + rect.getHeight() + 1;
      }

      rect = getLeftBorder();
      if(rect.isDefined())
      {
         delta = getLeftBorderDelta();
         bx1 = delta.getX1() + rect.getWidth() + 1;
      }

      rect = getBottomBorder();
      if(rect.isDefined())
      {
         delta = getBottomBorderDelta();
         by2 = (height - delta.getY2()) - rect.getHeight() - 1;
      }

      rect = getRightBorder();
      if(rect.isDefined())
      {
         delta = getRightBorderDelta();
         bx2 = (width - delta.getX2()) - rect.getWidth() - 1;
      }
   }

   return Rect(bx1, by1, bx2, by2);
}

/***********************************************************************
 *                            getMinSize                               *
 ***********************************************************************/
Rect Skin::SkinElement::getMinSize()
{
   if(!minSize.isDefined())
   {
      minSize = getBounds(0, 0);
   }

   return minSize;
}

/***********************************************************************
 *                             getBounds                               *
 ***********************************************************************/
Rect Skin::SkinElement::getBounds(int width, int height)
{
   int x1=10000, y1=10000, x2=-1, y2=-1;
   Farso::Rect rect;

   /* Background */
   if(hasBackground())
   {
      defBoundValue(x1, y1, x2, y2, getBackgroundDelta(), getBackground(), 
                    true, true, true, true, width, height);
   }

   /* Border */
   if(hasBorder())
   {
      /* Top border */
      rect = getTopBorder();
      if(rect.isDefined())
      {
         defBoundValue(x1, y1, x2, y2, getTopBorderDelta(), rect, 
                    true, true, true, false, width, height);
      }

      /* Bottom border */
      rect = getBottomBorder();
      if(rect.isDefined())
      {
         defBoundValue(x1, y1, x2, y2, getBottomBorderDelta(), rect, 
                       true, false, true, true, width, height);
      }

      /* Left border */
      rect = getLeftBorder();
      if(rect.isDefined())
      {
         defBoundValue(x1, y1, x2, y2, getLeftBorderDelta(), rect,
                       true, true, false, true, width, height);
      }

      /* Right border */
      rect = getRightBorder();
      if(rect.isDefined())
      {
         defBoundValue(x1, y1, x2, y2, getRightBorderDelta(), rect,
                       false, true, true, true, width, height);
      }
   }

   /* Corner */
   if(hasCorner())
   {
      /* Top Left Corner */
      rect = getTopLeftCorner();
      if(rect.isDefined())
      {
         defBoundValue(x1, y1, x2, y2, getCornerDelta(), rect,
                       true, true, false, false, width, height);
      }

      /* Top Right Corner */
      rect = getTopRightCorner();
      if(rect.isDefined())
      {
         defBoundValue(x1, y1, x2, y2, getCornerDelta(), rect,
                       false, true, true, false, width, height);
      }

      /* Bottom Left Corner */
      rect = getBottomLeftCorner();
      if(rect.isDefined())
      {
         defBoundValue(x1, y1, x2, y2, getCornerDelta(), rect,
                       true, false, false, true, width, height);
      }

      /* Bottom Right Corner */
      rect = getBottomRightCorner();
      if(rect.isDefined())
      {
         defBoundValue(x1, y1, x2, y2, getCornerDelta(), rect,
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
   int width = (wx2 - wx1 + 1);
   int height = (wy2 - wy1 + 1);

   /* Background */
   if(hasBackground())
   {
      delta = getBackgroundDelta();
      rect = getBackground();
      fdraw->doStampFill(dest, wx1 + delta.getX1(), wy1 + delta.getY1(),
            wx1 + width - delta.getX2(), wy1 + height - delta.getY2(), 
            src, rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
   }

   /* Border */
   if(hasBorder())
   {
      /* Top border */
      rect = getTopBorder();
      if(rect.isDefined())
      {
         delta = getTopBorderDelta();
         fdraw->doStampFill(dest, wx1 + delta.getX1(), wy1 + delta.getY1(),
               wx1 + width - delta.getX2(), 
               wy1 + rect.getY2() - rect.getY1() + delta.getY1(), 
               src, rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
      }

      /* Bottom border */
      rect = getBottomBorder();
      if(rect.isDefined())
      {
         delta = getBottomBorderDelta();
         fdraw->doStampFill(dest, wx1 + delta.getX1(),
               wy1 + height - delta.getY2() - (rect.getY2() - rect.getY1()),
               wx1 + width - delta.getX2(), 
               wy1 + height - delta.getY2(), src,
               rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
      }

      /* Left border */
      rect = getLeftBorder();
      if(rect.isDefined())
      {
         delta = getLeftBorderDelta();
         fdraw->doStampFill(dest, wx1 + delta.getX1(), wy1 + delta.getY1(),
               wx1 + delta.getX1() + (rect.getX2() - rect.getX1()), 
               wy1 + height - delta.getY2(), src,
               rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
      }

      /* Right border */
      rect = getRightBorder();
      if(rect.isDefined())
      {
         delta = getRightBorderDelta();
         fdraw->doStampFill(dest, 
               wx1 + width - delta.getX2() - (rect.getX2() - rect.getX1()),
               wy1 + delta.getY1(), wx1 + width - delta.getX2(), 
               wy1 + height - delta.getY2(), src,
               rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
      }
   }

   /* Corner */
   if(hasCorner())
   {
      delta = getCornerDelta();

      /* Top Left Corner */
      rect = getTopLeftCorner();
      if(rect.isDefined())
      {
         fdraw->doStampFill(dest, wx1 + delta.getX1(), wy1 + delta.getY1(),
               wx1 + delta.getX1() + (rect.getX2() - rect.getX1()),
               wy1 + delta.getY1() + (rect.getY2() - rect.getY1()), src,
               rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
      }

      /* Top Right Corner */
      rect = getTopRightCorner();
      if(rect.isDefined())
      {
         fdraw->doStampFill(dest, 
               wx1 + width - delta.getX2() - (rect.getX2() - rect.getX1()),
               wy1 + delta.getY1(), wx1 + width - delta.getX2(),
               wy1 + delta.getY1() + (rect.getY2() - rect.getY1()), src,
               rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
      }

      /* Bottom Left Corner */
      rect = getBottomLeftCorner();
      if(rect.isDefined())
      {
         fdraw->doStampFill(dest, wx1 + delta.getX1(),
               wy1 + height - delta.getY2() - (rect.getY2() - rect.getY1()),
               wx1 + delta.getX1() + (rect.getX2() - rect.getX1()),
               wy1 + height - delta.getY2(), src,
               rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
      }

      /* Bottom Right Corner */
      rect = getBottomRightCorner();
      if(rect.isDefined())
      {
         fdraw->doStampFill(dest, 
               wx1 + width - delta.getX2() - (rect.getX2() - rect.getX1()),
               wy1 + height - delta.getY2() - (rect.getY2() - rect.getY1()),
               wx1 + width - delta.getX2(), wy1 + height - delta.getY2(), 
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
   /* Do the normal draw */
   draw(dest, src, wx1, wy1, wx2, wy2);

   if(textAreaDelta.isDefined())
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
      font->write(dest, area, caption);
   }
}

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
Skin::Skin(Kobold::String filename)
{
   surface = NULL;
   defaultFontSize = 10;
   this->filename = filename;
   load(filename);
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
}

/***********************************************************************
 *                              getFileName                            *
 ***********************************************************************/
Kobold::String Skin::getFileName()
{
   return filename;
}

/***********************************************************************
 *                            getSkinElement                           *
 ***********************************************************************/
Skin::SkinElement Skin::getSkinElement(SkinElementType type)
{
   return elements[type]; 
}

/***********************************************************************
 *                                load                                 *
 ***********************************************************************/
void Skin::load(Kobold::String filename)
{
   Kobold::DefParser def;

   if(!def.load(filename))
   {
      return;
   }

   Kobold::String key, value;
   int cur = -1; //current element

   while(def.getNextTuple(key, value))
   {
      if(key == SKIN_KEY_ATLAS)
      {
         if(surface == NULL)
         {
            surface = Controller::loadImageToSurface(value); 
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
            break;
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

            /* Define element data */
            if(key == SKIN_KEY_BACKGROUND_DELTA)
            {
               elements[cur].getBackgroundDelta().set(x1, y1, x2, y2);
            }
            else if(key == SKIN_KEY_BACKGROUND)
            {
               elements[cur].getBackground().set(x1, y1, x2, y2);
            }
            else if(key == SKIN_KEY_BORDER_DELTA)
            {
               elements[cur].getBorderDelta().set(x1, y1, x2, y2);
            }
            else if(key == SKIN_KEY_TOP_BORDER_DELTA)
            {
               elements[cur].getTopBorderDelta(false).set(x1, y1, x2, y2);
            }
            else if(key == SKIN_KEY_BOTTOM_BORDER_DELTA)
            {
               elements[cur].getBottomBorderDelta(false).set(x1, y1, x2, y2);
            }
            else if(key == SKIN_KEY_LEFT_BORDER_DELTA)
            {
               elements[cur].getLeftBorderDelta(false).set(x1, y1, x2, y2);
            }
            else if(key == SKIN_KEY_RIGHT_BORDER_DELTA)
            {
               elements[cur].getRightBorderDelta(false).set(x1, y1, x2, y2);
            }
            else if(key == SKIN_KEY_LEFT_BORDER)
            {
               elements[cur].getLeftBorder().set(x1, y1, x2, y2);
            }
            else if(key == SKIN_KEY_RIGHT_BORDER)
            {
               elements[cur].getRightBorder().set(x1, y1, x2, y2);
            }
            else if(key == SKIN_KEY_TOP_BORDER)
            {
               elements[cur].getTopBorder().set(x1, y1, x2, y2);
            }
            else if(key == SKIN_KEY_BOTTOM_BORDER)
            {
               elements[cur].getBottomBorder().set(x1, y1, x2, y2);
            }
            else if(key == SKIN_KEY_CORNER_DELTA)
            {
               elements[cur].getCornerDelta().set(x1, y1, x2, y2);
            }
            else if(key == SKIN_KEY_TOP_LEFT_CORNER)
            {
               elements[cur].getTopLeftCorner().set(x1, y1, x2, y2);
            }
            else if(key == SKIN_KEY_TOP_RIGHT_CORNER)
            {
               elements[cur].getTopRightCorner().set(x1, y1, x2, y2);
            }
            else if(key == SKIN_KEY_BOTTOM_LEFT_CORNER)
            {
               elements[cur].getBottomLeftCorner().set(x1, y1, x2, y2);
            }
            else if(key == SKIN_KEY_BOTTOM_RIGHT_CORNER)
            {
               elements[cur].getBottomRightCorner().set(x1, y1, x2, y2);
            }
            else if(key == SKIN_KEY_TEXT_AREA_DELTA)
            {
               elements[cur].getTextAreaDelta().set(x1, y1, x2, y2);
            }
            else
            {
               Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
                  "ERROR: Unknown key '%s' at skin: '%s'",
                  key.c_str(), filename.c_str());
            }
         }
      }
   }
}

/***********************************************************************
 *                            getWidgetType                            *
 ***********************************************************************/
Skin::SkinElementType Skin::getElementType(Kobold::String typeName)
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
   Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
      "ERROR: Unknow widget name '%s' on screen definition file!",
      typeName.c_str());

   /* Unknown widget. let's overhide window to make sure
    * the user view that something is wrong with the skin definition.*/
   return SKIN_TYPE_WINDOW;
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
void Skin::drawElement(Surface* dest, SkinElementType type, 
      int wx1, int wy1, int wx2, int wy2)
{
   getSkinElement(type).draw(dest, surface, wx1, wy1, wx2, wy2); 
}
void Skin::drawElement(Surface* dest, SkinElementType type, 
      int wx1, int wy1, int wx2, int wy2, Rect bounds, Kobold::String caption)
{
   getSkinElement(type).draw(dest, surface, wx1, wy1, wx2, wy2, bounds, 
         caption);
}

/***********************************************************************
 *                          isElementDefined                           *
 ***********************************************************************/
bool Skin::isElementDefined(SkinElementType type)
{
   return getSkinElement(type).isDefined();
}

