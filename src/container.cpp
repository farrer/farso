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

#include "container.h"
using namespace Farso;

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
Container::Container(ContainerType type, Widget* parent)
          :Widget(WIDGET_TYPE_CONTAINER, 0, 0, 0, 0, parent)
{
   assert(parent != NULL);
   contType = type;
   setSize(parent->getWidth(), parent->getHeight());
   body.set(0, 0, parent->getWidth() - 1, parent->getHeight() - 1);
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
   setSize(parent->getBody().getWidth(), parent->getBody().getHeight());
   body.set(0, 0, width, height);
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
      break;
      case TYPE_TOP_RIGHT:
      case TYPE_BOTTOM_RIGHT:
         return getWidth() - width - x;
      break;
      case TYPE_TOP_CENTERED:
      case TYPE_BOTTOM_CENTERED:
         return ((getWidth() - width) / 2) - x;
      break;
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
 *                               doDraw                                *
 ***********************************************************************/
void Container::doDraw(Rect pBody)
{
   if((dynamicSize) && (getParent() != NULL))
   {
      setSize(getParent()->getBody().getWidth(), 
              getParent()->getBody().getHeight());
      body.set(0, 0, getParent()->getWidth(), getParent()->getHeight());
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
   /* As a container, if someone marked us as dirty, we must also mark
    * our parent, as we don't know how to render our background. */
   if(getParent() != NULL)
   {
      getParent()->setDirty();
   }

   setDirtyValue(true);
}

