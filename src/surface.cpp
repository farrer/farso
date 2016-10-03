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

#include "surface.h"
#include "controller.h"
#include <assert.h>


using namespace Farso;

/******************************************************************
 *                           constructor                          *
 ******************************************************************/
Surface::Surface(Kobold::String name, int width, int height)
{
   this->locked = false;
   this->ownedTexture = true;
   this->width = width;
   this->height = height;
   this->name = name;
}

/******************************************************************
 *                           constructor                          *
 ******************************************************************/
Surface::Surface(Kobold::String filename, Kobold::String group)
{
   this->locked = false;
   this->ownedTexture = false;
   this->name = filename;
}

/******************************************************************
 *                            destructor                          *
 ******************************************************************/
Surface::~Surface()
{
}

/******************************************************************
 *                          setDimensions                         *
 ******************************************************************/
void Surface::setDimensions(int width, int height)
{
   this->width = width;
   this->height = height;
}

/******************************************************************
 *                              clear                             *
 ******************************************************************/
void Surface::clear()
{
   Farso::Draw* draw = Farso::Controller::getDraw();
   Uint8 r=0,g=0,b=0,a=0;
   draw->getActiveColor(r, g, b, a);
   draw->setActiveColor(0, 0, 0, 0);
   draw->doFilledRectangle(this, 0, 0, this->getRealWidth() - 1, 
                           this->getRealHeight() - 1);
   draw->setActiveColor(r, g, b, a);
}

/******************************************************************
 *                          isTextureOwned                        *
 ******************************************************************/
bool Surface::isTextureOwned()
{
   return ownedTexture;
}

/******************************************************************
 *                          getTextureName                        *
 ******************************************************************/
Kobold::String Surface::getTextureName()
{
   return name;
}

/******************************************************************
 *                           isDrawable                           *
 ******************************************************************/
bool Surface::isDrawable()
{
   return ownedTexture;
}

/******************************************************************
 *                            getWidth                            *
 ******************************************************************/
int Surface::getWidth()
{
   return width;
}

/******************************************************************
 *                           getHeight                            *
 ******************************************************************/
int Surface::getHeight()
{
   return height;
}


