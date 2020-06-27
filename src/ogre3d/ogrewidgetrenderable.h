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

#ifndef _farso_ogre_widget_renderable_h
#define _farso_ogre_widget_renderable_h

#include "farsoconfig.h"

#include <OGRE/Ogre.h>
#include <OGRE/OgreRenderable.h>
#include <OGRE/OgreMovableObject.h>
#include <kobold/kstring.h>

namespace Farso
{

/*! This class implements an Ogre::Renderable to be used as base for Farso's 
 * Ogre's WidgetRenderer. */ 
class OgreWidgetRenderable : public Ogre::Renderable
{
   public:
      /*! Constructor. 
       * \param name Renderable name 
       * \param width element's width
       * \param height element's height */
      OgreWidgetRenderable(Kobold::String name, int width, int height);
      /*! Destructor */
      ~OgreWidgetRenderable();

#if OGRE_VERSION_MAJOR == 1
      /** @copydoc Renderable::getRenderOperation */
      void getRenderOperation(Ogre::RenderOperation& op);
      /*! \return Material used by the renderable */
      const Ogre::MaterialPtr& getMaterial(void) const;
      /*! Set the Material used by the renderable */
      void setMaterial(Ogre::MaterialPtr& material);
      /*! @copydoc Renderable::getSquaredViewDepth */
      Ogre::Real getSquaredViewDepth(const Ogre::Camera* cam) const;
#else
      /** @copydoc Renderable::getRenderOperation */
      void getRenderOperation(Ogre::v1::RenderOperation& op, bool casterPass);
#endif

      /** @copydoc Renderable::getWorldTransforms */
      void getWorldTransforms(Ogre::Matrix4* xform) const;

      /** @copydoc Renderable::getLights */
      const Ogre::LightList& getLights(void) const
      {
         /* Should never be called, as light won't affect Farso Widgets */
         static Ogre::LightList ll;
         return ll;
      };

      /** @copydoc Renderable::getPolygonModeOverrideable */
      bool getPolygonModeOverrideable(void) const { return false; };
      
      /** @copydoc Renderable::getCastsShadows */
      bool getCastsShadows(void) const;

#if OGRE_VERSION_MAJOR == 1
      /*! Set the subgroup rendering order of this renderable,
       * simulating the behaviour of Ogre 2.x */
      void setRenderQueueSubGroup(Ogre::uint8 subGroupId);
#endif

      /*! Set the movable attached to this renderable.
       * Must only be called by movable->attachWidgetRenderable */
      void setMovable(Ogre::MovableObject* movable) { this->movable = movable;};

   protected:

#if OGRE_VERSION_MAJOR == 1
      void createRenderOperation();
#else 
      /*! Create the VertexArrayObject to render the widget */
      void createVAO();
#endif
      /*! Calculate the renderable coordinates in Ogre [-1, 1] space */
      void calculateCoordinates(float& x1, float& y1, float& x2, float& y2);

   private:

      Kobold::String name;  /**< Current renderable name */
      int width;            /**< Current width */
      int height;           /**< Current height */
      /*! Movable where it is attached (only relevant for Ogre 1.x 
       * implementation, as we'll need to get the transform by its 
       * sceneNode by hand on getWorldTransforms). */
      Ogre::MovableObject* movable; 

#if OGRE_VERSION_MAJOR == 1
      Ogre::MaterialPtr material; /**< Ogre 1.x must have a material */
      /*! For Ogre 1.x we simulate the renderQueueSubGroup of Ogre's 2.x,
       * for controlling the rendering order of each widget */
      Ogre::uint8 renderQueueSubGroupId;
      Ogre::RenderOperation renderOperation; /**< Renderable render operation */
      mutable Ogre::Matrix4 transformMatrix;
#else
      Ogre::VaoManager* vaoManager; /**< VAO manager used */
      Ogre::VertexArrayObject* vao; /**< The single VAO of this renderable */
      float* vertices; /**< The SIMD array of vertices */
      Ogre::VertexBufferPacked* vertexBuffer; /**< Its vertex buffer */
      Ogre::IndexBufferPacked* indexBuffer; /**< Its index buffer */
#endif

};

}

#endif

