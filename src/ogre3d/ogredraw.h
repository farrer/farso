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

#ifndef _farso_ogre_draw_h
#define _farso_ogre_draw_h

#include "../draw.h"
#include "../sdl/sdldraw.h"
#include "ogresurface.h"

#include <OGRE/OgrePixelFormat.h>

namespace Farso
{

/*! Draw implementation for Ogre3d. */
class OgreDraw : public SDLDraw
{
   public:
    
      /*!Constructor */
      OgreDraw();

      /*! Destructor */
      ~OgreDraw();

      /*! Copy Ogre::Image's contents to a Surface 
       * \param target target OgreSurface to copy contents to
       * \param image pointer to the source Image */
      void doOgreImageCopy(OgreSurface* target, Ogre::Image* image);

   protected:

      /*! Get pixel directed from current pixel data position */
      void getPixel(Uint8* pixel, Ogre::PixelFormat pixelFormat,
            Uint8& red, Uint8& green, Uint8& blue, Uint8& alpha);

};

}

#endif

