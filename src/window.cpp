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

#include "window.h"
#include "controller.h"
#include "font.h"
#include <assert.h>
using namespace Farso;

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
Window::Window(int width, int height, Kobold::String title)
      : Widget(Widget::WIDGET_TYPE_WINDOW, 0, 0, width, height, NULL)
{
   setCaption(title);

   this->active = false;
   this->modal = false;
   this->moveable = true;
   this->closeable = true;
   this->externPointer = NULL;
   this->moveStarted = false;
   this->willActivate = false;
   this->clickedInnerWindow = false;
   this->closeButton = new Button(0, -20, 18, 18, "C", this);
   this->closeButton->setSkinTypes(Skin::SKIN_TYPE_WINDOW_ENABLED_CLOSE_BUTTON,
         Skin::SKIN_TYPE_WINDOW_PRESSED_CLOSE_BUTTON,
         Skin::SKIN_TYPE_WINDOW_ON_OVER_CLOSE_BUTTON,
         Skin::SKIN_TYPE_WINDOW_DISABLED_CLOSE_BUTTON);

   defineBody();
   hide();
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
Window::~Window()
{
   /* Note: there's no need to delete closeButton, as it will be deleted
    * at the children list destructor. */

   if(active)
   {
      active = false;
      /* Tell controller, if it already not know. */
      if(Controller::getActiveWindow() == this)
      {
         Controller::setActiveWindow(NULL);
      }
   }


   /* Clear extern pointer, if any */
   if(externPointer != NULL)
   {
      *externPointer = NULL;
   }
}

/***********************************************************************
 *                               getBody                               *
 ***********************************************************************/
Farso::Rect Window::getBody()
{
   return body;
}

/***********************************************************************
 *                            defineBody                               *
 ***********************************************************************/
void Window::defineBody()
{
   Farso::Skin* skin = Farso::Controller::getSkin();

   if(skin)
   {
      /* Define Body: just bellow titleBar, and within borders. */
      Rect insideBorder = skin->getSkinElement(
            Farso::Skin::SKIN_TYPE_WINDOW).getBody(getWidth(), getHeight());

      body.set(insideBorder.getX1(), 
               (insideBorder.getY1() > titleBar.getY2()) ? 
                  insideBorder.getY1() :
                  (titleBar.getY2() + 1), 
               insideBorder.getX2(), 
               insideBorder.getY2());
   }
   else
   {
      body.set(3, 23, getWidth()-4, getHeight()-4);
   }
}

/***********************************************************************
 *                               doDraw                                *
 ***********************************************************************/
void Window::doDraw(Rect pBody)
{
   /* Note: as window never has parents, pBody should be ignored here. */
   Farso::Skin* skin = Farso::Controller::getSkin();
   Farso::Surface* surface = getWidgetRenderer()->getSurface();

   int x1 = getX();
   int y1 = getY();
   int x2 = getX() + getWidth() - 1;
   int y2 = getY() + getHeight() - 1;

   /* Redefine body, for dynamic skin changes */
   defineBody();

   if(skin != NULL)
   {
      /* Must draw the window using current skin */
      skin->drawElement(surface, Farso::Skin::SKIN_TYPE_WINDOW,
            x1, y1, x2, y2);
      
      if(active)
      {
         /* Define the title bar rectangle */
         titleBar = skin->getSkinElement(
               Farso::Skin::SKIN_TYPE_WINDOW_ACTIVE_TITLE_BAR).getBounds(
                  getWidth(), getHeight());
         /* Draw it with title */
         skin->drawElement(surface, 
               Farso::Skin::SKIN_TYPE_WINDOW_ACTIVE_TITLE_BAR, x1, y1, x2, y2,
               titleBar, getCaption());
      }
      else
      {
         /* Define the title bar rectangle */
         titleBar = skin->getSkinElement(
               Farso::Skin::SKIN_TYPE_WINDOW_INACTIVE_TITLE_BAR).getBounds(
                  getWidth(), getHeight());
         /* Write inactive title bar with caption */
         skin->drawElement(surface, 
               Farso::Skin::SKIN_TYPE_WINDOW_INACTIVE_TITLE_BAR, 
               x1, y1, x2, y2, titleBar, getCaption());
      }

      /* Define close button position */
      Farso::Skin::SkinElement sk = skin->getSkinElement(
            Farso::Skin::SKIN_TYPE_WINDOW_ENABLED_CLOSE_BUTTON);
      if(sk.hasCorner())
      {
         Rect delta = sk.getCornerDelta();
         Rect rect = sk.getTopLeftCorner();
         if(rect.isDefined())
         {
            closeButton->setPosition(delta.getX1() - body.getX1(), 
                  delta.getY1() - body.getY1());
            closeButton->setSize(rect.getWidth(), rect.getHeight());
         }
         rect = sk.getTopRightCorner();
         if(rect.isDefined())
         {
            closeButton->setPosition((getWidth() - 1) - delta.getX2() - 
                  (rect.getWidth() - 1) - body.getX1(), 
                  delta.getY1() - body.getY1());
            closeButton->setSize(rect.getWidth(), rect.getHeight());
         }

      }
   }
   else
   {
      /* No skin is used, must draw with default style. */
      Farso::Draw* draw = Farso::Controller::getDraw();

      /* Define close button position */
      closeButton->setPosition(0, -20);
      closeButton->setSize(18, 18);

      /* Window background */
      draw->setActiveColor(Colors::colorWindow);
      draw->doFilledRectangle(surface, 3, 3, getWidth() - 3, getHeight() - 3);
      
      /* Window contorns */
      draw->setActiveColor(Colors::colorCont[0]);
      draw->doRectangle(surface, 0, 0, getWidth()-1, getHeight()-1);
      draw->setActiveColor(Colors::colorButton);
      draw->doRectangle(surface, 1, 1, getWidth()-2, getHeight()-2);
      draw->setActiveColor(Colors::colorCont[2]);
      draw->doLine(surface, 2, 22, getWidth()-4, 22);
      draw->setActiveColor(Colors::colorCont[0]);
      draw->doTwoColorsRectangle(surface, 2, 2, getWidth()-3, getHeight()-3, 
            Colors::colorCont[2]);

      /* Window title bar */
      if(active)
      {
         draw->setActiveColor(Colors::colorBar);
         draw->doFilledRectangle(surface, 21, 3, getWidth()-3, 21);
      }
      /* Define the title bar set */
      titleBar.set(21, 3, getWidth() - 3, 21);

      /* Window title text */
      draw->setActiveColor(Colors::colorText);

      Farso::Font* font = Farso::FontManager::getDefaultFont();
      assert(font != NULL);
      font->setSize(10);
      font->setAlignment(Font::TEXT_LEFT);
      font->write(surface, titleBar, getCaption());
   }
}

/***********************************************************************
 *                              activate                               *
 ***********************************************************************/
void Window::activate()
{
   if(!active)
   {
      active = true;
      setDirty();
      /* Tell controller, if needed */
      if(Controller::getActiveWindow() != this)
      {
         Controller::setActiveWindow(this);
      }
   }
}

/***********************************************************************
 *                             inactivate                              *
 ***********************************************************************/
void Window::inactivate()
{
   if(active)
   {
      active = false;
      setDirty();
      /* Tell controller, if it already not know. */
      if(Controller::getActiveWindow() == this)
      {
         Controller::setActiveWindow(NULL);
      }
   }
}

/***********************************************************************
 *                               isActive                              *
 ***********************************************************************/
bool Window::isActive()
{
   return active;
}

/***********************************************************************
 *                                open                                 *
 ***********************************************************************/
void Window::open()
{
   show();
   activate();

   Farso::Skin* skin = Farso::Controller::getSkin();
   Farso::Surface* surface = getWidgetRenderer()->getSurface();

   if(skin != NULL)
   {
      skin->getSurface()->lock();
   }
   surface->lock();
   
   draw();

   if(skin != NULL)
   {
      skin->getSurface()->unlock();
   }
   surface->unlock();
}

/***********************************************************************
 *                             isCloseable                             *
 ***********************************************************************/
bool Window::isCloseable()
{
   return closeable;
}

/***********************************************************************
 *                              isMoveable                             *
 ***********************************************************************/
bool Window::isMoveable()
{
   return moveable;
}

/***********************************************************************
 *                                isModal                              *
 ***********************************************************************/
bool Window::isModal()
{
   return modal;
}

/***********************************************************************
 *                               setModal                              *
 ***********************************************************************/
void Window::setModal()
{
   modal = true;
}

/***********************************************************************
 *                           setExternPointer                          *
 ***********************************************************************/
void Window::setExternPointer(Window** extr)
{
   externPointer = extr;
}

/***********************************************************************
 *                                treat                                *
 ***********************************************************************/
bool Window::doTreat(bool leftButtonPressed, bool rightButtonPressed,
            int mouseX, int mouseY, int mrX, int mrY)
{
   clickedInnerWindow = false;
   if(leftButtonPressed)
   {
      clickedInnerWindow = isInner(mrX, mrY);
      
      if(!active)
      {
         /* Check if mouse is inner the window */
         if(clickedInnerWindow)
         {
            willActivate = true;
            Controller::setEvent(this, EVENT_WINDOW_WILL_ACTIVATE);
            return true;
         }
      }
      if(!moveStarted)
      {
         if((titleBar.isInner(mrX, mrY)))
         {
            /* Must define the current difference */
            moveStarted = true;
            clickDiff[0] = mrX;
            clickDiff[1] = mrY;
            Controller::setEvent(this, EVENT_WINDOW_MOVE_INIT);
            return true;
         }
      }
      else
      {
         /* Move already started, let's get the new window's position */

         /* Calculate new window coordinate with initial click on window
          * difference (the reference point is the initial point clicked) */
         getWidgetRenderer()->setPosition(mouseX - clickDiff[0], 
               mouseY - clickDiff[1]);
         Controller::setEvent(this, EVENT_WINDOW_MOVING);
         return true;
      }

   }
   else if(willActivate)
   {
      /* Activate the window (after releasing the click) */
      willActivate = false;
      activate();
      Controller::setEvent(this, EVENT_WINDOW_MOVE_END);
      return true;
   }
   else if(moveStarted)
   {
      /* Stoped the screen movement. */
      moveStarted = false;
      Controller::setEvent(this, EVENT_WINDOW_ACTIVATED);
      return true;
   }

   return false;
}

/***********************************************************************
 *                          doAfterChildTreat                          *
 ***********************************************************************/
void Window::doAfterChildTreat()
{
   Event ev = Controller::getLastEvent();

   if((ev.getType() == EVENT_NONE) && (clickedInnerWindow))
   {
      /* Received a click on the window, but without any children
       * reaction to it. */
      Controller::setEvent(this, EVENT_WINDOW_CLICKED);
   }
   else if((ev.getType() == EVENT_BUTTON_RELEASE) && 
           (ev.getWidget() == closeButton))
   {
      Controller::markToRemoveWidget(this);
      Controller::setEvent(this, EVENT_WINDOW_WILL_CLOSE);
   }
}

