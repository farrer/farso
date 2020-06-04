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
#if FARSO_USE_OGRE_OVERLAY == 0

#include <OGRE/Ogre.h>
#include <OGRE/OgreRenderable.h>
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

      /** @copydoc Renderable::getRenderOperation */
      void getRenderOperation(Ogre::v1::RenderOperation& op, bool casterPass);

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

   protected:

      /*! Create the VertexArrayObject to render the widget */
      void createVAO();
      /*! Calculate the renderable coordinates in Ogre [-1, 1] space */
      void calculateCoordinates(float& x1, float& y1, float& x2, float& y2);

   private:

      Kobold::String name;  /**< Current renderable name */
      int width;            /**< Current width */
      int height;           /**< Current height */
      Ogre::VaoManager* vaoManager; /**< VAO manager used */
      Ogre::VertexArrayObject* vao; /**< The single VAO of this renderable */
      float* vertices; /**< The SIMD array of vertices */
      Ogre::VertexBufferPacked* vertexBuffer; /**< Its vertex buffer */
      Ogre::IndexBufferPacked* indexBuffer; /**< Its index buffer */
};

}

#endif
#endif

