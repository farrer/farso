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

#ifndef _farso_surface_h
#define _farso_surface_h

#include <kobold/kstring.h>
#include "farsoconfig.h"

namespace Farso
{

/*! The surface is a bidimensional space where we can draw to */
class Surface
{
   public:
      /*! Constructor, for a new empty drawable surface.
       * \param name -> name of the drawable surface texture to create
       *                (must be unique). 
       * \param width -> width of the surface. Should be power of two. 
       * \param height -> height of the surface. Should be power of two. */
      Surface(const Kobold::String& name, int width, int height);
      /*! Constructor to load an image from file and use it as a surface.
       * \param filename -> filename of the image file to load */
      Surface(const Kobold::String& filename);
      /*! Destructor */
      virtual ~Surface();

      /*! Clear a drawable surface.
       * \note: the caller is responsible to lock the surface before clear(). */
      void clear();

      /*! Lock the surface to draw.
       * \note: must be called before start to draw on it. */
      virtual void lock() = 0;

      /*! Unlock surface after draw.
       * \note: must be called when done with draw on it. */
      virtual void unlock() = 0;

      /*! \return texture name */
      const Kobold::String& getTextureName() const { return name; };

      /*! If you can use this Surface as a drawable target or not.
       * \note all surfaces could be used as sources, but only created
       *       ones could be used as target. */
      const bool isDrawable() const;

      /**! \return surfaces's width */
      const int getWidth() const { return width; };
      /**! \return surfaces's height */
      const int getHeight() const { return height; };

      /*! Get the real surface width (usually, surfaces are created as power 
       * of two, so this function will reflect that) */
      virtual int getRealWidth() = 0;

      /*! Get the real surface height (usually, surfaces are created as power 
       * of two, so this function will reflect that) */
      virtual int getRealHeight() = 0;

   protected:
      
      /*! \return if texture is manual and created here */
      bool isTextureOwned();

      /*! Set the dimensions (not the real ones) of the surface */
      void setDimensions(int width, int height);

      Kobold::String name; /**< Texture name (filename) */
      bool locked; /**< If locked or not. Just for assertion, as the real
                        lock is implemented inside HardwareBuffer. */
      bool ownedTexture; /**< If the texture is manual and created here */
      int width; /**< Surface's width */
      int height; /**< Surface's height */
};

}


#endif


