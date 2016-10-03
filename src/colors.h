
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


