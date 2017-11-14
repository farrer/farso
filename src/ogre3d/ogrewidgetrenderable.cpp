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


#include "ogrewidgetrenderable.h"

#if FARSO_USE_OGRE_OVERLAY == 0

#include "../controller.h"

#include <OGRE/OgreRenderOperation.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreRenderSystem.h>
#include <OGRE/Vao/OgreVaoManager.h>

#include <string.h>

using namespace Farso;

/************************************************************************
 *                              Constructor                             *
 ************************************************************************/
OgreWidgetRenderable::OgreWidgetRenderable(Kobold::String name, 
                         int width, int height)
                     :Ogre::Renderable()
{
   this->width = width;
   this->height = height;
   this->name = name;
   this->vertexBuffer = NULL;
   this->indexBuffer = NULL;
   this->vao = NULL;
   this->vertices = NULL;
   
   /* no skeleton animation */
   mHasSkeletonAnimation = false;
   
   /* identity projection and view */
   mUseIdentityProjection = true;
   mUseIdentityView = true;

   /* Let's get VaoManager pointer */
   Ogre::Root* root = Ogre::Root::getSingletonPtr();
   Ogre::RenderSystem* renderSystem = root->getRenderSystem();
   vaoManager = renderSystem->getVaoManager();

   /* Create our VAO */
   createVAO();
}

/************************************************************************
 *                               Destructor                             *
 ************************************************************************/
OgreWidgetRenderable::~OgreWidgetRenderable()
{
   /* Let's destroy our Vao and related buffers */
   if(vao)
   {
      vaoManager->destroyVertexArrayObject(vao);
      vao = NULL;
   }
   if(vertexBuffer)
   {
      if(vertexBuffer->getMappingState() != Ogre::MS_UNMAPPED)
      {
         vertexBuffer->unmap(Ogre::UO_UNMAP_ALL);
      }

      vaoManager->destroyVertexBuffer(vertexBuffer);
      vertexBuffer = NULL;
   }
   if(vertices)
   {
      OGRE_FREE_SIMD(vertices, Ogre::MEMCATEGORY_GEOMETRY);
      vertices = NULL;
   }
   if(indexBuffer)
   {
      vaoManager->destroyIndexBuffer(indexBuffer);
      indexBuffer = NULL;
   }
}

/************************************************************************
 *                               createVAO                              *
 ************************************************************************/
void OgreWidgetRenderable::createVAO()
{
   /* Define our face index data */
   const Ogre::uint16 indexData[6] = { 0, 1, 2, 2, 3, 0 };

   /* Copy it to a SIMD array (the memory will be managed by the vao */
   Ogre::uint16* faceIndices = reinterpret_cast<Ogre::uint16*>(
         OGRE_MALLOC_SIMD(sizeof(Ogre::uint16) * 6, 
                          Ogre::MEMCATEGORY_GEOMETRY) );
   memcpy(faceIndices, indexData, sizeof(indexData));

   /* Let's create the index buffer */
   try
   {
      indexBuffer = vaoManager->createIndexBuffer(
            Ogre::IndexBufferPacked::IT_16BIT, 6, Ogre::BT_IMMUTABLE, 
            faceIndices, true);
   }
   catch(Ogre::Exception& e)
   {
      /* With exceptions, we should need to free it */
      OGRE_FREE_SIMD(indexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
      indexBuffer = NULL;
      throw e;
   }

   /* Define our vertices elements: Positions and UVs */
   Ogre::VertexElement2Vec vertexElements;
   vertexElements.push_back(Ogre::VertexElement2(Ogre::VET_FLOAT3, 
            Ogre::VES_POSITION));
   vertexElements.push_back(Ogre::VertexElement2(Ogre::VET_FLOAT2, 
            Ogre::VES_TEXTURE_COORDINATES));

   /* Define our coordinates */
   float x1 = 0.0f, y1 = 0.0f, x2 = 0.0f, y2 = 0.0f;
   calculateCoordinates(x1, y1, x2, y2);

   /* Define our vertices (with UVs)  */
   const float faceVertices[4 * 5] = 
   { 
      x2, y2, 0.0f, 1.0f, 1.0f,
      x2, y1, 0.0f, 1.0f, 0.0f,
      x1, y1, 0.0f, 0.0f, 0.0f,
      x1, y2, 0.0f, 0.0f, 1.0f
   };

   /* Let's copy it to a SIMD array */
   vertices = reinterpret_cast<float*>(OGRE_MALLOC_SIMD(
            sizeof(float) * 4 * 5, Ogre::MEMCATEGORY_GEOMETRY));
   memcpy(vertices, faceVertices, sizeof(float) * 4 * 5);

   /* And create our packed vertex buffer */
   try
   {
       vertexBuffer = vaoManager->createVertexBuffer(vertexElements, 4, 
             Ogre::BT_DYNAMIC_PERSISTENT, vertices, false); 
   }
   catch(Ogre::Exception &e)
   {
      OGRE_FREE_SIMD(vertexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
      vertexBuffer = NULL;
      throw e;
   }

   /* Finally, the Vao. */
   Ogre::VertexBufferPackedVec vertexBuffers;
   vertexBuffers.push_back(vertexBuffer);

   vao = vaoManager->createVertexArrayObject(vertexBuffers, indexBuffer, 
         Ogre::OT_TRIANGLE_LIST);

   mVaoPerLod[Ogre::VpNormal].push_back(vao);
   mVaoPerLod[Ogre::VpShadow].push_back(vao);
}

/************************************************************************
 *                          calculateCoordinates                        *
 ************************************************************************/
void OgreWidgetRenderable::calculateCoordinates(float& x1, float& y1, 
      float& x2, float& y2)
{
   /* Define vertices at Ogre's -1,1 range. Note the signals, due to 
    * Farso's top-left coordinates are at 0,0 (and for from top-left to 
    * bottom-right). Also, note the 2.0f multiplier, as -1 to 1 is a 2 
    * units space. */
   x1 = 0.0f;
   y1 = 0.0f;
   x2 = (width / static_cast<float>(Controller::getWidth()) * 2.0f);
   y2 = - (height / static_cast<float>(Controller::getHeight()) * 2.0f);
}

/************************************************************************
 *                          getRenderOperation                          *
 ************************************************************************/
void OgreWidgetRenderable::getRenderOperation(Ogre::v1::RenderOperation& op, 
      bool casterPass)
{
   OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED,
         "WidgetRenderable doesn't support the old v1::RenderOperations.",
         "Farso::OgreWidgetRenderable::getRenderOperation");
}

/************************************************************************
 *                           getWorldTransforms                         *
 ************************************************************************/
void OgreWidgetRenderable::getWorldTransforms(Ogre::Matrix4* xform) const
{
   OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED,
         "WidgetRenderable doesn't implements getWorldTransforms.",
         "Farso::OgreWidgetRenderable::getWorldTransforms");
}

/************************************************************************
 *                             getCastShadows                           *
 ************************************************************************/
bool OgreWidgetRenderable::getCastsShadows(void) const
{
   OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED,
         "WidgetRenderable doesn't implements getCastsShadows.",
         "Farso::OgreWidgetRenderable::getCastShadows");

}

#endif

