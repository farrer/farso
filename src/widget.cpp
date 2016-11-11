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
Widget::Widget(WidgetType type, int x, int y, int width, int height, 
      Widget* parent)
{
   assert((width > 0) && (height > 0));

   this->available = true;
   this->visible = true;
   this->width = width;
   this->height = height;
   this->dirty = true;
   this->type = type;
   this->x = 0;
   this->y = 0;

   this->parent = parent;
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
Widget::Widget(WidgetType type, Widget* parent)
{
   this->available = true;
   this->visible = true;
   this->x = 0;
   this->y = 0;
   this->width = 0;
   this->height = 0;
   this->dirty = true;
   this->type = type;
   this->parent = parent;

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
   if((renderer != NULL) && (parent == NULL))
   {
      /* Renderer allocation belongs to us. Let's free it. */
      delete renderer;
   }
}

/***********************************************************************
 *                               getType                               *
 ***********************************************************************/
Widget::WidgetType Widget::getType()
{
   return type;
}

/***********************************************************************
 *                              getCaption                             *
 ***********************************************************************/
Kobold::String Widget::getCaption()
{
   return caption;
}

/***********************************************************************
 *                              setCaption                             *
 ***********************************************************************/
void Widget::setCaption(Kobold::String text)
{
   if(caption != text)
   {
      setDirty();
      caption = text;
   }
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
         parent->setDirty();
         setDirty();
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
         /* Must recreate its renderer */
         if(this->renderer != NULL)
         {
            delete this->renderer;
         }
         this->renderer = Controller::createNewWidgetRenderer(width, height);
         //FIXME: must reset renderer pointer to all of its children.
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
 *                         getXWithotTransform                         *
 ***********************************************************************/
int Widget::getXWithoutTransform()
{
   return x;
}

/***********************************************************************
 *                         getYWithotTransform                         *
 ***********************************************************************/
int Widget::getYWithoutTransform()
{
   return y;
}

/***********************************************************************
 *                                 getX1                               *
 ***********************************************************************/
int Widget::getX()
{
   if(parentContainer)
   {
      Container* cont = (Container*) parentContainer;
      return cont->getChildX(x, width);
   }

   return x;
}

/***********************************************************************
                                  getWidth                             *
 ***********************************************************************/
int Widget::getWidth()
{
   return width;
}

/***********************************************************************
 *                                  getY                               *
 ***********************************************************************/
int Widget::getY()
{
   if(parentContainer)
   {
      Container* cont = (Container*) parentContainer;
      return cont->getChildY(y, height);
   }

   return y;
}

/***********************************************************************
 *                               getHeight                             *
 ***********************************************************************/
int Widget::getHeight()
{
   return height;
}

/***********************************************************************
 *                            isAvailable                              *
 ***********************************************************************/
bool Widget::isAvailable() 
{
   return available;
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
   if(!visible)
   {
      /* No need to draw invisible widgets. */
      dirty = false;
      return;
   }
   bool wasDirty = dirty;

   /* Draw it */
   if(wasDirty || force)
   {
      doDraw((parent ? parent->getBodyWithParentsApplied()
                     : Rect(0, 0, width-1, height-1)));
   }
   dirty = false;

   /* Draw each of its dirty children (or, if dirty, MUST draw the children). */
   Widget* child = (Widget*) getFirst();
   for(int i = 0; i < getTotal(); i++)
   {
      if(force || wasDirty || child->isDirty())
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
 *                             isSelfDirty                             *
 ***********************************************************************/
bool Widget::isSelfDirty()
{
   return dirty;
}

/***********************************************************************
 *                              setDirty                               *
 ***********************************************************************/
void Widget::setDirty()
{
   dirty = true;
}

/***********************************************************************
 *                           setDirtyValue                             *
 ***********************************************************************/
void Widget::setDirtyValue(bool val)
{
   dirty = val;
}

/***********************************************************************
 *                            setMouseHint                             *
 ***********************************************************************/
void Widget::setMouseHint(Kobold::String txt)
{
   mouseHint = txt;
}

/***********************************************************************
 *                            getMouseHint                             *
 ***********************************************************************/
Kobold::String Widget::getMouseHint()
{
   return mouseHint;
}

/***********************************************************************
 *                                hide                                 *
 ***********************************************************************/
void Widget::hide()
{
   visible = false;
   available = false;

   /* Note that when hidden, there's no need to redraw itself.
    * But we must tell our parent. */
   dirty = false;
   if(getParent() != NULL)
   {
      getParent()->setDirty();
   }
   else
   {
      renderer->hide();
   }
}

/***********************************************************************
 *                                show                                 *
 ***********************************************************************/
void Widget::show()
{
   visible = true;
   available = true;
   dirty = true;

   if(parent == NULL)
   {
      renderer->show();
   }
}

/***********************************************************************
 *                             isVisible                               *
 ***********************************************************************/
bool Widget::isVisible()
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

