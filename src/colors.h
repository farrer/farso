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

#ifndef _farso_colors_h
#define _farso_colors_h

#include <inttypes.h>
#include <kobold/kstring.h>
#include "farsoconfig.h"

namespace Farso
{

typedef uint8_t Uint8;
typedef uint16_t Uint16;
typedef uint32_t Uint32;

/*! A color Definition */
class Color
{
   public:
      /*! Constructor with default white fully opaque color */
      Color();
      /*! Constructor for custom color initialize */
      Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

      Uint8 red, /**< Red [0,255] */
            green, /**< Green [0,255] */
            blue, /**< Blue [0,255] */
            alpha; /**< Alpha [0,255]. 0 -> transparent 255 -> opaque. */


      /*! Shortcut function to set color */
      void set(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

      /*! Define the color by a string with its integer components
       * defined, separed by commas: "red,green,blue,alpha" */
      void set(Kobold::String value);

      /*! Check if two colors are the same */
      bool operator==(Color& color);
};

/*! The GUI Colors Definition */
class Colors
{
   public:

      static void init();

      static Color colorCont[3];    /**< Contorn Colors */
      static Color colorWindow;     /**< Window Color */
      static Color colorButton;     /**< Button Color */
      static Color colorMenu;       /**< Menu Color */
      static Color colorText;       /**< Text Color */
      static Color colorBar;        /**< Active Window Bar Color */
      static Color colorSelText;    /**< Color Selected Text */
      static Color colorFile;       /**< Color for file on file lists */
      static Color colorDirectory;  /**< color for dirs on file lists */
      static Color colorHigh;          /**< Color for highlight */
   private:
      Colors(){};
};

}

#endif


