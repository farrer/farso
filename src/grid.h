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

#ifndef _farso_grid_h
#define _farso_grid_h

#include "widget.h"
#include <kobold/list.h>

namespace Farso
{

/*! The grid is a widget that subdivides its parent's body are
 * in several special small selectable areas to be interacted with. */
class Grid : public Widget
{
   public:

      enum GridType
      {
         /*! Just draw a border to the current element */
         GRID_TYPE_HIGHLIGHT_BORDER,
         /*! Fill the current element (usually with a child draw after it) */
         GRID_TYPE_HIGHLIGHT_FILL,
         /*! No highlighting for current element */
         GRID_TYPE_HIGHLIGHT_NONE
      };

      /*! A single grid element representation */
      class GridElement : public Kobold::ListElement
      {
         public:
            /*! Constructor */
            GridElement(int x, int y, int width, int height, 
                  const Kobold::String& mouseHint, int index);
            /*! Destructor */
            ~GridElement();

            /*! Set the grid area */
            void set(int x, int y, int width, int height);

            /*! Check if point (x, y) is inner the element */
            bool isInner(int x, int y);

            /*! \return element's relative area */
            const Rect& getArea() { return area; };

            /*! \return text to be displayed on mouse over */
            const Kobold::String& getMouseHint() { return mouseHint; };

            /*! \return element's index (defined as an incremented integer 
             * from initial 0, for each added element). */
            const int getIndex() { return index; };

            /*! Enable the selection of this grid element */
            void enable();
            /*! Disable this grid element */
            void disable();
            /* \return if the element is enabled or not */
            const bool isEnabled() { return enabled; };

         private:
            Rect area;  /**< Element's area */
            Kobold::String mouseHint; /**< Text to display on mouse over */
            int index; /**< Element's index */

            bool enabled; /**< If is enabled or not */
      };

      /*! Constructor
       * \param gridType current type
       * \param parent grid's parent. Mandatory, as the grid will use 
       *               full parent's body. */ 
      Grid(const GridType& gridType, Widget* parent);
      /*! Destructor */
      ~Grid();

      /*! Clear all current grid elements */
      void clearElements();

      /*! Add a grid element to the grid.
       * \note the element will be deleted on grid's destructor.
       * \param x element's left coordinate
       * \param y element's top coordinate
       * \param width element's width
       * \param height element's height 
       * \param mouseHint text to be displayed on mouse over element
       * \return pointert to the created grid element */  
      GridElement* addElement(int x, int y, int width, int height,
            const Kobold::String& mouseHint="");

      /*! \return pointer to GridElement actally active (under cursor) */
      GridElement* getCurrent();

      /*! Get a Grid element by its index
       * \param index [0, total) 
       * \return pointer to the grid element or NULL */
      GridElement* getByIndex(int index);

      /*! Enable the draw of a border for grid's whole area */
      void enableBorder();
      /*! Disable the draw of a border for grid's whole area */
      void disableBorder();

      /* Functions from Widget */
      const Rect& getBody();
      void setDirty();

   protected:
      
      /* Functions from Widget */
      void doDraw(const Rect& pBody);
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY, int mrX, int mrY);
      void doAfterChildTreat();

   private:

      Kobold::List elements; /**< Elements from the grid */
      Rect body; /**< Grid body */
      GridElement* current; /**< Current element under cursor, if any */
      GridType gridType; /**< Current grid type */
      bool pressStarted; /**< If pressing the element */
      int curIndex; /**< Current next element's index */
      bool useBorder; /**< If should draw a border do grid's area */
      Rect parentBody; /**< Last state of parent's body */
};

}

#endif

