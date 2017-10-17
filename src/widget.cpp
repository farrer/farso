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

#include "widget.h"
#include "controller.h"
#include "container.h"
#include "cursor.h"

#include <kobold/platform.h>
#include <kobold/keyboard.h>
#include <assert.h>

using namespace Farso;

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
Widget::Widget(WidgetType wType, int x, int y, int w, int h, Widget* wParent)
       :type(wType),
        visible(true),
        x(0),
        y(0),
        width(w),
        height(h),
        available(true),
        id(""),
        caption(""),
        mouseHint(""),
        parentContainer(NULL),
        renderer(NULL),
        parent(wParent),
        dirty(true),
        skinElementType(Skin::SKIN_TYPE_UNKNOWN)
{
   assert((width > 0) && (height > 0));

   if(parent)
   {
      /* Set this one as a child of its parent */
      this->parent->addChild(this);
      /* Must use the parent's renderer. */
      this->renderer = this->parent->getWidgetRenderer();
   }
   else
   {
      /* Without parent, must have its own renderer. */
      this->renderer = Controller::createNewWidgetRenderer(width, height);
      /* And be added to the controller as a 'root' widget */
      Controller::addWidget(this);
   }

   defineParentContainer();

   setPosition(x, y);
}

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
Widget::Widget(WidgetType wType, Widget* wParent)
       :type(wType),
        visible(true),
        x(0),
        y(0),
        width(0),
        height(0),
        available(true),
        id(""),
        caption(""),
        mouseHint(""),
        parentContainer(NULL),
        renderer(NULL),
        parent(wParent),
        dirty(true),
        skinElementType(Skin::SKIN_TYPE_UNKNOWN)
{
   if(parent)
   {
      /* Set this one as a child of its parent */
      this->parent->addChild(this);
      /* Must use the parent's renderer. */
      this->renderer = this->parent->getWidgetRenderer();
   }
   else
   {
      /* Without parent, should create its renderer later with setSize call */
      this->renderer = NULL;
      /* And be added to the controller as a 'root' widget */
      Controller::addWidget(this);
   }
   
   defineParentContainer();
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
Widget::~Widget()
{
   listeners.clear();

   /* Remove reference from its id, if defined */
   if(!this->id.empty())
   {
      Controller::clearIdReference(this->id);
   }

   if((renderer != NULL) && (parent == NULL))
   {
      /* Renderer allocation belongs to us. Let's free it. */
      delete renderer;
   }
}

/***********************************************************************
 *                                setId                                *
 ***********************************************************************/
void Widget::setId(const Kobold::String& id)
{
   if(!this->id.empty())
   {
      /* Remove reference from it */
      Controller::clearIdReference(this->id);
   }
   this->id = id;
   if(!this->id.empty())
   {
      /* Add reference to it */
      Controller::setIdReference(this->id, this);
   }
}

/***********************************************************************
 *                              setCaption                             *
 ***********************************************************************/
void Widget::setCaption(const Kobold::String& text)
{
   if(caption != text)
   {
      setDirty();
      caption = text;
   }
}

/***********************************************************************
 *                           setSkinElement                            *
 ***********************************************************************/
void Widget::setSkinElement(int skinElement)
{
   skinElementType = skinElement;
}

/***********************************************************************
 *                             setPosition                             *
 ***********************************************************************/
void Widget::setPosition(int x, int y)
{
   if(parent == NULL)
   {
      renderer->setPosition(x, y);
   }
   else
   {
      if( (this->x != x) || (this->y != y) )
      {
         this->x = x; 
         this->y = y;

         /* As changed its position, must dirty its parent too */
         setDirtyWithParent();
      }
   }
}

/***********************************************************************
 *                                setX                                 *
 ***********************************************************************/
void Widget::setX(int x)
{
   setPosition(x, y);
}

/***********************************************************************
 *                                setY                                 *
 ***********************************************************************/
void Widget::setY(int y)
{
   setPosition(x, y);
}

/***********************************************************************
 *                              setWidth                               *
 ***********************************************************************/
void Widget::setWidth(int width)
{
   setSize(width, height);
}

/***********************************************************************
 *                              setHeight                              *
 ***********************************************************************/
void Widget::setHeight(int height)
{
   setSize(width, height);
}

/***********************************************************************
 *                               setSize                               *
 ***********************************************************************/
void Widget::setSize(int width, int height)
{
   if( (this->width != width) || (this->height != height) )
   {
      this->width = width;
      this->height = height;
      if(parent == NULL)
      {
         if(this->renderer != NULL)
         {
            /* Just update renderer size (recreating its surface if needed) */
            this->renderer->setSize(width, height);
            setDirty();
         }
         else
         {
            /* As no renderer yet, should create one. */
            this->renderer = Controller::createNewWidgetRenderer(width, height);
         }
      }
      else
      {
         /* As changed its position, must dirty its parent too */
         setDirtyWithParent();
      }

   }
}

/***********************************************************************
 *                          getParentContainer                         *
 ***********************************************************************/
void Widget::defineParentContainer()
{
   parentContainer = NULL;

   if( (parent) && (parent->getType() == WIDGET_TYPE_CONTAINER) )
   {
      parentContainer = parent;
      return;
   }
}

/***********************************************************************
 *                                 getX1                               *
 ***********************************************************************/
const int Widget::getX() const
{
   if(parentContainer)
   {
      Container* cont = (Container*) parentContainer;
      return cont->getChildX(x, width);
   }

   return x;
}

/***********************************************************************
 *                                  getY                               *
 ***********************************************************************/
const int Widget::getY() const
{
   if(parentContainer)
   {
      Container* cont = (Container*) parentContainer;
      return cont->getChildY(y, height);
   }

   return y;
}

/***********************************************************************
 *                              disable                                *
 ***********************************************************************/
void Widget::disable()
{
   if(available)
   {
      available = false;
      setDirty();
   }
}

/***********************************************************************
 *                               enable                                *
 ***********************************************************************/
void Widget::enable()
{
   if(!available)
   {
      available = true;
      setDirty();
   }
}

/***********************************************************************
 *                                draw                                 *
 ***********************************************************************/
void Widget::draw(bool force)
{
   bool wasDirty = dirty;

   /* Draw it */
   if(wasDirty || force)
   {
      Rect pBody = (parent ? parent->getBodyWithParentsApplied()
                           : Rect(0, 0, width-1, height-1));
      if(skinElementType == Skin::SKIN_TYPE_UNKNOWN)
      {
         /* Usual render */
         doDraw(pBody);
      }
      else
      {
         /* Override render */
         Farso::Skin* skin = Farso::Controller::getSkin();
         if(skin)
         {
            int x1 = pBody.getX1() + getX();
            int y1 = pBody.getY1() + getY();
            int x2 = x1 + getWidth() - 1;
            int y2 = y1 + getHeight() - 1;

            skin->drawElement(getWidgetRenderer()->getSurface(), 
                  skinElementType, x1, y1, x2, y2);
         }
      }
   }
   dirty = false;

   /* Draw each of its dirty children (or, if dirty, MUST draw the children). */
   Widget* child = (Widget*) getFirst();
   for(int i = 0; i < getTotal(); i++)
   {
      if((child->isVisible()) && (force || wasDirty || child->isDirty()))
      {
         child->draw(wasDirty || force);
      }
      child = (Widget*) child->getNext();
   }

   /* Reupload its texture, if it's a 'root' widget */
   if(parent == NULL)
   {
      getWidgetRenderer()->uploadSurface();
   }
}

/***********************************************************************
 *                               isDirty                               *
 ***********************************************************************/
bool Widget::isDirty()
{
   if(!visible)
   {
      /* A not-visible widget will never be dirty */
      return false;
   }
   if(dirty)
   {
      /* This is dirty: will redraw it and its children.*/
      return true;
   }

   /* Not dirty, must check children */
   Widget* child = (Widget*) getFirst();
   for(int i = 0; i < getTotal(); i++)
   {
      if(child->isDirty())
      {
         return true;
      }
      child = (Widget*) child->getNext();
   }

   /* No dirty found! */
   return false;
}

/***********************************************************************
 *                              setDirty                               *
 ***********************************************************************/
void Widget::setDirty()
{
   dirty = true;
}

/***********************************************************************
 *                         setDirtyWithParent                          *
 ***********************************************************************/
void Widget::setDirtyWithParent()
{
   if(!dirty)
   {
      if(parent)
      {
         parent->setDirty();
      }
      dirty = true;
   }
}

/***********************************************************************
 *                            setMouseHint                             *
 ***********************************************************************/
void Widget::setMouseHint(const Kobold::String& txt)
{
   mouseHint = txt;
}

/***********************************************************************
 *                                hide                                 *
 ***********************************************************************/
void Widget::hide()
{
   if(visible)
   {
      visible = false;
      available = false;

      /* Note that when hidden, there's no need to redraw itself.
       * But we must tell our parent. */
      if(getParent() != NULL)
      {
         getParent()->setDirty();
      }
      else
      {
         renderer->hide();
      }
   }
}

/***********************************************************************
 *                                show                                 *
 ***********************************************************************/
void Widget::show()
{
   if(!visible)
   {
      visible = true;
      available = true;

      if(parent == NULL)
      {
         renderer->show();
      }
   }
}

/***********************************************************************
 *                             isVisible                               *
 ***********************************************************************/
const bool Widget::isVisible() const
{
   if(parent == NULL)
   {
      return renderer->isVisible();
   }
   return visible;
}

/***********************************************************************
 *                              getParent                              *
 ***********************************************************************/
Widget* Widget::getParent() 
{
   return parent;
}

/***********************************************************************
 *                         getParentContainer                          *
 ***********************************************************************/
Widget* Widget::getParentContainer()
{
   return parentContainer;
}

/***********************************************************************
 *                               isInner                               *
 ***********************************************************************/
bool Widget::isInner(int x, int y)
{
   int wx = getX();
   int wy = getY();
   return (x >= wx) && (x <= (wx + width -1)) && 
          (y >= wy) && (y < (wy + height - 1));
}

/***********************************************************************
 *                           isInnerAbsolute                           *
 ***********************************************************************/
bool Widget::isInnerAbsolute(int x, int y)
{
   /* Calculate x / y as relative to parent coordinate */
   Rect body;
   if(getParent() != NULL)
   {
      body = getParent()->getBodyWithParentsApplied();
   }
   int rX = x - getWidgetRenderer()->getPositionX() - body.getX1();
   int rY = y - getWidgetRenderer()->getPositionY() - body.getY1();

   /* Check it the usual way */
   return isInner(rX, rY);
}

/***********************************************************************
 *                              addChild                               *
 ***********************************************************************/
void Widget::addChild(Widget* child)
{
   insertAtEnd(child);
}

/***********************************************************************
 *                           getWidgetRenderer                         *
 ***********************************************************************/
WidgetRenderer* Widget::getWidgetRenderer()
{
   if((renderer == NULL) && (parent != NULL))
   {
      /* Renderer not yet defined, must ask its parent. */
      renderer = parent->getWidgetRenderer();
   }

   return renderer;
}

/***********************************************************************
 *                                treat                                *
 ***********************************************************************/
bool Widget::treat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY)
{
   if((!visible) || (!available))
   {
      /* No events on hidden or disabled widgets. */
      return false;
   }

   /* Let's convert mouse coordinates to window coordinate system */
   int mrX = mouseX - getWidgetRenderer()->getPositionX();
   int mrY = mouseY - getWidgetRenderer()->getPositionY();

   return treat(leftButtonPressed, rightButtonPressed, 
         mouseX, mouseY, mrX, mrY);

}

/***********************************************************************
 *                                treat                                *
 ***********************************************************************/
bool Widget::treat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY, int mrX, int mrY)
{
   if((!visible) || (!available))
   {
      /* No events on hidden or disabled widgets. */
      return false;
   }

#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID && \
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS
   /* Check if have mouse hint and is cursor is over it */
   if( (!mouseHint.empty()) && (isInner(mrX, mrY)) )
   {
      Farso::Cursor::setTextualTip(mouseHint);
   }
#endif
   
   /* Note that if is editing a text, we just need to treat events
    * on the text editor itself. */

#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID && \
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS
   if((!Kobold::Keyboard::isEditingText()) ||
      ((this->getType() == WIDGET_TYPE_TEXT_ENTRY) &&
       ((TextEntry*) this == (TextEntry*) Kobold::Keyboard::getEditor())))
   {
#endif
      if(doTreat(leftButtonPressed, rightButtonPressed, 
               mouseX, mouseY, mrX, mrY))
      {
         /* Event got, no need to check children. */
         return true;
      }
#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID && \
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS
   }
#endif
   
   /* Apply body to mouse coordinates (as the children are relative to it). */
   Rect body = getBodyWithParentsApplied();
   mrX = mouseX - getWidgetRenderer()->getPositionX() - body.getX1();
   mrY = mouseY - getWidgetRenderer()->getPositionY() - body.getY1();

   if((type == WIDGET_TYPE_WINDOW) && 
      (Controller::getActiveWindow() != this))
   {
      /* We'll never check children events on not active
       * windows (as the only event to happen on it will be activation). */
      return false;
   }

   /* Check children */
   Widget* child = (Widget*) getFirst();
   for(int i = 0; i < getTotal(); i++)
   {
      if(child->treat(leftButtonPressed, rightButtonPressed, 
               mouseX, mouseY, mrX, mrY))
      {
         doAfterChildTreat();
         return true;
      }
      child = (Widget*) child->getNext();
   }

   /* Must check for clicks inner the active window. */
   doAfterChildTreat();
   Event last = Controller::getLastEvent(); 
   return last.getType() != EVENT_NONE;
}

/***********************************************************************
 *                      getBodyWithParentsApplied                      *
 ***********************************************************************/
Farso::Rect Widget::getBodyWithParentsApplied()
{
   Rect body = getBody();
   
   if(parent != NULL)
   {
      Rect parentBody = parent->getBodyWithParentsApplied();

      return Rect(parentBody.getX1() + body.getX1(), 
                  parentBody.getY1() + body.getY1(),
                  parentBody.getX1() + body.getX2(),
                  parentBody.getY1() + body.getY2());
   }

   return Rect(body.getX1(), body.getY1(),
               body.getX2(), body.getY2());
}

/***********************************************************************
 *                           addEventListener                          *
 ***********************************************************************/
void Widget::addEventListener(WidgetEventListener* listener)
{
   listeners.push_back(listener);
}

/***********************************************************************
 *                         removeEventListener                         *
 ***********************************************************************/
void Widget::removeEventListener(WidgetEventListener* listener)
{
   listeners.remove(listener);
}

/***********************************************************************
 *                                onEvent                              *
 ***********************************************************************/
void Widget::onEvent(const EventType& eventType)
{
   for(std::list<WidgetEventListener*>::iterator it=listeners.begin(); 
       it != listeners.end(); ++it) 
   {
      (*it)->onEvent(eventType, this);
   }
}

