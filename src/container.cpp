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
#include <assert.h>
using namespace Farso;

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
Container::Container(ContainerType type, Widget* parent)
          :Widget(WIDGET_TYPE_CONTAINER, parent)
{
   assert(parent != NULL);
   contType = type;
   setSize(parent->getBody().getWidth(), parent->getBody().getHeight());
   body.set(getX(), getX(), getX() + getWidth() - 1, getX() + getHeight() - 1);
   dynamicSize = true;
}

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
Container::Container(ContainerType type, int x, int y, int width, int height, 
      Widget* parent)
          :Widget(WIDGET_TYPE_CONTAINER, x, y, width, height, parent)
{
   contType = type;
   setPosition(x, y);
   setSize(width, height);
   body.set(getX(), getY(), getX() + width - 1, getY() + height - 1);
   dynamicSize = false;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
Container::~Container()
{
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
Rect Container::getBody()
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
      setSize(getParent()->getBody().getWidth(), 
              getParent()->getBody().getHeight());
   }
   /* Reset body to allow skin changes on the fly */
   body.set(getX(), getY(), 
            getX() + getWidth() - 1, getY() + getHeight() - 1);
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
   /* As a container, if someone marked us as dirty, we must also mark
    * our parent, as we don't know how to render our background. */
   if(getParent() != NULL)
   {
      getParent()->setDirty();
   }

   setDirtyValue(true);
}

