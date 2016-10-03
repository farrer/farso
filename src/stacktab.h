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

#ifndef _farso_stack_tab_h
#define _farso_stack_tab_h

#include "widget.h"
#include "container.h"

#include <kobold/list.h>

namespace Farso
{

/*! A stack tab widget: provide a way to switch the display of groups of 
 * widgets with a row button selector. */
class StackTab : public Widget
{
   public:
      /*! Constructor. The stack tab usually uses all avaible parent's body.
       * \param parent mandatory pointer to its parent */
      StackTab(Widget* parent);

      /*! Destructor */
      ~StackTab();

      /*! Insert a tab (group of widgets), representing by a container (and a 
       * selector button).
       * \param name name of the group that will be displayed at its selector.
       * \return container relative to the group */
      Container* insertTab(Kobold::String name);
      
      Rect getBody();

      void setDirty();

   protected:

      void doDraw(Rect pBody);
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY, int mrX, int mrY);
      void doAfterChildTreat();

   private:
      class Tab : public Kobold::ListElement
      {
         public:
            Kobold::String name;    /**< Name of the tab element */
            Container* container; /**< Container relative to it */
      };

      void makeActive(Tab* tab);
      
      Tab* active;  /**< Current active (visible) tab */
      Kobold::List tabs;   /**< Defined tabs */
      Rect body; /**< Body (without row for selectors) */
};

}

#endif

