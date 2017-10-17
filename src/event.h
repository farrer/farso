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
#include "eventtype.h"
#include "widget.h"

namespace Farso
{

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
      const EventType& getType() const;

   private:
      Widget* widget;
      EventType event;
};

}

#endif

