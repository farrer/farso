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

#include "container.h"

#include "skin.h"
#include "controller.h"

#include <assert.h>
using namespace Farso;

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
Container::Container(ContainerType type, Widget* parent)
          :Widget(WIDGET_TYPE_CONTAINER, parent),
           distance(0, 0, 0, 0)
{
   assert(parent != NULL);
   contType = type;
   setSize(parent->getBody().getWidth(), parent->getBody().getHeight());
   body.set(getX(), getX(), getX() + getWidth() - 1, getX() + getHeight() - 1);
   dynamicSize = true;
   filled = false;
}

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
Container::Container(ContainerType type, Rect rect, Widget* parent)
          :Widget(WIDGET_TYPE_CONTAINER, parent),
           distance(rect)
{
   assert(parent != NULL);
   contType = type;

   setPosition(distance.getX1(), distance.getY1());
   setSize(parent->getBody().getWidth() - distance.getX1() - distance.getX2(), 
           parent->getBody().getHeight()- distance.getY1() - distance.getY2());

   body.set(getX(), getX(), getX() + getWidth() - 1, getX() + getHeight() - 1);
   dynamicSize = true;
   filled = false;
}

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
Container::Container(ContainerType type, int x, int y, 
      int width, int height, Widget* parent)
          :Widget(WIDGET_TYPE_CONTAINER, x, y, width, height, parent)
{
   contType = type;
   setPosition(x, y);
   setSize(width, height);
   body.set(getX(), getY(), getX() + width - 1, getY() + height - 1);
   dynamicSize = false;
   filled = false;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Container::~Container()
{
}

/***********************************************************************
 *                               setFilled                             *
 ***********************************************************************/
void Container::setFilled()
{
   filled = true;
}

/***********************************************************************
 *                               getChildX                             *
 ***********************************************************************/
int Container::getChildX(int x, int width)
{
   switch(contType)
   {
      case TYPE_TOP_LEFT:
      case TYPE_BOTTOM_LEFT:
         return x;
      case TYPE_TOP_RIGHT:
      case TYPE_BOTTOM_RIGHT:
         return getWidth() - width - x;
      case TYPE_TOP_CENTERED:
      case TYPE_BOTTOM_CENTERED:
         return ((getWidth() - width) / 2) - x;
   }

   return x;
}

/***********************************************************************
 *                               getChildY                             *
 ***********************************************************************/
int Container::getChildY(int y, int height)
{
   switch(contType)
   {
      case TYPE_TOP_LEFT:
      case TYPE_TOP_RIGHT:
      case TYPE_TOP_CENTERED:
         return y;
      break;
      case TYPE_BOTTOM_CENTERED:
      case TYPE_BOTTOM_LEFT:
      case TYPE_BOTTOM_RIGHT:
         return getHeight() - height - y;
      break;
   }

   return y;
}

/***********************************************************************
 *                               getBody                               *
 ***********************************************************************/
Farso::Rect Container::getBody()
{
   return body;
}

/***********************************************************************
 *                          getContainerType                           *
 ***********************************************************************/
Container::ContainerType Container::getContainerType()
{
   return contType;
}

/***********************************************************************
 *                               doDraw                                *
 ***********************************************************************/
void Container::doDraw(Rect pBody)
{
   if((dynamicSize) && (getParent() != NULL))
   {
      setSize(getParent()->getBody().getWidth() - distance.getX1() - 
              distance.getX2(), getParent()->getBody().getHeight() - 
              distance.getY1() - distance.getY2());
   }
   /* Reset body to allow skin changes on the fly */
   body.set(getX(), getY(), 
            getX() + getWidth() - 1, getY() + getHeight() - 1);

   if(filled)
   {
      Farso::Skin* skin = Farso::Controller::getSkin();
      Farso::Surface* surface = getWidgetRenderer()->getSurface();
      
      int xt = getX();
      int yt = getY();

      int rx1 = pBody.getX1() + xt;
      int ry1 = pBody.getY1() + yt;
      int rx2 = pBody.getX1() + xt + getWidth() - 1;
      int ry2 = pBody.getY1() + yt + getHeight() - 1;

      if(skin != NULL)
      {
         skin->drawElement(surface, Skin::SKIN_TYPE_BUTTON_ENABLED, 
               rx1, ry1, rx2, ry2);
      }
      else
      {
         Farso::Draw* draw = Farso::Controller::getDraw();

         /* Draw the background */
         draw->setActiveColor(Colors::colorButton);
         draw->doFilledRectangle(surface, rx1+1, ry1+1, rx2-1, ry2-1);

         /* Draw the edges */
         draw->setActiveColor(Colors::colorCont[0]);
         draw->doTwoColorsRectangle(surface, rx1, ry1, rx2, ry2,
               Colors::colorCont[1]); 
      }
   }
}

/***********************************************************************
 *                              doTreat                                *
 ***********************************************************************/
bool Container::doTreat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY, int mrX, int mrY)
{
   return false;
}

/***********************************************************************
 *                         doAfterChildTreat                           *
 ***********************************************************************/
void Container::doAfterChildTreat()
{
}

/***********************************************************************
 *                             setDirty                                *
 ***********************************************************************/
void Container::setDirty()
{
   /* As a not filled container, if someone marked us as dirty, we must also
    * mark our parent, as we don't know how to render our background. */
   if((!filled) && (getParent() != NULL))
   {
      getParent()->setDirty();
   }

   setDirtyValue(true);
}

