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
      const bool operator==(Rect& r) const;

      /*! Not equal operator */
      const bool operator!=(Rect& r) const;

      /*! Verify if point x,y is inner the rectangle */
      const bool isInner(int x, int y) const;

      /*! Set rectangle coordinates */
      void set(int x1, int y1, int x2, int y2);

      /*! \return if the rectangle was defined or not */
      const bool isDefined() const;

      /*! \return left coordinate */
      const int getX1() const { return x1; };

      /*! \return top coordinate */
      const int getY1() const { return y1; };

      /*! \return right coordinate */
      const int getX2() const { return x2; };

      /*! \return bottom coordinate */
      const int getY2() const { return y2; };

      /*! \return current width */
      const int getWidth() const { return width; };

      /*! \rturn current height */
      const int getHeight() const { return height; };

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
