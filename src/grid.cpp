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

#include "grid.h"
#include "controller.h"
#include "cursor.h"
using namespace Farso;

/***********************************************************************
 *                                GridElement                          *
 ***********************************************************************/
Grid::GridElement::GridElement(int x, int y, int width, int height, 
                               Kobold::String mouseHint, int index)
                  :Kobold::ListElement(),
                   area(x, y, x + width - 1, y + height - 1)
{
   this->mouseHint = mouseHint;
   this->index = index;
}

/***********************************************************************
 *                               ~GridElement                          *
 ***********************************************************************/
Grid::GridElement::~GridElement()
{
}

/***********************************************************************
 *                                 isInner                             *
 ***********************************************************************/
bool Grid::GridElement::isInner(int x, int y)
{
   return (x >= area.getX1()) && (y >= area.getY1()) &&
          (x <= area.getX2()) && (y <= area.getY2());
}

/***********************************************************************
 *                                  getArea                            *
 ***********************************************************************/
Rect Grid::GridElement::getArea()
{
   return area;
}

/***********************************************************************
 *                               getMouseHint                          *
 ***********************************************************************/
Kobold::String Grid::GridElement::getMouseHint()
{
   return mouseHint;
}

/***********************************************************************
 *                               getIndex                              *
 ***********************************************************************/
int Grid::GridElement::getIndex()
{
   return index;
}

/***********************************************************************
 *                                  Grid                               *
 ***********************************************************************/
Grid::Grid(GridType gridType, Widget* parent)
     :Widget(Widget::WIDGET_TYPE_GRID,
             parent->getBody().getX1(), parent->getBody().getY1(),
             parent->getBody().getWidth(), parent->getBody().getHeight(),
             parent)
{
   /* Note that grid area will be fully parent's area  */
   this->body = Rect(getX(), getY(), getX() + parent->getBody().getWidth() - 1, 
                     getY() + parent->getBody().getHeight() -1);
   this->current  = NULL;
   this->gridType = gridType;
   this->pressStarted = false;
   this->curIndex = 0;
}

/***********************************************************************
 *                                  ~Grid                              *
 ***********************************************************************/
Grid::~Grid()
{
   clearElements();
}

/***********************************************************************
 *                              clearElements                          *
 ***********************************************************************/
void Grid::clearElements()
{
   while(elements.getFirst())
   {
      elements.remove(elements.getFirst());
   }
}

/***********************************************************************
 *                               addElement                            *
 ***********************************************************************/
Grid::GridElement* Grid::addElement(int x, int y, int width, int height, 
      Kobold::String mouseHint)
{
   GridElement* el = new GridElement(x, y, width, height, mouseHint, curIndex);
   elements.insert(el);

   curIndex++;

   return el;
}

/***********************************************************************
 *                                 getBody                             *
 ***********************************************************************/
Rect Grid::getBody()
{
   return body;
}

/***********************************************************************
 *                                getCurrent                           *
 ***********************************************************************/
Grid::GridElement* Grid::getCurrent()
{
   return current;
}

/***********************************************************************
 *                                 gridType                            *
 ***********************************************************************/
void Grid::setDirty()
{
   if(gridType != GRID_TYPE_HIGHLIGHT_NONE)
   {
      /* Must redraw the parent on changes, to redo its background */
      getParent()->setDirty();
   }

   Widget::setDirty();
}

/***********************************************************************
 *                                  doDraw                             *
 ***********************************************************************/
void Grid::doDraw(Rect pBody)
{
   /* Update body */
   body = Rect(getX(), getY(), 
               getX() + getParent()->getBody().getWidth() - 1, 
               getY() + getParent()->getBody().getHeight() -1);

   if(gridType == GRID_TYPE_HIGHLIGHT_NONE)
   {
      /* If no highlight for current element, no need to draw anything */
      return;
   }

   if(current)
   {
      Skin* skin = Controller::getSkin();
      Draw* draw = Controller::getDraw();
      Surface* surface = getWidgetRenderer()->getSurface();

      /* Define element's area within parent's body applied */
      Rect area = current->getArea();
      int x1 = area.getX1() + pBody.getX1();
      int y1 = area.getY1() + pBody.getY1();
      int x2 = x1 + area.getWidth() - 1;
      int y2 = y1 + area.getHeight() - 1;

      /* Must do the current selected highlighting */
      if(gridType == GRID_TYPE_HIGHLIGHT_BORDER)
      {
         if(skin)
         {
            skin->drawElement(surface, Skin::SKIN_TYPE_GRID_BORDER, 
                  x1, y1, x2, y2);
         }
         else
         {
            /* Draw a 'double-thick' rectangle for the element's area */
            draw->setActiveColor(Colors::colorHigh);
            draw->doRectangle(surface, x1, y1, x2, y2);
            draw->doRectangle(surface, x1 + 1, y1 + 1, x2 - 1, y2 - 1);
         }
      }
      else if(gridType == GRID_TYPE_HIGHLIGHT_FILL)
      {
         if(skin)
         {
            skin->drawElement(surface, Skin::SKIN_TYPE_GRID_FILL, 
                  x1, y1, x2, y2);
         }
         else
         {
            /* Fill a rectangle for the current element's area */
            draw->setActiveColor(Colors::colorHigh);
            draw->doFilledRectangle(surface, x1, y1, x2, y2);
         }
      }
   }
}

/***********************************************************************
 *                                  doTreat                            *
 ***********************************************************************/
bool Grid::doTreat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY, int mrX, int mrY)
{
   if(current)
   {
      /* Already have a selected element, let's check if remains selected */
      if(!current->isInner(mrX, mrY))
      {
         /* No longer selected */
         current = NULL;
         setDirty();
      }
      else
      {
         /* Let's set mouse hint, if defined for the element */
#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID &&\
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS
         if(!current->getMouseHint().empty())
         {
            Farso::Cursor::setTextualTip(current->getMouseHint());
         }
#endif

         /* Still over, let's check press */
         if(leftButtonPressed)
         {
            if(!pressStarted)
            {
               pressStarted = true;
            }
         } 
         else if(pressStarted)
         {
            /* No longer pressing (just released): selected event! */
            Controller::setEvent(this, EVENT_GRID_SELECTED_ELEMENT);
            pressStarted = false;
            return true;
         }

         /* No press - or not yet released -, just over element */
         if(leftButtonPressed)
         {
            Controller::setEvent(this, EVENT_GRID_CLICKING_ELEMENT);
            return true;
         }
      }
   }

   /* Without elements, press is no more started */
   pressStarted = false;
 
   /* Check if inner the grid */
   if(isInner(mrX, mrY))
   {
      GridElement* el = (GridElement*) elements.getFirst();
      Rect area;
      for(int i = 0; i < elements.getTotal(); i++)
      {
         /* Check if inner area */
         if(el->isInner(mrX, mrY))
         {
            current = el;
            /* Selected current, but no event for this. */
            return false;
         }

         el = (GridElement*) el->getNext();
      }
   }
  
   return false;
}

/***********************************************************************
 *                            doAfterChildTreat                        *
 ***********************************************************************/
void Grid::doAfterChildTreat()
{
}

