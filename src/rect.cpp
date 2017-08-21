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

#include "rect.h"
using namespace Farso;

/***********************************************************************
 *                              Constructor                            *
 ***********************************************************************/
Rect::Rect()
{
   x1 = 0;
   y1 = 0;
   x2 = 0;
   y2 = 0;
   width = 0;
   height = 0;
   defined = false;
}
Rect::Rect(int x1, int y1, int x2, int y2)
{
   set(x1, y1, x2, y2);
}

/***********************************************************************
 *                               Destructor                            *
 ***********************************************************************/
Rect::~Rect()
{
}

/***********************************************************************
 *                                isInner                              *
 ***********************************************************************/
const bool Rect::isInner(int x, int y) const
{
   return (x >= x1) && (x <= x2) && (y >= y1) && (y <= y2);
}

/***********************************************************************
 *                                   set                               *
 ***********************************************************************/
void Rect::set(int x1, int y1, int x2, int y2)
{
   this->x1 = x1;
   this->y1 = y1;
   this->x2 = x2;
   this->y2 = y2;
   this->width = x2 - x1 + 1;
   this->height = y2 - y1 + 1;
   this->defined = true;
}

/***********************************************************************
 *                              Assign                                 *
 ***********************************************************************/
Rect& Rect::operator=(Rect r)
{
   this->defined = r.defined;
   this->x1 = r.x1;
   this->y1 = r.y1;
   this->x2 = r.x2;
   this->y2 = r.y2;
   this->width = r.width;
   this->height = r.height;
   return *this;
}

/***********************************************************************
 *                              Equal                                  *
 ***********************************************************************/
const bool Rect::operator==(Rect& r) const
{
   return !((*this) != r);
}

/***********************************************************************
 *                            Not Equal                                *
 ***********************************************************************/
const bool Rect::operator!=(Rect& r) const
{
    return (x1 != r.x1) || (y1 != r.y1) || 
           (x2 != r.x2) || (y2 != r.y2);
}


/***********************************************************************
 *                             isDefined                               *
 ***********************************************************************/
const bool Rect::isDefined() const 
{
   return defined;
}

