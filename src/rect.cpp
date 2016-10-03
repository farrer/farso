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
bool Rect::isInner(int x, int y)
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
 *                             isDefined                               *
 ***********************************************************************/
bool Rect::isDefined()
{
   return defined;
}

/***********************************************************************
 *                              getX1                                  *
 ***********************************************************************/
int Rect::getX1()
{
   return x1;
}

/***********************************************************************
 *                              getY1                                  *
 ***********************************************************************/
int Rect::getY1()
{
   return y1;
}


/***********************************************************************
 *                              getX2                                  *
 ***********************************************************************/
int Rect::getX2()
{
   return x2;
}


/***********************************************************************
 *                              getY2                                  *
 ***********************************************************************/
int Rect::getY2()
{
   return y2;
}

/***********************************************************************
 *                              getWidth                               *
 ***********************************************************************/
int Rect::getWidth()
{
   return width;
}


/***********************************************************************
 *                              getHeight                              *
 ***********************************************************************/
int Rect::getHeight()
{
   return height;
}

