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

   /* Create the drawable surface */
   this->surface = new OgreSurface(name, realWidth, realHeight);

   /* Define the container and set it to the overlay */
   this->container = (Ogre::OverlayContainer*)
         Ogre::OverlayManager::getSingletonPtr()->createOverlayElement(
            "Panel", name);
   OgreJunction* ogreJunction = (OgreJunction*) junction;
   ogreJunction->getOverlay()->add2D(this->container);

   /* Create the material to use */
   Ogre::ResourceManager::ResourceCreateOrRetrieveResult matRes;
   matRes = Ogre::MaterialManager::getSingleton().createOrRetrieve(
         this->container->getName(), "gui");
   material = matRes.first.staticCast<Ogre::Material>();

   defineTexture();

   container->setMaterialName(material->getName());

   /* Set container dimensions */
   container->setWidth(((float)realWidth) / 
         Controller::getWidth());
   container->setHeight(((float)realHeight) / 
         Controller::getHeight());
}

/***********************************************************************
 *                            ~OgreWidgetRenderer                          *
 ***********************************************************************/
OgreWidgetRenderer::~OgreWidgetRenderer()
{  
   /* Clear the container of the overlay */
   OgreJunction* ogreJunction = (OgreJunction*) junction;
   ogreJunction->getOverlay()->remove2D(container);
   Ogre::OverlayManager::getSingletonPtr()->destroyOverlayElement(container);

   /* Remove Shaders from material */
   Ogre::RTShader::ShaderGenerator::getSingleton().
      removeAllShaderBasedTechniques(material->getName());

   /* Free the material used */
   Ogre::MaterialManager::getSingleton().remove(material->getName());

   /* Free the surface and its texture */
   delete surface;
}

/***********************************************************************
 *                         getOverlayContainer                         *
 ***********************************************************************/
Ogre::OverlayContainer* OgreWidgetRenderer::getOverlayContainer()
{
   return container;
}

/***********************************************************************
 *                            redefineTexture                          *
 ***********************************************************************/
void OgreWidgetRenderer::defineTexture()
{
   OgreSurface* ogreSurface = (OgreSurface*) surface;
   Ogre::Technique* tech = material->getTechnique(0);

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
}

/***********************************************************************
 *                              setPosition                            *
 ***********************************************************************/
void OgreWidgetRenderer::doSetPosition(Ogre::Real x, Ogre::Real y)
{
   container->setPosition(x / Controller::getWidth(),
         y / Controller::getHeight());
}
 
/***********************************************************************
 *                                doShow                               *
 ***********************************************************************/
void OgreWidgetRenderer::doShow()
{
   container->show();
}

/***********************************************************************
 *                                 doHide                              *
 ***********************************************************************/
void OgreWidgetRenderer::doHide()
{
   container->hide();
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

