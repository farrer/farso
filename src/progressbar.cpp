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

#include "progressbar.h"
#include "controller.h"
using namespace Farso;

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
ProgressBar::ProgressBar(int x, int y, int width, int height, Widget* parent)
            :Widget(WIDGET_TYPE_PROGRESS_BAR, x, y, width, height, parent),
             color(255, 10, 10, 255),
             body(x, y, x + width - 1, y + height -1)
{
   curValue = 0;
   maxValue = 100;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
ProgressBar::~ProgressBar()
{
}

/***********************************************************************
 *                             setMaxValue                             *
 ***********************************************************************/
void ProgressBar::setMaxValue(int max)
{
   maxValue = max;
   setDirty();
}

/***********************************************************************
 *                               setValue                              *
 ***********************************************************************/
void ProgressBar::setValue(int val)
{
   if(val <= maxValue)
   {
      curValue = val;
   }
   else 
   {
      curValue = maxValue;
   }
   setDirty();
}

/***********************************************************************
 *                               setColor                              *
 ***********************************************************************/
void ProgressBar::setColor(Color color)
{
   this->color = color;
   setDirty();
}

/***********************************************************************
 *                               getBody                               *
 ***********************************************************************/
Rect ProgressBar::getBody()
{
   return body;
}

/***********************************************************************
 *                                doDraw                               *
 ***********************************************************************/
void ProgressBar::doDraw(Rect pBody)
{
   Farso::Draw* draw = Controller::getDraw();
   Farso::Skin* skin = Controller::getSkin();
   Farso::Surface* surface = getWidgetRenderer()->getSurface();

   int x1 = pBody.getX1() + getX();
   int y1 = pBody.getY1() + getY();
   int x2 = x1 + getWidth() - 1;
   int y2 = y1 + getHeight() - 1;

   float perc = curValue / (float)maxValue;

   if(skin != NULL)
   {
      /* draw progress bar background */
      skin->drawElement(surface, Skin::SKIN_TYPE_PROGRESS_BAR, 
            x1, y1, x2, y2);
      if(curValue > 0)
      {
         /* draw filler up to the current filled percentual */
         int coloredWidth = (int) (perc * (getWidth()));

         /* Make sure the minimun size is respected, to avoid errors
          * of sobreposition when bliting/stamping the image */
         Rect minSize = skin->getSkinElement(
               Skin::SKIN_TYPE_PROGRESS_BAR_FILL).getMinSize();
         if(coloredWidth < minSize.getWidth())
         {
            coloredWidth = minSize.getWidth();
         }

         skin->drawElement(surface, Skin::SKIN_TYPE_PROGRESS_BAR_FILL, 
               x1, y1, x1 + coloredWidth, y2);
      }
   }
   else
   {
      int coloredX2 = (int) (perc * (x2 - x1 - 6)) + x1 + 3;
      draw->setActiveColor(Colors::colorCont[0]);
      draw->doTwoColorsRectangle(surface, x1, y1, x2, y2,
            Colors::colorCont[1]);
      draw->setActiveColor(Colors::colorText);
      draw->doFilledRectangle(surface, x1 + 1, y1 + 1, x2 - 1, y2 - 1); 
      if(perc > 0)
      {
         draw->setActiveColor(color);
         draw->doFilledRectangle(surface, x1 + 3, y1 + 3, coloredX2, y2 - 3);
      }
   }
}

/***********************************************************************
 *                               doTreat                               *
 ***********************************************************************/
bool ProgressBar::doTreat(bool leftButtonPressed, bool rightButtonPressed,
      int mouseX, int mouseY, int mrX, int mrY)
{
   /* No events happens on a progress bar */
   return false;
}

/***********************************************************************
 *                          doAfterChildTreat                          *
 ***********************************************************************/
void ProgressBar::doAfterChildTreat()
{
}


