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

#include "ogrewidgetrenderer.h"
#include "ogresurface.h"
#include "ogrejunction.h"
#include "../controller.h"

#include <OGRE/OgreStringConverter.h>
#include <OGRE/OgreDataStream.h>
#include <OGRE/RTShaderSystem/OgreRTShaderSystem.h>
#include <OGRE/OgreMath.h>

using namespace Farso;

/***********************************************************************
 *                             OgreWidgetRenderer                          *
 ***********************************************************************/
OgreWidgetRenderer::OgreWidgetRenderer(int width, int height,
            ControllerRendererJunction* junction) 
                   :WidgetRenderer(width, height, junction)
{
   Farso::Draw* draw = Farso::Controller::getDraw();

   /* Define real dimensions */
   this->realWidth = draw->smallestPowerOfTwo(width);
   this->realHeight = draw->smallestPowerOfTwo(height);

   /* Nullify things */
   this->texState = NULL;

   /* Set junction to use */
   this->ogreJunction = (OgreJunction*) junction;

   /* Create the drawable surface */
   this->surface = new OgreSurface(name, realWidth, realHeight);

   /* Create the material to use */
   Ogre::ResourceManager::ResourceCreateOrRetrieveResult matRes;
   matRes = Ogre::MaterialManager::getSingleton().createOrRetrieve(name,
         "gui");
   material = matRes.first.staticCast<Ogre::Material>();

   defineTexture();

#if FARSO_USE_OGRE_OVERLAY == 1
   /* Define the container and set it to the overlay */
   this->container = (Ogre::OverlayContainer*)
         Ogre::OverlayManager::getSingletonPtr()->createOverlayElement(
            "Panel", name);
   ogreJunction->getOverlay()->add2D(this->container);

   container->setMaterialName(material->getName());

   /* Set container dimensions */
   container->setWidth(((float)realWidth) / 
         Controller::getWidth());
   container->setHeight(((float)realHeight) / 
         Controller::getHeight());
#else
   curX = 0.0f;
   curY = 0.0f;
   curZ = (Controller::getTotalRootWidgets() - 10) * (0.01f);

   /* Create the manual object to use. */
   manualObject = ogreJunction->getSceneManager()->createManualObject(name);

   /* define dimensions for manual object coordinates */
   manualWidth = (realWidth / (float)Controller::getWidth()) * 2.0f;
   manualHeight = (realHeight / (float)Controller::getHeight()) * 2.0f;

   float pX = -1.0f + manualWidth;
   float pY = 1.0f - manualHeight;
   
   /* Set identity rendering */
   manualObject->setUseIdentityProjection(true);
   manualObject->setUseIdentityView(true);
   manualObject->begin(material->getName(), 
                       Ogre::RenderOperation::OT_TRIANGLE_LIST);
      manualObject->position(pX, pY, 0.0f);
      manualObject->textureCoord(1, 1);
      manualObject->position(pX, 1.0f, 0.0f);
      manualObject->textureCoord(1, 0);
      manualObject->position(-1.0f, 1.0f, 0.0f);
      manualObject->textureCoord(0, 0);
      manualObject->position(-1.0f, pY, 0.0f);
      manualObject->textureCoord(0, 1);

      manualObject->index(0);
      manualObject->index(1);
      manualObject->index(2);
      manualObject->index(2);
      manualObject->index(3);
      manualObject->index(0);
   manualObject->end();

   /* Always visible bounding box */
   Ogre::AxisAlignedBox aabInf;
   aabInf.setInfinite();
   manualObject->setBoundingBox(aabInf);
   
   /* Define render queue position */
   manualObject->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY - 1);
   
   /* Attach it */
   sceneNode = ogreJunction->getSceneManager()->getRootSceneNode()->
      createChildSceneNode();
   sceneNode->attachObject(manualObject);
   sceneNode->setPosition(0.0f, 0.0f, curZ);
#endif
}

/***********************************************************************
 *                            ~OgreWidgetRenderer                          *
 ***********************************************************************/
OgreWidgetRenderer::~OgreWidgetRenderer()
{
#if FARSO_USE_OGRE_OVERLAY == 1
   /* Clear the container of the overlay */
   OgreJunction* ogreJunction = (OgreJunction*) junction;
   ogreJunction->getOverlay()->remove2D(container);
   Ogre::OverlayManager::getSingletonPtr()->destroyOverlayElement(container);
#else
   sceneNode->detachObject(manualObject);
   OgreJunction* ogreJunction = (OgreJunction*) junction;
   ogreJunction->getSceneManager()->destroySceneNode(sceneNode);
   ogreJunction->getSceneManager()->destroyManualObject(manualObject);
#endif

   /* Remove Shaders from material */
   Ogre::RTShader::ShaderGenerator::getSingleton().
      removeAllShaderBasedTechniques(material->getName());

   /* Free the material used */
   Ogre::MaterialManager::getSingleton().remove(material->getName());

   /* Free the surface and its texture */
   delete surface;
}

#if FARSO_USE_OGRE_OVERLAY == 1
/***********************************************************************
 *                         getOverlayContainer                         *
 ***********************************************************************/
Ogre::OverlayContainer* OgreWidgetRenderer::getOverlayContainer()
{
   return container;
}
#else
/***********************************************************************
 *                                setZ                                 *
 ***********************************************************************/
void OgreWidgetRenderer::setZ(float z)
{
   curZ = z;
   sceneNode->setPosition(0.0f, 0.0f, z);
}
#endif

/***********************************************************************
 *                            redefineTexture                          *
 ***********************************************************************/
void OgreWidgetRenderer::defineTexture()
{
   OgreSurface* ogreSurface = (OgreSurface*) surface;
   Ogre::Technique* tech = material->getBestTechnique();

   if(!tech)
   {
      /* Let's use the first one. */
      tech = material->getTechnique(0);
   }
   /* As we are using a shader, let's set the technique as RTSS to avoid our
    * Goblin listener try to recreate a technique for it. */
   tech->setSchemeName(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

   /* Verify if need to create a pass */
   int numPasses = tech->getNumPasses();
   if(numPasses == 0)
   {
      tech->createPass();
   }
   /* Get or create new texture */
   int numTextures = tech->getPass(0)->getNumTextureUnitStates();
   Ogre::TexturePtr texture = ogreSurface->getTexture(); 
   if(numTextures != 0)
   {
      texState = tech->getPass(0)->getTextureUnitState(0);
   }
   else
   {
      texState = tech->getPass(0)->createTextureUnitState(texture->getName());
   }
   texState->setTexture(texture);
   texState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
   
   tech->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
   tech->getPass(0)->setDepthWriteEnabled(false);
   tech->getPass(0)->setDepthCheckEnabled(false);
   tech->getPass(0)->setLightingEnabled(false);
   tech->getPass(0)->setCullingMode(Ogre::CULL_NONE);
   tech->getPass(0)->setColourWriteEnabled(true);

   tech->getPass(0)->setVertexProgram(ogreJunction->getVertexProgramName());
   tech->getPass(0)->setFragmentProgram(ogreJunction->getFragmentProgramName());
}

/***********************************************************************
 *                              setPosition                            *
 ***********************************************************************/
void OgreWidgetRenderer::doSetPosition(Ogre::Real x, Ogre::Real y)
{
#if FARSO_USE_OGRE_OVERLAY == 1
   container->setPosition(x / Controller::getWidth(),
         y / Controller::getHeight());
#else
   if((curX == x) && (curY == y))
   {
      /* No need to update at same position */
      return;
   }

   /* Update current position */
   curX = x;
   curY = y;

   /* Recalculate manual object coordinates */
   float x1 = -1.0f + (x / (float)Controller::getWidth()) * 2.0f;
   float y2 = 1.0f - (y / (float)Controller::getHeight()) * 2.0f;
   float x2 = x1 + manualWidth;
   float y1 = y2 - manualHeight;

   /* Update it */
   manualObject->beginUpdate(0);
      
      manualObject->position(x2, y1, 0.0f);
      manualObject->textureCoord(1, 1);
      manualObject->position(x2, y2, 0.0f);
      manualObject->textureCoord(1, 0);
      manualObject->position(x1, y2, 0.0f);
      manualObject->textureCoord(0, 0);
      manualObject->position(x1, y1, 0.0f);
      manualObject->textureCoord(0, 1);

      manualObject->index(0);
      manualObject->index(1);
      manualObject->index(2);
      manualObject->index(2);
      manualObject->index(3);
      manualObject->index(0);
   manualObject->end();
#endif
}
 
/***********************************************************************
 *                                doShow                               *
 ***********************************************************************/
void OgreWidgetRenderer::doShow()
{
#if FARSO_USE_OGRE_OVERLAY == 1
   container->show();
#else
   sceneNode->setVisible(true);
#endif
}

/***********************************************************************
 *                                 doHide                              *
 ***********************************************************************/
void OgreWidgetRenderer::doHide()
{
#if FARSO_USE_OGRE_OVERLAY == 1
   container->hide();
#else
   sceneNode->setVisible(false);
#endif
}

/***********************************************************************
 *                                doRender                             *
 ***********************************************************************/
void OgreWidgetRenderer::doRender(float depth)
{
}

/***********************************************************************
 *                             uploadSurface                           *
 ***********************************************************************/
void OgreWidgetRenderer::uploadSurface()
{
}

