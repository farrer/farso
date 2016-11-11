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

#include "clickablepicture.h"
#include "controller.h"

using namespace Farso;

/************************************************************************
 *                             Constructor                              *
 ************************************************************************/
ClickablePicture::ClickablePicture(int x, int y, 
                                   Kobold::String imageFile, Widget* parent)
                :Widget(Widget::WIDGET_TYPE_CLICKABLE_PICTURE, parent)
{
   pressStarted = false;
   useBorder = false;

   /* Load and create the picture */
   picture = new Picture(0, 0, imageFile, this);
   
   /* Set dimension according to loaded picture */
   setSize(picture->getWidth(), picture->getHeight());
   setPosition(x, y);
   body.set(getX(), getY(), getX() + getWidth() - 1, getY() + getHeight() - 1);
}

/************************************************************************
 *                              Destructor                              *
 ************************************************************************/
ClickablePicture::~ClickablePicture()
{
}

/************************************************************************
 *                               getBody                                *
 ************************************************************************/
Farso::Rect ClickablePicture::getBody()
{
   return body;
}

/************************************************************************
 *                      enableBorderWhenClicked                         *
 ************************************************************************/
void ClickablePicture::enableBorderWhenClicked()
{
   if(pressStarted && !useBorder)
   {
      setDirty();
   }

   useBorder = true;
}

/************************************************************************
 *                     disableBorderWhenClicked                         *
 ************************************************************************/
void ClickablePicture::disableBorderWhenClicked()
{
   if(pressStarted && useBorder)
   {
      setDirty();
   }

   useBorder = false;
}

/************************************************************************
 *                               setDirty                               *
 ************************************************************************/
void ClickablePicture::setDirty()
{
   if(getParent())
   {
      getParent()->setDirty();
   }
   Widget::setDirty();
}

/************************************************************************
 *                               doDraw                                 *
 ************************************************************************/
void ClickablePicture::doDraw(Rect pBody)
{
   if(pressStarted && useBorder)
   {
      Skin* skin = Controller::getSkin();
      Draw* draw = Controller::getDraw();
      Surface* surface = getWidgetRenderer()->getSurface();

      /* Must draw a border */
      int x1 = getX() + pBody.getX1();
      int y1 = getY() + pBody.getY1();
      int x2 = x1 + getWidth() - 1;
      int y2 = y1 + getHeight() - 1;
      if(skin)
      {
         skin->drawElement(surface, Skin::SKIN_TYPE_GRID_BORDER,
               x1, y1, x2, y2);
      }
      else
      {
         draw->setActiveColor(Colors::colorHigh);
         draw->doRectangle(surface, x1, y1, x2, y2);
         draw->doRectangle(surface, x1 + 1, y1 + 1, x2 - 1, y2 - 1);
      }
   }
}

/************************************************************************
 *                                doTreat                               *
 ************************************************************************/
bool ClickablePicture::doTreat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY, int mrX, int mrY)
{
   if(leftButtonPressed)
   { 
      if(isInner(mrX, mrY))
      {
         if(!pressStarted)
         {
            pressStarted = true;
            if(useBorder)
            {
               setDirty();
            }
         }
         Controller::setEvent(this, EVENT_CLICKABLEPICTURE_PRESSING);
         return true;
      }
   }
   else if(pressStarted)
   {
      pressStarted = false;
      if(useBorder)
      {
         setDirty();
      }

      if(isInner(mrX, mrY))
      {
         Controller::setEvent(this, EVENT_CLICKABLEPICTURE_CLICKED);
         return true;
      }
   }

   return false;
}

/************************************************************************
 *                          doAfterChildTreat                           *
 ************************************************************************/
void ClickablePicture::doAfterChildTreat()
{
}


