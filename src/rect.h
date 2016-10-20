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

#ifndef _farso_rect_h
#define _farso_rect_h

#include "farsoconfig.h"

namespace Farso
{

/*! A simple rectangle defined by top left corner [x1, y1] and
 * bottom right corner [x2, y2], both inclusive (ie: the rectangle
 * width will be (x2 - x1 + 1) and height (y2 - y1 + 1)). */
class Rect
{
   public: 
      /*! Constructor */
      Rect();
      /*! Constructor with defined values */
      Rect(int x1, int y1, int x2, int y2);
      /*! Destructor */
      ~Rect();

      /*! Assign operator overload. */
      Rect& operator=(Rect r);

      /*! Equal operator */
      bool operator==(Rect& r);

      /*! Not equal operator */
      bool operator!=(Rect& r);

      /*! Verify if point x,y is inner the rectangle */
      bool isInner(int x, int y);

      /*! Set rectangle coordinates */
      void set(int x1, int y1, int x2, int y2);

      /*! \return if the rectangle was defined or not */
      bool isDefined();

      /*! \return left coordinate */
      int getX1();

      /*! \return top coordinate */
      int getY1();

      /*! \return right coordinate */
      int getX2();

      /*! \return bottom coordinate */
      int getY2();

      /*! \return current width */
      int getWidth();

      /*! \rturn current height */
      int getHeight();

   private:
      int x1,
          y1,
          x2,
          y2;
      int width,
          height;

      bool defined;

};

}

#endif
