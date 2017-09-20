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

#ifndef _farso_widget_event_listener_h
#define _farso_widget_event_listener_h

#include "farsoconfig.h"
#include <kobold/list.h>
#include "eventtype.h"

namespace Farso
{
   class Widget;

   /*! Base class for listening for events on a single widget. */
   class WidgetEventListener : public Kobold::ListElement
   {
      public:
         /*! Constructor */
         WidgetEventListener(){};
         /*! Destructor */
         virtual ~WidgetEventListener(){};

         /*! Function called when an event happened on some owner widgets.
          * \param eventType type of the happened event.
          * \param widget pointer to the widget where the event happened. */
         virtual void onEvent(const EventType& eventType, Widget* widget)=0;
   };

}

#endif

