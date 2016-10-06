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

#include "draw.h"
#include <math.h>
#include <stdlib.h>
#include <kobold/log.h>
#include <string.h>

using namespace Farso;

/******************************************************************
 *                           constructor                          *
 ******************************************************************/
Draw::Draw()
{
  setActiveColor(0, 0, 0, 255);
}

/******************************************************************
 *                            destructor                          *
 ******************************************************************/
Draw::~Draw()
{
}


/******************************************************************
 *                            color_Set                           *
 ******************************************************************/
void Draw::setActiveColor(Uint8 Ri, Uint8 Gi, Uint8 Bi, Uint8 Ai)
{
   curColor.set(Ri, Gi, Bi, Ai);
}
void Draw::setActiveColor(Color color)
{
   curColor = color;
}

/******************************************************************
 *                            color_Get                           *
 ******************************************************************/
void Draw::getActiveColor(Uint8& Ri, Uint8& Gi, Uint8& Bi, Uint8& Ai)
{
   Ri = curColor.red;
   Gi = curColor.green;
   Bi = curColor.blue;
   Ai = curColor.alpha;
}
Color Draw::getActiveColor()
{
   return curColor;
}

/******************************************************************
 *                            setPixel                           *
 ******************************************************************/
void Draw::setPixel(Surface* surface, int x, int y)
{
   setPixel(surface, x, y, curColor.red, curColor.green, curColor.blue,
         curColor.alpha);
}

/******************************************************************
 *                            setPixel                           *
 ******************************************************************/
void Draw::setPixel(Surface* surface, int x, int y, float bright)
{
   float factor = bright;
   if(factor < 0.1f)
   {
      factor = 0.1f;
   }
   if(factor > 1.0f)
   {
      factor = 1.0f;
   }
   setPixel(surface, x, y, curColor.red * factor, curColor.green * factor, 
            curColor.blue * factor, curColor.alpha);
}

/******************************************************************
 *                              round                             *
 ******************************************************************/
inline int round(const float a)
{
   /* Used to the line draw function */
   return (int)(a+0.5);
}

/******************************************************************
 *                            line_Draw                           *
 ******************************************************************/
void Draw::doLine(Surface* surface, int x1, int y1, int x2, int y2)
{
   /* Define deltas (width and height) */
   int dx = x2 - x1;
   int dy = y2 - y1;
   int steps, k;
   float xInc, yInc, x = x1, y = y1;
 
   /* Define needed steps */
   if(abs(dx) > abs(dy))
   {
      steps = abs(dx);
   }
   else
   {
      steps = abs(dy);
   }

   /* Let's iterate and set each line pixel */
   xInc = (float) (dx) / (float) (steps);
   yInc = (float) (dy) / (float) (steps);

   setPixel(surface, round(x), round(y));
   for(k = 0; k < steps; k++)
   {
      x += xInc;
      y += yInc;
      setPixel(surface, round(x), round(y));
   }
}

inline float fpart(const float a)
{
   /* Define fractional part of a */
   return a - (int)(a);
}

inline float rfpart(const float a)
{
   return 1 - fpart(a);
}

/******************************************************************
 *                            line_Draw                           *
 ******************************************************************/
void Draw::doAntiAliasedLine(Surface* surface, int x1, int y1, int x2, int y2)
{
   float dx = (float)x2 - (float)x1;
   float dy = (float)y2 - (float)y1;
   if( fabsf(dx) > fabsf(dy) ) 
   {
      if ( x2 < x1 ) 
      {
         int tmp = x1;
         x1 = x2;
         x2 = tmp;
         tmp = y1;
         y1 = y2;
         y2 = tmp;
      }
      float gradient = dy / dx;
      float xend = round((float) x1);
      float yend = y1 + gradient*(xend - x1);
      float xgap = rfpart(x1 + 0.5f);
      int xpxl1 = xend;
      int ypxl1 = (int)(yend);
      setPixel(surface, xpxl1, ypxl1, rfpart(yend)*xgap);
      setPixel(surface, xpxl1, ypxl1+1, fpart(yend)*xgap);
      float intery = yend + gradient;

      xend = round((float)x2);
      yend = y2 + gradient*(xend - x2);
      xgap = fpart(x2 + 0.5f);
      int xpxl2 = xend;
      int ypxl2 = (int)(yend);
      setPixel(surface, xpxl2, ypxl2, rfpart(yend) * xgap);
      setPixel(surface, xpxl2, ypxl2 + 1, fpart(yend) * xgap);

      int x;
      for(x=xpxl1+1; x <= (xpxl2-1); x++) 
      {
         setPixel(surface, x, (int)(intery), rfpart(intery));
         setPixel(surface, x, (int)(intery) + 1, fpart(intery));
         intery += gradient;
      }
   } 
   else 
   {
      if ( y2 < y1 ) 
      {
         int tmp = x1;
         x1 = x2;
         x2 = tmp;
         tmp = y1;
         y1 = y2;
         y2 = tmp;
      }
      float gradient = dx / dy;
      float yend = round((float) y1);
      float xend = x1 + gradient * (yend - y1);
      float ygap = rfpart(y1 + 0.5f);
      int ypxl1 = yend;
      int xpxl1 = (int)(xend);
      setPixel(surface, xpxl1, ypxl1, rfpart(xend)*ygap);
      setPixel(surface, xpxl1, ypxl1+1, fpart(xend)*ygap);
      float interx = xend + gradient;

      yend = round((float)y2);
      xend = x2 + gradient*(yend - y2);
      ygap = fpart(y2+0.5);
      int ypxl2 = yend;
      int xpxl2 = (int)(xend);
      setPixel(surface, xpxl2, ypxl2, rfpart(xend) * ygap);
      setPixel(surface, xpxl2, ypxl2 + 1, fpart(xend) * ygap);

      int y;
      for(y=ypxl1+1; y <= (ypxl2-1); y++) 
      {
         setPixel(surface, (int)(interx), y, rfpart(interx));
         setPixel(surface, (int)(interx) + 1, y, fpart(interx));
         interx += gradient;
      }
   }
}

/******************************************************************
 *                         rectangle_Draw                         *
 ******************************************************************/
void Draw::doRectangle(Surface* surface, int x1, int y1, int x2, int y2)
{
   doLine(surface, x1, y1, x2, y1);
   doLine(surface, x1, y2, x2, y2);
   doLine(surface, x1, y1, x1, y2);
   doLine(surface, x2, y1, x2, y2);
}

/******************************************************************
 *                        rectangle_2Colors                       *
 ******************************************************************/
void Draw::doTwoColorsRectangle(Surface* surface, int x1, int y1, 
            int x2, int y2, Color secondColor)
{
   doTwoColorsRectangle(surface, x1, y1, x2, y2, secondColor.red,
         secondColor.green, secondColor.blue, secondColor.alpha);
}
void Draw::doTwoColorsRectangle(Surface* surface, int x1, int y1, 
      int x2, int y2, Uint8 Ri, Uint8 Gi, Uint8 Bi, Uint8 Ai)
{
   /* Copy current active color. */
   Uint8 Ra = 0, Ga = 0, Ba = 0, Aa = 0;
   getActiveColor(Ra, Ga, Ba, Aa);

   /* Two lines with active color  */
   doLine(surface, x1, y1, x2, y1);
   doLine(surface, x1, y1, x1, y2);

   /* Two lines with other color */
   setActiveColor(Ri, Gi, Bi, Ai);
   doLine(surface, x1, y2, x2, y2);
   doLine(surface, x2, y1, x2, y2);

   /* Reset the active color (avoiding side effects). */
   setActiveColor(Ra, Ga, Ba, Aa);
}

/******************************************************************
 *                         rectangle_Oval                         *
 ******************************************************************/
void Draw::doRoundedRectangle(Surface* surface, int x1, int y1, int x2, int y2, 
            Uint8 Ri, Uint8 Gi, Uint8 Bi, Uint8 Ai)
{
   /* Kepp previous active color */
   Uint8 Ra, Ga, Ba, Aa;
   getActiveColor(Ra, Ga, Ba, Aa);

   /* Draw with active color */
   doLine(surface, x1,   y1+2, x1+2, y1);
   doLine(surface, x1+2, y1,   x2-2, y1);
   doLine(surface, x1,   y1+2, x1,   y2-2);

   /* Draw with second color */
   setActiveColor(Ri, Gi, Bi, Ai);
   doLine(surface, x1,   y2-2, x1+2, y2);
   doLine(surface, x1+2, y2,   x2-2, y2);
   doLine(surface, x2-2, y2,   x2,   y2-2);
   doLine(surface, x2,   y2-2, x2,   y1+2);
   doLine(surface, x2-2, y1,   x2,   y1+2);

   /* Restore active color */
   setActiveColor(Ra,Ga,Ba,Aa);
}
void Draw::doRoundedRectangle(Surface* surface, int x1, int y1, 
      int x2, int y2, Color secondColor)
{
   doRoundedRectangle(surface, x1, y1, x2, y2, secondColor.red,
         secondColor.green, secondColor.blue, secondColor.alpha);
}

/******************************************************************
 *                          doTriangle                            *
 ******************************************************************/
void Draw::doTriangle(Surface* surface, int x1, int y1, int x2, int y2, 
      int x3, int y3)
{

   doAntiAliasedLine(surface, x1, y1, x2, y2);
   doAntiAliasedLine(surface, x2, y2, x3, y3);
   doAntiAliasedLine(surface, x3, y3, x1, y1);
}

/******************************************************************
 *                          doTriangle                            *
 ******************************************************************/
void Draw::doFilledTriangle(Surface* surface, int x1, int y1, int x2, int y2, 
      int x3, int y3)
{
   doTriangle(surface, x1, y1, x2, y2, x3, y3);

   if(x3 < x2)
   {
      int tmp = x2;
      x2 = x3;
      x3 = tmp;
   }
   if(y3 < y2)
   {
      int tmp = y2;
      y2 = y3;
      y3 = tmp;
   }
   
   if(x2 == x3)
   {
      int delta = 1;
      if(x1 > x2)
      {
         delta = -1;
      }
      for(int y = y2 + 1; y < y3; y++)
      {
         doLine(surface, x1 + delta, y1, x2 - delta, y);
      }

   }
   else if(y2 == y3)
   {
      int delta = 1;
      if(y1 > y2)
      {
         delta = -1;
      }

      for(int x = x2 + 1; x < x3; x++)
      {
         doLine(surface, x1, y1 + delta, x, y2 - delta);
      }
   }
   else
   {
      //TODO.
   }

}

/******************************************************************
 *                          circle_Draw                           *
 ******************************************************************/
void Draw::doCircle(Surface* surface, int xC, int yC, int r)
{
   int x=0, y=r,u=1,v=2*r-1,E=0;
   while(x < y)
   {
     setPixel(surface, xC+x, yC+y);
     setPixel(surface, xC+y, yC-x);
     setPixel(surface, xC-x, yC-y);
     setPixel(surface, xC-y, yC+x);
     x++; E+= u; u+=2;
     if (v < 2 * E)
     {
         y--;
         E -= v;
         v -= 2;
      }
      /* Must check if not done. */
      if (x > y)
      {
         break;
      }
      setPixel(surface, xC+y, yC+x);
      setPixel(surface, xC+x, yC-y);
      setPixel(surface, xC-y, yC-x);
      setPixel(surface, xC-x, yC+y);
   }
}

/******************************************************************
 *                      smallestPowerOfTwo                        *
 ******************************************************************/
int Draw::smallestPowerOfTwo(int num)
{
   int i = 2;
   while(i < num)
   {
      i *= 2;
   }
   return(i);
}

/******************************************************************
 *                          blendColor                            *
 ******************************************************************/
void Draw::blendColor(Uint8 sr, Uint8 sg, Uint8 sb, Uint8 sa, 
                      Uint8& tr, Uint8& tg, Uint8& tb, Uint8& ta)
{
   if(ta == 0)
   {
      /* Must use just the source */
      tr = sr;
      tg = sg;
      tb = sb;
      ta = sa;
   }
   else
   {
      /* Do the blend */
      int da = ((int) sa) + ((int) ta) * (255 - ((int) sa)) / 255;

      tr = (((int)sr) * ((int)sa) + ((int)tr) * 
            (255 - ((int)sa))) / 255;
      tb = (((int)sb) * ((int)sa) + ((int)tb) * 
            (255 - ((int)sa))) / 255;
      tg = (((int)sg) * ((int)sa) + ((int)tg) * 
            (255 - ((int)sa))) / 255;
      ta = da;
   }
}

