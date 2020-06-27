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

#include "ogrewidgetmovable.h"
#include "ogrerenderer.h"
#include <OGRE/OgreSceneManager.h>

namespace Farso
{

/************************************************************************
 *                              Constructor                             *
 ************************************************************************/
#if OGRE_VERSION_MAJOR == 1
OgreWidgetMovable::OgreWidgetMovable(const Ogre::String& name)
                  :Ogre::MovableObject(name),
                   bbox(Ogre::AxisAlignedBox::EXTENT_INFINITE)
{
   this->renderable = NULL;
}
#else
OgreWidgetMovable::OgreWidgetMovable(Ogre::IdType id, 
                      Ogre::ObjectMemoryManager* objectMemoryManager, 
                      Ogre::SceneManager* sceneManager, 
                      Ogre::uint8 renderQueueId)
                   :Ogre::MovableObject(id, objectMemoryManager, sceneManager,
                                  renderQueueId)
{
   this->renderable = NULL;
   /* Define our infinite AABB */
   mObjectData.mLocalAabb->setFromAabb(Ogre::Aabb::BOX_INFINITE, 
         mObjectData.mIndex);
   mObjectData.mWorldAabb->setFromAabb(Ogre::Aabb::BOX_INFINITE, 
         mObjectData.mIndex );
   mObjectData.mLocalRadius[mObjectData.mIndex] = 
      Ogre::Aabb::BOX_INFINITE.getRadius();
   mObjectData.mWorldRadius[mObjectData.mIndex] = 
      Ogre::Aabb::BOX_INFINITE.getRadius();
   mObjectData.mQueryFlags[mObjectData.mIndex] = 
      Ogre::SceneManager::QUERY_ENTITY_DEFAULT_MASK;
}
#endif

/************************************************************************
 *                               Destructor                             *
 ************************************************************************/
OgreWidgetMovable::~OgreWidgetMovable()
{
}

/************************************************************************
 *                       attachOgreWidgetRenderable                     *
 ************************************************************************/
void OgreWidgetMovable::attachOgreWidgetRenderable(
      OgreWidgetRenderable* renderable)
{
#if OGRE_VERSION_MAJOR >= 2
   /* Note: as we are just supporting a single renderable per widget movable,
    * we should passed it as contructor parameter, but I was too lazy (and not
    * very confortable to) implement a 'search widget by name' (even if it was
    * a simple hash) for our factory... we already have the pointer at the 
    * first place at all, I refuse to search for it again.  */
   assert(this->renderable == NULL);
   /* Add the renderable to our renderable queue */
   mRenderables.push_back(renderable);
#endif
   this->renderable = renderable;
   this->renderable->setMovable(this);
}

/************************************************************************
 *                       detachOgreWidgetRenderable                     *
 ************************************************************************/
void OgreWidgetMovable::detachOgreWidgetRenderable(
      OgreWidgetRenderable* renderable)
{
   assert(this->renderable == renderable);
#if OGRE_VERSION_MAJOR >= 2
   mRenderables.pop_back();
#endif
   this->renderable->setMovable(NULL);
   this->renderable = NULL;
}

/************************************************************************
 *                             getMovableType                           *
 ************************************************************************/
const Ogre::String& OgreWidgetMovable::getMovableType() const
{
   return OgreWidgetMovableFactory::FACTORY_TYPE_NAME;
}


#if OGRE_VERSION_MAJOR == 1
/************************************************************************
 *                           _updateRenderQueue                         *
 ************************************************************************/
void OgreWidgetMovable::_updateRenderQueue(Ogre::RenderQueue* queue)
{
   queue->addRenderable(this->renderable);
}

/************************************************************************
 *                            visitRenderables                          *
 ************************************************************************/
void OgreWidgetMovable::visitRenderables(Ogre::Renderable::Visitor* visitor, 
      bool debugRenderables)
{
   visitor->visit(this->renderable, 0, false);
}
#endif

/************************************************************************
 *                           createInstanceImpl                         *
 ************************************************************************/
#if OGRE_VERSION_MAJOR == 1
Ogre::MovableObject* OgreWidgetMovableFactory::createInstanceImpl(
      const Ogre::String& name, 
      const Ogre::NameValuePairList* params)
{
   return OGRE_NEW OgreWidgetMovable(name);
}
#else
Ogre::MovableObject* OgreWidgetMovableFactory::createInstanceImpl(
      Ogre::IdType id, Ogre::ObjectMemoryManager* objectMemoryManager,
      Ogre::SceneManager* manager, const Ogre::NameValuePairList* params)
{
   return OGRE_NEW OgreWidgetMovable(id, objectMemoryManager, manager, 
         FARSO_DEFAULT_RENDER_QUEUE);
}
#endif

/************************************************************************
 *                            destroyInstance                           *
 ************************************************************************/
void OgreWidgetMovableFactory::destroyInstance(Ogre::MovableObject* obj)
{
   OGRE_DELETE obj;
}

/************************************************************************
 *                               getType                                *
 ************************************************************************/
const Ogre::String& OgreWidgetMovableFactory::getType(void) const
{
   return FACTORY_TYPE_NAME;
}

/************************************************************************
 *                          static attribute                            *
 ************************************************************************/
Ogre::String OgreWidgetMovableFactory::FACTORY_TYPE_NAME = "FarsoWidget";

}

