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

#include "colors.h"
#include <stdio.h>
using namespace Farso;

/***************************************************************
 *                        Constructor                          *
 ***************************************************************/
Color::Color()
{
   set(255, 255, 255, 255);
}

/***************************************************************
 *                        Constructor                          *
 ***************************************************************/
Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
   set(r, g, b, a);
}

/***************************************************************
 *                             set                             *
 ***************************************************************/
void Color::set(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
   red = r;
   green = g;
   blue = b;
   alpha = a;
}

/***************************************************************
 *                             set                             *
 ***************************************************************/
void Color::set(Kobold::String value)
{
   int r,g,b,a;
   sscanf(value.c_str(), "%d,%d,%d,%d", &r, &g, &b, &a);
   set(r,g,b,a);
}

/***************************************************************
 *                             ==                              *
 ***************************************************************/
bool Color::operator==(Color& color)
{
   return red == color.red && blue == color.blue && green == color.green &&
          alpha == color.alpha;
}

/***************************************************************
 *                            init                             *
 ***************************************************************/
void Colors::init()
{
   white = Color(255, 255, 255, 255);
   black = Color(0, 0, 0, 255);

   colorCont[0] = Color(134, 134, 134, 255);
   colorCont[1] = Color(70, 70, 70, 255);
   colorCont[2] = Color(169, 169, 169, 255);

   colorWindow = Color(30, 30, 30, 230);
   colorText = Color(255, 255, 255, 255);
   colorSelText = Color(26, 230, 25, 255);

   colorBar = Color(102, 0, 0, 190);
   colorButton = colorWindow;
   colorMenu = colorWindow;

   colorFile = Color(240, 240, 240, 255);
   colorDirectory = Color(255, 20, 20, 255);
   colorHigh = Color(240, 120, 0, 255);
}

Color Colors::colorCont[3];
Color Colors::colorWindow;
Color Colors::colorButton;
Color Colors::colorMenu;
Color Colors::colorText;
Color Colors::colorBar;
Color Colors::colorSelText;
Color Colors::colorFile;
Color Colors::colorDirectory;
Color Colors::colorHigh;

Color Colors::black;
Color Colors::white;


