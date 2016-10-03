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

#include "ogresurface.h"
#include "../controller.h"
#include <assert.h>
#include <OGRE/OgreTextureManager.h>


using namespace Farso;

/******************************************************************
 *                           constructor                          *
 ******************************************************************/
OgreSurface::OgreSurface(Kobold::String name, int width, int height)
            :Surface(name, width, height)
{
   /* Create the ogre manual's surface */
   this->texture = Ogre::TextureManager::getSingleton().createManual(
         name, 
         Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
         Ogre::TEX_TYPE_2D, width, height, 0, Ogre::PF_A8R8G8B8,
         Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
   
   /* Empty the entire texture surface, as transparent */
   lock();
   clear();
   unlock();
}

/******************************************************************
 *                           constructor                          *
 ******************************************************************/
OgreSurface::OgreSurface(Kobold::String filename, Kobold::String group)
            :Surface(filename, group)
{
   this->locked = false;
   this->ownedTexture = false;
   this->texture = Ogre::TextureManager::getSingleton().load(filename, group,
         Ogre::TEX_TYPE_2D);
   this->width = this->texture->getWidth();
   this->height = this->texture->getHeight();
}

/******************************************************************
 *                            destructor                          *
 ******************************************************************/
OgreSurface::~OgreSurface()
{
   if(ownedTexture)
   {
      Ogre::TextureManager::getSingleton().remove(texture->getName());
   }
}

/******************************************************************
 *                                lock                            *
 ******************************************************************/
void OgreSurface::lock()
{
   assert(!locked);
   locked = true;
   Ogre::HardwarePixelBufferSharedPtr pixelBuffer = texture->getBuffer();

   pixelFormat = texture->getFormat();

   pixelBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL); 
   pixelBox = pixelBuffer->getCurrentLock();
}

/******************************************************************
 *                               unlock                           *
 ******************************************************************/
void OgreSurface::unlock()
{
   assert(locked);
   locked = false;
   texture->getBuffer()->unlock();
}

/******************************************************************
 *                           getPixelFormat                       *
 ******************************************************************/
Ogre::PixelFormat OgreSurface::getPixelFormat()
{
   assert(locked);
   return pixelFormat;
}

/******************************************************************
 *                            getPixelBox                         *
 ******************************************************************/
const Ogre::PixelBox OgreSurface::getPixelBox()
{
   assert(locked);
   return pixelBox;
}

/******************************************************************
 *                            getTexture                          *
 ******************************************************************/
Ogre::TexturePtr OgreSurface::getTexture()
{
   return texture;
}

/******************************************************************
 *                            getWidth                            *
 ******************************************************************/
int OgreSurface::getRealWidth()
{
   return texture->getWidth();
}

/******************************************************************
 *                           getHeight                            *
 ******************************************************************/
int OgreSurface::getRealHeight()
{
   return texture->getHeight();
}


