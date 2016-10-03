/* 
  Farso: a simple GUI.
  Copyright (C) DNTeam <dnt@dnteam.org>
 
  This file is part of Farso.
 
  Farso is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Farso is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Farso.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "event.h"
using namespace Farso;

/***********************************************************************
 *                            Constructor                              *
 ***********************************************************************/
Event::Event(Widget* widget, EventType event)
{
   set(widget, event);
}

/***********************************************************************
 *                                set                                  *
 ***********************************************************************/
void Event::set(Widget* widget, EventType event)
{
   this->widget = widget;
   this->event = event;
}

/***********************************************************************
 *                             getWidget                               *
 ***********************************************************************/
Widget* Event::getWidget()
{
   return widget;
}

/***********************************************************************
 *                              getType                                *
 ***********************************************************************/
EventType Event::getType()
{
   return event;
}

