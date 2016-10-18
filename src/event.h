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

#ifndef _farso_event_h
#define _farso_event_h

#include "farsoconfig.h"
#include "widget.h"

namespace Farso
{

enum EventType
{
   /*! When no events happened */
   EVENT_NONE,
   /*! When just clicked the window title bar to start its move */
   EVENT_WINDOW_MOVE_INIT,
   /*! When is moving the window (even if its at the same position) */
   EVENT_WINDOW_MOVING,
   /*! When finished the window movement, just after releasing the click */
   EVENT_WINDOW_MOVE_END,
   /*! When clicked on an inactive window, but not yet released the click */
   EVENT_WINDOW_WILL_ACTIVATE,
   /*! When released the click on an inactive window and it is now active */
   EVENT_WINDOW_ACTIVATED,
   /*! When clicked inner the active window, without reacting on 
    * another event on itself or some of its children */
   EVENT_WINDOW_CLICKED,
   /*! When a window is marked to close. In fact, it will close 
    * on next cycle */
   EVENT_WINDOW_WILL_CLOSE,
   /*! When start to press a button */
   EVENT_BUTTON_PRESS_INIT,
   /*! When the button is currently pressing - onpress */
   EVENT_BUTTON_PRESSING,
   /*! When the press is still active, but the presser (mouse cursor or finger)
    *  is no longer inside the button (so its released, but still on the 
    *  active press procedure). */
   EVENT_BUTTON_NOT_PRESSING,
   /*! When the button press is cancelled (ie: released outside it) */
   EVENT_BUTTON_PRESS_CANCELLED,
   /*! When the buttonis released (sucessfully completing its press) */
   EVENT_BUTTON_RELEASE,
   /*! When label is clicked (just after button/finger up) */
   EVENT_LABEL_CLICKED,
   /*! When actively pressing the checkbox, but still without toggling it. */
   EVENT_CHECKBOX_PRESSING,
   /*! When a Checkbox that was unchecked was checked by a click. */
   EVENT_CHECKBOX_CHECKED,
   /*! When a Checkbox that was checked was unchecked by a click. */
   EVENT_CHECKBOX_UNCHECKED,
   /*! When selected an option from the text selector */
   EVENT_TEXTSELECTOR_OPTION_SELECTED,
   /*! When current initial value of the scroll bar changed */
   EVENT_SCROLLBAR_CHANGED,
   /*! When is at scrolling procedure, but without any change to the scroll */
   EVENT_SCROLLBAR_UNCHANGED,
   /*! When just changed the current active tab */
   EVENT_STACKTAB_CHANGED,
   /*! When clicking on the row of stacktab options, but without changes to
    * the current active tab */
   EVENT_STACKTAB_UNCHANGED,
   /*! When started to edit a text entry (ie: when it gain focus) */
   EVENT_TEXTENTRY_EDITION_START,
   /*! When is actually editing a text entry (even if it doesn't changes) */
   EVENT_TEXTENTRY_EDITING,
   /*! When done the editing of a text entry */
   EVENT_TEXTENTRY_EDITION_DONE,
   /*! When cursor is over a grid's element */
   EVENT_GRID_OVER_ELEMENT,
   /*! When just selected a grid's element (after release) */
   EVENT_GRID_SELECTED_ELEMENT,
   /*! Accepted a file selection (to load or save) */
   EVENT_FILE_SELECTOR_ACCEPT,
   /*! Canceled a file selection (load or save) */
   EVENT_FILE_SELECTOR_CANCEL,
   /*! When selected a file on the file selector list */
   EVENT_FILE_SELECTOR_SELECTED_FILE,
   /*! When selected a directory on the file selector list (and thus, changed
    * its current directory displayed) */
   EVENT_FILE_SELECTOR_SELECTED_DIRECTORY
};

/*! A Farso event definition */
class Event
{
   public:
      /*! Constructor
       * \param widget pointer to widget owner of the event
       * \param event event type */
      Event(Widget* widget, EventType event);

      /*! Set event.
       * \param widget pointer to widget owner of the event
       * \param event event type */
      void set(Widget* widget, EventType event);

      /*! \return pointer to the widget owner of the event */
      Widget* getWidget();
      /*! \return event type */
      EventType getType();

   private:
      Widget* widget;
      EventType event;
};

}

#endif

