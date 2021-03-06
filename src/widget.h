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
#include <list>

#include "widgetrenderer.h"
#include "draw.h"
#include "rect.h"
#include "farsoconfig.h"
#include "widgeteventlistener.h"
#include "skin.h"

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
         WIDGET_TYPE_USER_CREATED = 0,
         WIDGET_TYPE_WINDOW,
         WIDGET_TYPE_BUTTON,
         WIDGET_TYPE_LABEL,
         WIDGET_TYPE_CHECK_BOX,
         WIDGET_TYPE_PICTURE,
         WIDGET_TYPE_TEXT_SELECTOR,
         WIDGET_TYPE_CONTAINER,
         WIDGET_TYPE_PROGRESS_BAR,
         WIDGET_TYPE_SCROLL_BAR,
         WIDGET_TYPE_STACK_TAB,
         WIDGET_TYPE_TEXT_ENTRY,
         WIDGET_TYPE_SCROLL_TEXT,
         WIDGET_TYPE_GRID,
         WIDGET_TYPE_FILE_SELECTOR,
         WIDGET_TYPE_MENU,
         WIDGET_TYPE_LABELLED_PICTURE,
         WIDGET_TYPE_CLICKABLE_PICTURE,
         WIDGET_TYPE_SPIN,
         WIDGET_TYPE_TREE_VIEW,
         WIDGET_TYPE_COMBO_BOX
      };

      /*! Constructor
       * \param wType widget type.
       * \param x position on x axis
       * \param position on y axis
       * \param w width
       * \param h height
       * \param wParent pointer to parent Widget (if any). */
      Widget(WidgetType wType, int x, int y, int w, int h, Widget* wParent);

      /*! Special constructor. Only used for widgets that doesn't know its
       * dimensions from start.
       * \param wType widget type.
       * \note widget with this constructor should create its renderer 
       *       before been able to use, with a setSize call with width and
       *       height different from zero.*/
      Widget(WidgetType wType, Widget* wParent);
      
      /*! Destructor */
      virtual ~Widget();

      /*! Set an identifier to the widget.
       * \note this is usefull to check widgets without keeping a pointer
       *       reference to them. Usually used when inserting widgets with
       *       a JSON file instead of the programatic method.
       * \param id string which defines the widget ID. */
      void setId(const Kobold::String& id);

      /*! \return Widget's id. Could be undefined (empty) */
      const Kobold::String& getId() const { return id; };

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
      const Kobold::String& getCaption() const { return caption; };

      /*! Define widget's caption */
      virtual void setCaption(const Kobold::String& text);

      /*! Gets the X coordinate
       * \return x coordinate */
      const int getX() const;

      /*! Gets the widget's width.
       * \return width */
      const int getWidth() const { return width; };

      /*! Gets the y coordinate
       * \return y coordinate */
      const int getY() const;

      /*! \return x coordinate without any container transformation */
      const int getXWithoutTransform() const { return x; };
      /*! \return y coordinate without any container transformation */
      const int getYWithoutTransform() const { return y; };


      /*! Gets the height
       * \return height */
      const int getHeight() const { return height; };

      /*! Set Widget relative (to its parent's body) coordinates
       * \param x -> x coordinate
       * \param y -> y coordinate */ 
      void setPosition(int x, int y);

      /*! Set X coordinate */
      void setX(int x);
      
      /*! Set Y coordinate */
      void setY(int x);
      
      /*! Set widget's size. 
       * \note the cost to call it on a widget without a parent 
       * (parent == NULL) is much more, as will need to recreate its 
       * renderer and tell all children of the change.
       * \param width -> new widget's width
       * \param height -> new widget's height */ 
      virtual void setSize(int width, int height);

      /*! Set widget's width */
      void setWidth(int width);
      /*! Set widget's height */
      void setHeight(int height);

      /*! Get Widget relative (to its parent's body) coordinates
       * \param xa -> x1 coordinate
       * \param ya -> y1 coordinate
       * \param xb -> x2 coordinate
       * \param yb -> y2 coordinate */
      //void getCoordinates(int& xa, int& ya, int& xb, int& yb);

      /*! \return widget's availability. */ 
      const bool isAvailable() const { return available; };
      /*! \retur widget's availablility */
      const bool isEnabled() const { return available; };

      /*! Set available as true. */
      virtual void enable();

      /*! Set available as false */
      virtual void disable();

      /*! Verify if need to redraw the Widget (or some of its children). 
       * \note: the flag will be cleaned with a call to draw. */
      bool isDirty();

      /*! Verify if the widget itself is dirty (ignoring its children state).*/
      const bool isSelfDirty() const { return dirty; };

      /*! Set the Widget as dirty (ie: that will need to be redraw). */
      virtual void setDirty();

      /*! Set the Widget as dirty and also its parent (if any). */
      void setDirtyWithParent();

      /*! Set Text to display when mouse is over the widget */
      void setMouseHint(const Kobold::String& txt);
      /*! Get current text to disaply when mouse is over */
      const Kobold::String& getMouseHint() const { return mouseHint; };
   
      /*! Hide the widget */
      void hide();
      /*! Show the widget */
      void show();
      /*! Get visibility status */
      const bool isVisible() const;

      /*! \return widget type */
      const WidgetType& getType() const { return type; };

      /*! \return pointer to parent Widget, if any */
      Widget* getParent();
      /*! \return pointer to nearest Container parent of this Widget, if any */
      Widget* getParentContainer();

      /*! Add a child to this Widget.
       * \param child pointer to the Widget to add as a child
       * \note: all of its children are deleted at the Widget destructor. */
      void addChild(Widget* child);

      /*! \return the WidgetRenderer to use. */
      WidgetRenderer* getWidgetRenderer();
      /*! \return if the renderer it use is one of its own (true) or if it's
       * the one of its parent (false). */
      const bool haveOwnRenderer() const { return ownRenderer; };

      /*! Check if the relative to parent's coordinate point (x,y)
       * is inside this widget. */
      bool isInner(int x, int y);

      /*! Check if the absolute (screen) coordinate point (x,y) is
       * inside the widget. */
      bool isInnerAbsolute(int x, int y);

      /*! \return rectangle with body with parent's coordinate applyed */
      Rect getBodyWithParentsApplied();

      /*! \return rectangle defining the widget's body. All its children
       * have their coordinates relative to this body */
      virtual const Rect& getBody() = 0;

      /*! Override the widget draw with an specific skin element.
       * \note only useful when using a skin
       * \param skinElement element to use instead of the default draw. */
      void setSkinElement(int skinElement);

      /*! Add an event listener to this widget.
       * \param listener pointer to the event listener. */
      void addEventListener(WidgetEventListener* listener);
      /*! Remove an event listener from this widget
       * \param lister pointer to the event listener to remove. */
      void removeEventListener(WidgetEventListener* listener);
      /*! Tell our listeners that an event happened on this widget */
      void onEvent(const EventType& eventType);

      /*! \return the Root widget of the current widget.
       * \note could be the same widget if it's a root one. */
      Widget* getRoot();

   protected:

      /*! Especific widget implementation of draw function.
       * \param pBody rectangle defining parent's body, if one is found. */
      virtual void doDraw(const Rect& pBody)=0;

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

      /*! Define the nearest container parent if exists. */
      void defineParentContainer();

      /*! Override the widget renderer used.
       * \note should not be called on root widgets */
      void overrideWidgetRenderer(WidgetRenderer* renderer, bool ownRenderer);

   private:
      
      WidgetType type;     /**< Widget Type */ 

      bool visible;        /**< Visible or invisible? */
      int x,               /**< Coordinate on Parent */
          y;               /**< Coordinate on Parent */
      int width,           /**< widget's width */
          height;          /**< Widget's height */
      bool available;      /**< Available or disabled? */

      Kobold::String id; /**< Widget identifier. Not mandatory */
      Kobold::String caption;   /**< Text of the widget */
      Kobold::String mouseHint; /**< Hint when mouse over */

      Widget* parentContainer; /**< Container parent to this widget */

      WidgetRenderer* renderer; /**< Internal renderer, if without parents. */
      bool ownRenderer; /**< If the renderer was created for this widget and
                             its memmory is our responsibility */
      Widget* parent;   /**< Parent Widget - if any */
      Widget* root; /**< Root widget of this one */
      bool dirty;     /**< Flag if the had changed its draw state */

      int skinElementType; /**< Override the way to draw the element */

      std::list<WidgetEventListener*> listeners; /**< List of event listeners */
};

}

#endif

