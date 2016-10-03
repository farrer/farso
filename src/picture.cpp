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

#include "picture.h"
#include "controller.h"

#include <assert.h>
using namespace Farso;

/************************************************************************
 *                              Constructor                             *
 ************************************************************************/
Picture::Picture(int x, int y, Kobold::String filename, Widget* parent)
        :Widget(WIDGET_TYPE_PICTURE, x, y, 0, 0, parent) 
{
   assert(parent != NULL);
   /* Initialize variables */
   pressStarted = false;
   /* Load the image */
   image = Controller::loadImageToSurface(filename);
   /* Set widget coordinates and size based on loaded image */
   setPosition(x, y);
   setSize(image->getWidth(), image->getHeight());

   body.set(getX(), getY(), getX() + getWidth() - 1, getY() + getHeight() - 1);
}

/************************************************************************
 *                               Destructor                             *
 ************************************************************************/
Picture::~Picture()
{
   delete image;
}

/************************************************************************
 *                                doDraw                                *
 ************************************************************************/
void Picture::doDraw(Rect pBody)
{
   Draw* draw = Farso::Controller::getDraw();
   image->lock();
   draw->doStampFill(getWidgetRenderer()->getSurface(), 
         pBody.getX1() + getX(), pBody.getY1() + getY(), 
         pBody.getX1() + getX() + getWidth() - 1, 
         pBody.getY1() + getY() + getHeight() - 1,
         image, 0, 0, image->getWidth() - 1, image->getHeight() -1);
   image->unlock();
}

/************************************************************************
 *                               doTreat                                *
 ************************************************************************/
bool Picture::doTreat(bool leftButtonPressed, bool rightButtonPressed,
      int mouseX, int mouseY, int mrX, int mrY)
{
   if(leftButtonPressed)
   {
      if(isInner(mrX, mrY))
      {
         if(!pressStarted)
         {
            pressStarted = true;
         }
         Farso::Controller::setEvent(this, EVENT_PICTURE_PRESSING);
         return true;
      }
   }
   else if(pressStarted)
   {
      pressStarted = false;
      if(isInner(mrX, mrY))
      {
         Farso::Controller::setEvent(this, EVENT_PICTURE_CLICKED);
         return true;
      }
   }
   return false;
}

/************************************************************************
 *                           doAfterChildTreat                          *
 ************************************************************************/
void Picture::doAfterChildTreat()
{
}

/************************************************************************
 *                                getBody                               *
 ************************************************************************/
Rect Picture::getBody()
{
   return body;
}

