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

#include "picture.h"
#include "controller.h"

#include <assert.h>
using namespace Farso;

/************************************************************************
 *                              Constructor                             *
 ************************************************************************/
Picture::Picture(int x, int y, const Kobold::String& filename, Widget* parent)
        :Widget(WIDGET_TYPE_PICTURE, parent) 
{
   ownImage = true;

   /* Load the image */
   image = Controller::loadImageToSurface(
         Controller::getRealFilename(filename));
   
   /* Set size based on loaded image */
   setSize(image->getWidth(), image->getHeight());

   /* define its position */
   setPosition(x, y);

   body.set(getX(), getY(), getX() + getWidth() - 1, getY() + getHeight() - 1);
}

/************************************************************************
 *                              Constructor                             *
 ************************************************************************/
Picture::Picture(int x, int y, int width, int height, Widget* parent)
        :Widget(WIDGET_TYPE_PICTURE, x, y, width, height, parent) 
{
   ownImage = false;
   image = NULL; 
   body.set(getX(), getY(), getX() + getWidth() - 1, getY() + getHeight() - 1);
}

/************************************************************************
 *                               Destructor                             *
 ************************************************************************/
Picture::~Picture()
{
   if((ownImage) && (image != NULL))
   {
      delete image;
   }
}

/************************************************************************
 *                             setImage                                 *
 ************************************************************************/
void Picture::setImage(Farso::Surface* picture)
{
   if((ownImage) && (image != NULL))
   {
      /* Must free current loaded one, as it's owned by us. */
      delete image;
   }

   image = picture;
   ownImage = false;

   setDirty();
}

/************************************************************************
 *                             setImage                                 *
 ************************************************************************/
void Picture::setImage(const Kobold::String& filename)
{
   if((ownImage) && (image != NULL))
   {
      delete image;
   }

   /* Load the image */
   image = Controller::loadImageToSurface(
         Controller::getRealFilename(filename));
   ownImage = true;

   assert(image->getWidth() <= getWidth());
   assert(image->getHeight() <= getHeight());

   setDirty();
}

/************************************************************************
 *                                doDraw                                *
 ************************************************************************/
void Picture::doDraw(const Rect& pBody)
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
const Farso::Rect& Picture::getBody()
{
   return body;
}

/************************************************************************
 *                               setDirty                               *
 ************************************************************************/
void Picture::setDirty()
{
   setDirtyWithParent();
}

