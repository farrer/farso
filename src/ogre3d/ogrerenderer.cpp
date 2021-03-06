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

#include "ogrerenderer.h"
#include "ogredraw.h"
#include "ogresurface.h"
#include "ogrewidgetrenderer.h"

#include <OGRE/OgreRoot.h>
#include <OGRE/OgreRenderQueue.h>
#include <kobold/platform.h>

/* Only need those shaders for Ogre 1.x or 2.0 (and with overlays). */
#if OGRE_VERSION_MAJOR == 1

#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID && \
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS
/*! Vertex shader for Farso Ogre3d renderer. Basically,
 * defines texture and polygon coordinates. */
static Ogre::String farso_ogre_glsl_vs_source(
      "void main(void)\n"
      "{\n"
      "    gl_TexCoord[0] = gl_MultiTexCoord0;\n"
      "    gl_FrontColor = gl_Color;\n"
      "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
      "}\n"
);
/*! Fragment shader for Farso Ogre3d rendererer. Just use the texture
 * as color definition, without other influences. */
static Ogre::String farso_ogre_glsl_fs_source(
      "uniform sampler2D texture0;\n"
      "void main(void)\n"
      "{\n"
      "    gl_FragColor = texture2D(texture0, gl_TexCoord[0].st) * gl_Color;\n"
      "}\n"
);
static Ogre::String farso_ogre_shader_type = "glsl";
#else
static Ogre::String farso_ogre_shader_type = "glsles";
static Ogre::String farso_ogre_glsl_vs_source(
      "#version 100\n"
      "attribute highp vec4 position;\n"
      "attribute highp vec4 uv0;\n"
      "varying highp vec4 outUV0;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = position;\n"
      "   outUV0 = uv0;\n"
      "}\n"
);
static Ogre::String farso_ogre_glsl_fs_source(
      "#version 100\n"
      "uniform sampler2D sampler;\n"
      "varying highp vec4 outUV0;\n"
      "void main()\n"
      "{\n"
      "   gl_FragColor = texture2D(sampler, outUV0.xy);\n"
      "}\n"
);
#endif

#endif


namespace Farso
{

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
OgreRenderer::OgreRenderer(Ogre::SceneManager* sceneManager,
      Ogre::RenderSystem* renderSystem)
{
   this->sceneManager = sceneManager;
   this->renderSystem = renderSystem;
   this->draw = new OgreDraw();

#if OGRE_VERSION_MAJOR == 1
   /* Create and load our shaders */
   vertexShader = 
      Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "farso_rendererVP_glsl", 
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
            farso_ogre_shader_type, Ogre::GPT_VERTEX_PROGRAM);
   vertexShader->setSource(farso_ogre_glsl_vs_source);
   vertexShader->load();

   fragmentShader = 
      Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
            "farso_rendererFP_glsl", 
            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
            farso_ogre_shader_type, Ogre::GPT_FRAGMENT_PROGRAM);
   fragmentShader->setSource(farso_ogre_glsl_fs_source);
   fragmentShader->load();
#endif

#if OGRE_VERSION_MAJOR >= 2
   /* Must set our needed render queues to v2 */
   sceneManager->getRenderQueue()->setRenderQueueMode(
         FARSO_DEFAULT_RENDER_QUEUE, Ogre::RenderQueue::FAST);
#endif

   /* We must register the WidgetMoveble factory */
   ogreWidgetMovableFactory = new OgreWidgetMovableFactory();
   Ogre::Root::getSingleton().addMovableObjectFactory(
         ogreWidgetMovableFactory);
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
OgreRenderer::~OgreRenderer()
{
   /* Unregister our widget's movable factory */
   if(ogreWidgetMovableFactory)
   {
      Ogre::Root::getSingleton().removeMovableObjectFactory(
            ogreWidgetMovableFactory);
      delete ogreWidgetMovableFactory;
      ogreWidgetMovableFactory = NULL;
   }
}

/**************************************************************************
 *                          createWidgetRenderer                          *
 **************************************************************************/
WidgetRenderer* OgreRenderer::createWidgetRenderer(int width, int height)
{
   return new OgreWidgetRenderer(width, height);
}

/**************************************************************************
 *                          loadImageToSurface                            *
 **************************************************************************/
Surface* OgreRenderer::loadImageToSurface(const Kobold::String& filename) 
{
   return new OgreSurface(filename,
         Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME); 
}

#if OGRE_VERSION_MAJOR == 1 
/*************************************************************************
 *                       getVertexProgramName                            *
 *************************************************************************/
Ogre::String OgreRenderer::getVertexProgramName()
{
   return vertexShader->getName();
}

/*************************************************************************
 *                       getFragmentProgramName                          *
 *************************************************************************/
Ogre::String OgreRenderer::getFragmentProgramName()
{
   return fragmentShader->getName();
}
#endif


}

