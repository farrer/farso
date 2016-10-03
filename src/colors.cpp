
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
 *                            init                             *
 ***************************************************************/
void Colors::init()
{
   colorCont[0].red = 134;
   colorCont[0].green = 134;
   colorCont[0].blue = 134;
   colorCont[0].alpha = 255;
   colorCont[1].red = 70;
   colorCont[1].green = 70;
   colorCont[1].blue = 70;
   colorCont[1].alpha = 255;
   colorCont[2].red = 169;
   colorCont[2].green = 169;
   colorCont[2].blue = 169;
   colorCont[2].alpha = 255;
   colorWindow.red = 30;
   colorWindow.green = 30;
   colorWindow.blue = 30;
   colorWindow.alpha = 230;
   colorText.red = 255;
   colorText.green = 255;
   colorText.blue = 255;
   colorText.alpha = 255;
   colorSelText.red = 26;
   colorSelText.green = 230;
   colorSelText.blue = 25;
   colorSelText.alpha = 255;
   colorBar.red = 102;
   colorBar.green = 0;
   colorBar.blue = 0;
   colorBar.alpha = 190;
   colorButton.red = colorWindow.red;
   colorButton.green = colorWindow.green;
   colorButton.blue = colorWindow.blue;
   colorButton.alpha = colorWindow.alpha;
   colorMenu.red = colorWindow.red;
   colorMenu.green = colorWindow.green;
   colorMenu.blue = colorWindow.blue;
   colorMenu.alpha = colorWindow.alpha;
   colorFile.red = 240;
   colorFile.blue = 240;
   colorFile.green = 240;
   colorFile.alpha = 255;
   colorDirectory.red = 255;
   colorDirectory.green = 20;
   colorDirectory.blue = 20;
   colorDirectory.alpha = 255;
   colorHigh.red = 240;
   colorHigh.green = 120;
   colorHigh.blue = 0;
   colorHigh.alpha = 255;
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

