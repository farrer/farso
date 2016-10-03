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

#ifndef _farso_widget_h
#define _farso_widget_h

#include <kobold/list.h>
#include <kobold/kstring.h>

#include "widgetrenderer.h"
#include "draw.h"
#include "rect.h"
#include "farsoconfig.h"

namespace Farso
{

/*! The base class for all Farso gui widgets. It can have a list of
 * children Widgets (and can also be a child on another Widget).
 * \note: all of its children are deleted at the Widget destructor. */
class Widget : public Kobold::ListElement, public Kobold::List
{
   public:

      /*! The farso guiWidget types */
      enum WidgetType
      {
         WIDGET_TYPE_WINDOW = 0,
         WIDGET_TYPE_BUTTON,
         WIDGET_TYPE_LABEL,
         WIDGET_TYPE_CHECK_BOX,
         WIDGET_TYPE_PICTURE,
         WIDGET_TYPE_TEXT_SELECTOR,
         WIDGET_TYPE_CONTAINER,
         WIDGET_TYPE_PROGRESS_BAR,
         WIDGET_TYPE_SCROLL_BAR,
         WIDGET_TYPE_STACK_TAB,
         WIDGET_TYPE_TEXT_ENTRY
      };

      /*! Constructor
       * \param type widget type.
       * \param parent pointer to parent Widget (if any). */
      Widget(WidgetType type, int x, int y, int width, int height, 
            Widget* parent);
      
      /*! Destructor */
      virtual ~Widget();

      /*! Draw the Widget to its target renderer.
       * \param force with will force a draw, regardless of its dirty state.
       *        Usually seted as true when parent is dirty.
       * \note: usually called when the widget (or some of its children) 
       * is dirty, to update the texture.
       * \note: the caller is responsable for locking the needed surfaces. */
      void draw(bool force = false);

      /*! Treat mouse (or finger) action on the widget.
       * \param leftButtonPressed state of the left mouse button (or similar,
       *        of a single finger touch on screen). 
       * \param rightButtonPressed state of the right mouse button (or similar,
       *        of a double near-finger touch on screen).
       * \param mouseX X screen coordinate where the cursor 
       *               (mouse or finger) is.
       * \param mouseY Y screen coordinate where the cursor 
       *               (mouse or finger) is.
       * \return true when event got.
       * \note side effect: on event got, it MUST set it at Controller. */
      bool treat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY);
      /*! Same as treat, but with pre-calculated coordinates on parent's
       * coordinate system */
      bool treat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY, int mrX, int mrY);

      /*! \return the widget's caption. */
      Kobold::String getCaption();

      /*! Define widget's caption */
      void setCaption(Kobold::String text);

      /*! Gets the X coordinate
       * \return x coordinate */
      int getX();

      /*! Gets the widget's width.
       * \return width */
      int getWidth();

      /*! Gets the y coordinate
       * \return y coordinate */
      int getY();

      /*! Gets the height
       * \return height */
      int getHeight();

      /*! Set Widget relative (to its parent's body) coordinates
       * \param x -> x coordinate
       * \param y -> y coordinate */
      void setPosition(int x, int y);
      
      /*! Set widget's size. 
       * \note the cost to call it on a widget without a parent 
       * (parent == NULL) is much more, as it'll need it's renderer to be
       * recreated.
       * \param width -> new widget's width
       * \param height -> new widget's height*/
      void setSize(int width, int height);

      /*! Get Widget relative (to its parent's body) coordinates
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate */
      //void getCoordinates(int& xa, int& ya, int& xb, int& yb);

      /*! \return widget's availability. */ 
      bool isAvailable();

      /*! Set available as true. */
      void enable();

      /*! Set available as false */
      void disable();

      /*! Verify if need to redraw the Widget (or some of its children). 
       * \note: the flag will be cleaned with a call to draw. */
      bool isDirty();

      /*! Verify if the widget itself is dirty (ignoring its children state).*/
      bool isSelfDirty();

      /*! Set the Widget as dirty (ie: that will need to be redraw). */
      virtual void setDirty();

      /*! Set Text to display when mouse is over the widget */
      void setMouseHint(Kobold::String txt);
      /*! Get current text to disaply when mouse is over */
      Kobold::String getMouseHint();
   
      /*! Hide the widget */
      void hide();
      /*! Show the widget */
      void show();
      /*! Get visibility status */
      bool isVisible();

      /*! \return widget type */
      WidgetType getType();

      /*! \return pointer to parent GuiWidget, if any */
      Widget* getParent();

      /*! Add a child to this Widget.
       * \param child pointer to the Widget to add as a child
       * \note: all of its children are deleted at the Widget destructor. */
      void addChild(Widget* child);

      /*! \return the WidgetRenderer to use. */
      WidgetRenderer* getWidgetRenderer();

      /*! Check if the relative to parent's coordinate point (x,y)
       * is inside this widget. */
      bool isInner(int x, int y);

      /*! \return rectangle with body with parent's coordinate applyed */
      Rect getBodyWithParentsApplied();


      /*! \return rectangle defining the widget's body. All its children
       * have their coordinates relative to this body */
      virtual Rect getBody() = 0;

   protected:

      /*! Especific widget implementation of draw function.
       * \param pBody rectangle defining parent's body, if one is found. */
      virtual void doDraw(Rect pBody)=0;

      /*! Treat mouse (or finger) action on the widget.
       * \param leftButtonPressed state of the left mouse button (or similar,
       *        of a single finger touch on screen). 
       * \param rightButtonPressed state of the right mouse button (or similar,
       *        of a double near-finger touch on screen).
       * \param mouseX X screen coordinate where the cursor 
       *               (mouse or finger) is.
       * \param mouseY Y screen coordinate where the cursor 
       *               (mouse or finger) is.

       * \param mrX X coordinate where the cursor 
       *    (mouse or finger) is, in renderer coordinate system.
       * \param mrY Y screen coordinate where the cursor 
       *    (mouse or finger) is, in renderer coodinate system.
       * \return true when event got.
       * \note side effect: on event got, it MUST set it at Controller. */
      virtual bool doTreat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY, int mrX, int mrY) = 0;
 
      /*! Function to be called after a child treat returned success.
       * It's usefull in the case the parent was expecting to some of its
       * children event to do some action and transform to an event of its own.
       * For example, a Window will transform an EVENT_BUTTON_RELEASED of
       * its close Button to its own EVENT_WINDOW_CLOSED, closing itself. */
      virtual void doAfterChildTreat() = 0;

      /*! Children should be able to redefine dirty variable */
      void setDirtyValue(bool val);

   private:
      
      WidgetType type;     /**< Widget Type */ 

      bool visible;        /**< Visible or invisible? */
      int x,               /**< Coordinate on Parent */
          y;               /**< Coordinate on Parent */
      int width,           /**< widget's width */
          height;          /**< Widget's height */
      bool available;      /**< Available or disabled? */

      
      Kobold::String caption;   /**< Text of the widget */
      Kobold::String mouseHint; /**< Hint when mouse over */

      WidgetRenderer* renderer; /**< Internal renderer, if without parents. */
      Widget* parent;   /**< Parent Widget - if any */
      bool dirty;     /**< Flag if the had changed its draw state */
};

}

#endif

