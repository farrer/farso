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

#include "../controller.h"

#include <OGRE/OgreRenderOperation.h>
#include <OGRE/OgreRoot.h>
#include <OGRE/OgreRenderSystem.h>

#if OGRE_VERSION_MAJOR == 1
   #include <OGRE/OgreHardwareBufferManager.h>
#else
   #include <OGRE/Vao/OgreVaoManager.h>
#endif

#include <string.h>

#define FARSO_OGRE_1_MAX_SUBS 255

namespace Farso
{

/************************************************************************
 *                              Constructor                             *
 ************************************************************************/
OgreWidgetRenderable::OgreWidgetRenderable(Kobold::String name, 
                         int width, int height)
                     :Ogre::Renderable()
{
   /* identity projection and view */
   mUseIdentityProjection = true;
   mUseIdentityView = true;

   this->width = width;
   this->height = height;
   this->name = name;
   this->movable = NULL;

#if OGRE_VERSION_MAJOR == 1
   this->renderQueueSubGroupId = FARSO_OGRE_1_MAX_SUBS;
   createRenderOperation();
#else
   this->vertexBuffer = NULL;
   this->indexBuffer = NULL;
   this->vao = NULL;
   this->vertices = NULL;

   /* no skeleton animation */
   mHasSkeletonAnimation = false;
   
   /* Let's get VaoManager pointer */
   Ogre::Root* root = Ogre::Root::getSingletonPtr();
   Ogre::RenderSystem* renderSystem = root->getRenderSystem();
   vaoManager = renderSystem->getVaoManager();

   /* Create our VAO */
   createVAO();
#endif
}

/************************************************************************
 *                               Destructor                             *
 ************************************************************************/
OgreWidgetRenderable::~OgreWidgetRenderable()
{
#if OGRE_VERSION_MAJOR == 1
   /* Destroy things related to the render operation */
   OGRE_DELETE renderOperation.vertexData;
   OGRE_DELETE renderOperation.indexData;
#else 
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
#endif
}

#if OGRE_VERSION_MAJOR == 1
/************************************************************************
 *                        createRenderOperation                         *
 ************************************************************************/
void OgreWidgetRenderable::createRenderOperation()
{
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
   /* And our indexes */
   const Ogre::uint16 indexData[6] = { 0, 1, 2, 2, 3, 0 };

   /* Create the vertex data */
   renderOperation.vertexData = OGRE_NEW Ogre::VertexData();
   Ogre::VertexDeclaration* decl = 
      renderOperation.vertexData->vertexDeclaration;
   size_t offset = 0;

   /* Positions */
   offset += decl->addElement(0, offset, Ogre::VET_FLOAT3, 
         Ogre::VES_POSITION).getSize();
   /* Uvs */
   decl->addElement(0, offset, Ogre::VET_FLOAT2, 
         Ogre::VES_TEXTURE_COORDINATES, 0);

   /* Define vertices and uvs */
   renderOperation.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
   renderOperation.vertexData->vertexStart = 0;
   renderOperation.vertexData->vertexCount = 4;
   renderOperation.useGlobalInstancingVertexBufferIsAvailable = false;
   Ogre::HardwareVertexBufferSharedPtr vbuf =
      Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
            sizeof(float) * 5, 4, Ogre::HardwareBuffer::HBU_STATIC, false);
   renderOperation.vertexData->vertexBufferBinding->setBinding(0, vbuf);
   vbuf->writeData(0, sizeof(float) * 5 * 4, &faceVertices[0]);

   /* Use and define indexes */
   renderOperation.useIndexes = true;
   renderOperation.indexData = OGRE_NEW Ogre::IndexData();
   renderOperation.indexData->indexStart = 0;
   renderOperation.indexData->indexCount = 6;
   renderOperation.indexData->indexBuffer = 
      Ogre::HardwareBufferManager::getSingleton().
         createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, 6, 
               Ogre::HardwareBuffer::HBU_STATIC, false);
   renderOperation.indexData->indexBuffer->writeData(0, 
         sizeof(Ogre::uint16) * 6, &indexData[0]);
}
#else 
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
#endif

/************************************************************************
 *                          calculateCoordinates                        *
 ************************************************************************/
void OgreWidgetRenderable::calculateCoordinates(float& x1, float& y1, 
      float& x2, float& y2)
{
   /* Define vertices at Ogre's -1,1 range. Note the signals, due to 
    * Farso's top-left coordinates are at 0,0 (and from top-left to 
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
#if OGRE_VERSION_MAJOR == 1
void OgreWidgetRenderable::getRenderOperation(Ogre::RenderOperation& op)
{
   op = renderOperation;
}
#else
void OgreWidgetRenderable::getRenderOperation(Ogre::v1::RenderOperation& op, 
      bool casterPass)
{
   OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED,
         "WidgetRenderable doesn't support the old v1::RenderOperations.",
         "Farso::OgreWidgetRenderable::getRenderOperation");
}
#endif

/************************************************************************
 *                           getWorldTransforms                         *
 ************************************************************************/
void OgreWidgetRenderable::getWorldTransforms(Ogre::Matrix4* xform) const
{
#if OGRE_VERSION_MAJOR >= 2
   OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED,
         "WidgetRenderable doesn't implements getWorldTransforms.",
         "Farso::OgreWidgetRenderable::getWorldTransforms");
#else
   /* We should use the attached scene node position transform */
   transformMatrix = Ogre::Matrix4::IDENTITY;
   transformMatrix.setTrans(movable->getParentSceneNode()->getPosition());
   *xform = transformMatrix;
#endif
}

/************************************************************************
 *                             getCastShadows                           *
 ************************************************************************/
bool OgreWidgetRenderable::getCastsShadows(void) const
{
#if OGRE_VERSION_MAJOR >= 2
   OGRE_EXCEPT(Ogre::Exception::ERR_NOT_IMPLEMENTED,
         "WidgetRenderable doesn't implements getCastsShadows.",
         "Farso::OgreWidgetRenderable::getCastShadows");
#else
   return false;
#endif
}

#if OGRE_VERSION_MAJOR == 1
/************************************************************************
 *                         setRenderQueueSubGroup                       *
 ************************************************************************/
void OgreWidgetRenderable::setRenderQueueSubGroup(Ogre::uint8 subGroupId)
{
   /* Setting MAX - subgroup id as the z order of Ogre 1.x is the inverse
    * of subgroups order on Ogre 2.x */
   this->renderQueueSubGroupId = FARSO_OGRE_1_MAX_SUBS - subGroupId;
}

/************************************************************************
 *                               getMaterial                            *
 ************************************************************************/
const Ogre::MaterialPtr& OgreWidgetRenderable::getMaterial(void) const
{
   return this->material;
}

/************************************************************************
 *                               setMaterial                            *
 ************************************************************************/
void OgreWidgetRenderable::setMaterial(Ogre::MaterialPtr& material)
{
   this->material = material;
}

/************************************************************************
 *                          getSquaredViewDepth                         *
 ************************************************************************/
Ogre::Real OgreWidgetRenderable::getSquaredViewDepth(
      const Ogre::Camera* cam) const
{
   return this->renderQueueSubGroupId * this->renderQueueSubGroupId;
}

#endif

}

