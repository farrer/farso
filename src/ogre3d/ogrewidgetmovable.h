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

#ifndef _farso_ogre_widget_movable_h
#define _farso_ogre_widget_movable_h

#include "farsoconfig.h"
#if FARSO_USE_OGRE_OVERLAY == 0

#include "ogrewidgetrenderable.h"

#include <OGRE/OgreMovableObject.h>

namespace Farso
{

/*! Ogre::Movable implementation for Farso's WidgetRenderer */
class OgreWidgetMovable : public Ogre::MovableObject
{
   public:
      /*! Constructor */
      OgreWidgetMovable(Ogre::IdType id, 
            Ogre::ObjectMemoryManager* objectMemoryManager, 
            Ogre::SceneManager* sceneManager, Ogre::uint8 renderQueueId);
      /*! Destructor */
      ~OgreWidgetMovable();

      void attachOgreWidgetRenderable(OgreWidgetRenderable* renderable);
      void detachOgreWidgetRenderable(OgreWidgetRenderable* renderable);

      const Ogre::String& getMovableType() const;

   private:
      OgreWidgetRenderable* renderable; /**< Current renderable associated */
};

/*! The MovableObjectFactory for OgreWidgetMovables. */
class OgreWidgetMovableFactory : public Ogre::MovableObjectFactory
{
   public:
      OgreWidgetMovableFactory(){};
      ~OgreWidgetMovableFactory(){};

      static Ogre::String FACTORY_TYPE_NAME;

      const Ogre::String& getType() const;
      
      /*! Destroy an OgreWidgetMovable instance */
      void destroyInstance(Ogre::MovableObject* obj);

   protected:
      /*! Create the OgreWidgetMovable instance */
      virtual Ogre::MovableObject* createInstanceImpl(Ogre::IdType id, 
            Ogre::ObjectMemoryManager* objectMemoryManager,
            Ogre::SceneManager* manager,
            const Ogre::NameValuePairList* params = 0);
};

}

#endif

#endif

