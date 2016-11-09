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

#include "labelledpicture.h"
#include "controller.h"

using namespace Farso;

/************************************************************************
 *                             Constructor                              *
 ************************************************************************/
LabelledPicture::LabelledPicture(int x, int y, int width, int height, 
                                 Kobold::String caption, 
                                 Kobold::String imageFile, 
                                 Farso::Color frontColor, 
                                 Farso::Color outlineColor,
                                 int outlineWidth, Widget* parent)
                :Widget(Widget::WIDGET_TYPE_LABELLED_PICTURE, x, y, 
                        width, height, parent)
{
   body.set(getX(), getY(), getX() + getWidth() - 1, getY() + getHeight() - 1);
   
   /* Load and create the picture */
   picture = new Picture(0, 0, imageFile, this);

   /* Make sure picture fits. */
   assert((width >= picture->getWidth()) && (height >= picture->getHeight()));

   /* Let's adjust picture position to be centered on X, if needed */
   if(picture->getWidth() < width)
   {
      int pX = (width - picture->getWidth()) / 2;
      picture->setPosition(pX, 0);
   }

   /* Finally, let's define its label */
   label = new Label(0, picture->getHeight(), width - 1, 
                     height - picture->getHeight() - 1, caption, this);
   label->setFontColor(frontColor);
   label->setFontAlignment(Farso::Font::TEXT_CENTERED);
   label->setFontOutline(2, outlineColor);

   pressStarted = false;
}

/************************************************************************
 *                              Destructor                              *
 ************************************************************************/
LabelledPicture::~LabelledPicture()
{
}

/************************************************************************
 *                               getBody                                *
 ************************************************************************/
Farso::Rect LabelledPicture::getBody()
{
   return body;
}

/************************************************************************
 *                              setCaption                              *
 ************************************************************************/
void LabelledPicture::setCaption(Kobold::String text)
{
   Widget::setCaption(text);
   label->setCaption(text);
}

/************************************************************************
 *                             setFontSize                              *
 ************************************************************************/
void LabelledPicture::setFontSize(int size)
{
   label->setFontSize(size);
}

/************************************************************************
 *                               setDirty                               *
 ************************************************************************/
void LabelledPicture::setDirty()
{
   Widget::setDirty();
   if(getParent())
   {
      getParent()->setDirty();
   }
}

/************************************************************************
 *                               doDraw                                 *
 ************************************************************************/
void LabelledPicture::doDraw(Rect pBody)
{
   if(pressStarted)
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
bool LabelledPicture::doTreat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY, int mrX, int mrY)
{
   if(leftButtonPressed)
   { 
      if(isInner(mrX, mrY))
      {
         if(!pressStarted)
         {
            pressStarted = true;
            setDirty();
         }
         Controller::setEvent(this, EVENT_LABELLEDPICTURE_PRESSING);
         return true;
      }
   }
   else if(pressStarted)
   {
      pressStarted = false;
      setDirty();

      if(isInner(mrX, mrY))
      {
         Controller::setEvent(this, EVENT_LABELLEDPICTURE_CLICKED);
         return true;
      }
   }

   return false;
}

/************************************************************************
 *                          doAfterChildTreat                           *
 ************************************************************************/
void LabelledPicture::doAfterChildTreat()
{
}


