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

#ifndef _farso_ogre_surface_h
#define _farso_ogre_surface_h

#include "../surface.h"

#include <OGRE/OgreHardwarePixelBuffer.h>
#include <OGRE/OgrePixelFormat.h>
#include <OGRE/OgreTexture.h>


namespace Farso
{

/*! The surface is a bidimensional space where we can draw to */
class OgreSurface : public Surface
{
   public:
      /*! Constructor, for a new empty drawable surface.
       * \param name -> name of the drawable surface texture to create
       *                (must be unique). 
       * \param width -> width of the surface. Should be power of two. 
       * \param height -> height of the surface. Should be power of two. */
      OgreSurface(Kobold::String name, int width, int height);
      /*! Constructor to load an image from file and use it as a surface.
       * \param filename -> filename of the image file to load 
       * \param group -> group that image belongs to. */
      OgreSurface(Kobold::String filename, Kobold::String group);
      /*! Destructor */
      ~OgreSurface();

      /*! Lock the surface to draw.
       * \note: must be called before start to draw on it. */
      void lock();

      /*! Unlock surface after draw.
       * \note: must be called when done with draw on it. */
      void unlock();

      /*! \return OgreSurface PixelFormat. Must be called while locked */
      Ogre::PixelFormat getPixelFormat();

      /*! \return OgreSurface PixelBox to edit / read. 
       * Must be called while locked */
      const Ogre::PixelBox getPixelBox();

      /**! \return the surface's texture*/
      Ogre::TexturePtr getTexture();

      int getRealWidth();
      int getRealHeight();

   private:
      Ogre::TexturePtr texture; /**< The manual dynamic texture */
      Ogre::PixelBox pixelBox;  /**< Pixel box related to the dynamic texture*/
      Ogre::PixelFormat pixelFormat; /**< Its format */
};

}


#endif


