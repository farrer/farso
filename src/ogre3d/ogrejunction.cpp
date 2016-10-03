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

#include "ogrejunction.h"
#include <OGRE/Overlay/OgreOverlayManager.h>

using namespace Farso;

/*************************************************************************
 *                            OgreJunction                               *
 *************************************************************************/
OgreJunction::OgreJunction()
{
   overlay = Ogre::OverlayManager::getSingletonPtr()->create(
               "farso_ogre3d_overlay");
   overlay->show();
}

/*************************************************************************
 *                           ~OgreJunction                               *
 *************************************************************************/
OgreJunction::~OgreJunction()
{
   Ogre::OverlayManager::getSingletonPtr()->destroy(overlay);
}

/*************************************************************************
 *                             getOverlay                                *
 *************************************************************************/
Ogre::Overlay* OgreJunction::getOverlay()
{
   return overlay;
}



