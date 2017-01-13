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

#ifndef _farso_container_h
#define _farso_container_h

#include "widget.h"

namespace Farso
{

/*! A container is a widget that simple contains other widgets, without
 * any events or draws of its own. */
class Container : public Widget
{
   public:

      enum ContainerType
      {
         /*! Direct children will be top-left oriented: Child x will be 
          * the distance to container left's and y to top. */
         TYPE_TOP_LEFT,
         /*! Direct children will be top-right oriented: Child x will be the 
          * distance to container's right and y to top. */
         TYPE_TOP_RIGHT,
         /*! Direct children will be top-centered oriented: Child x will be the 
          * negative distance to containers center and y to top. */
         TYPE_TOP_CENTERED,
         /*! Direct children will be bottom-left oriented: Child x will be 
          * the distance to container left's and y to bottom. */
         TYPE_BOTTOM_LEFT,
         /*! Direct children will be bottom-right oriented: Child x will be the
          * distance to container's right and y to bottom. */
         TYPE_BOTTOM_RIGHT,
         /*! Direct children will be bottom-centered oriented: Child x will be
          * the negative distance to containers center and y to bottom. */
         TYPE_BOTTOM_CENTERED
      };

      /*! Create a container within full parent's body 
       * \param type container's type
       * \param parent container's parent (mandatory) */
      Container(ContainerType type, Widget* parent);
      /*! Create a container at an specific distance from parent's body bounds.
       * \note The container's will be defined as the rectangle defined by:
       *       pBody.x1 + rect.x1, pBody.y1 + rect.y1, 
       *       pBody.x2 - rect.x2, pBody.y2 - rect.y2
       * \param rect rectangle with distances to each parentBody bound.
       * \param parent container's parent (mandatory). 
       * \param type container's type. */
      Container(ContainerType type, Rect rect, Widget* parent);
      /*! Create a container at an specific position on parent's body,
       * \note parent is optional here. */
      Container(ContainerType type, int x, int y, int width, int height, 
            Widget* parent);

      /*! Destructor */
      ~Container();

      /*! Set the container to be a filled one (instead of the default 
       * transparent). */
      void setFilled();

      /*! \return child X coordinate according to Container type 
       * \param x child x relative coordinate 
       * \param width child width */
      int getChildX(int x, int width);

      /*! \return child Y coordinate according to Container type 
       * \param y child y relative coordinate 
       * \param height child height */
      int getChildY(int y, int height);

      void setDirty();

      Rect getBody();

      /*! \return current container type */
      ContainerType getContainerType();

   protected:
      void doDraw(Rect pBody);
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY, int mrX, int mrY);
      void doAfterChildTreat();

   private:
      ContainerType contType; /**< Container type */
      Rect body;        /**< Full body */
      bool filled; /**< If the container is filled or not */

      bool dynamicSize; /**< If width and height are defined by parent's */

      Rect distance; /**< Distance to each parent's body bounds */

};

}

#endif

