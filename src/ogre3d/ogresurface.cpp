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

#include "ogresurface.h"
#include "ogredraw.h"
#include "../controller.h"

#include <assert.h>
#include <OGRE/OgreImage.h>

using namespace Farso;

/******************************************************************
 *                           constructor                          *
 ******************************************************************/
OgreSurface::OgreSurface(Kobold::String name, int width, int height)
            :SDLSurface(name, width, height)
{
}

/******************************************************************
 *                           constructor                          *
 ******************************************************************/
OgreSurface::OgreSurface(Kobold::String filename, Kobold::String group)
            :SDLSurface(filename, group, false)
{
   load(filename, group);
}

/******************************************************************
 *                              load                              *
 ******************************************************************/
bool OgreSurface::load(Kobold::String filename, Kobold::String group)
{
   /* Load Ogre::Image from disk */
   Ogre::Image ogreImage;
   ogreImage.load(filename, group);
   
   /* Set our dimensions (as not directly used to video card, no need to
    * be power of two: we'll ever blit it to a power of two surface of
    * widget's renderer before sending it to the video card anyway). */
   this->width = ogreImage.getWidth();
   this->height = ogreImage.getHeight();

   /* Create our SDL surface with the dimensions we now known. */
   createSurface(this->width, this->height);

   /* Copy the image to our Surface */
   OgreDraw* ogreDraw = static_cast<OgreDraw*>(Controller::getDraw());
   this->lock();
   ogreDraw->doOgreImageCopy(this, &ogreImage);
   this->unlock();

   return true;
}

/******************************************************************
 *                            destructor                          *
 ******************************************************************/
OgreSurface::~OgreSurface()
{
}

