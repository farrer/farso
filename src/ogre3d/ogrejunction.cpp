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

#include "ogrejunction.h"

#if FARSO_USE_OGRE_OVERLAY == 0
   #include <OGRE/OgreRoot.h>
#else
   #include <OGRE/Overlay/OgreOverlayManager.h>
#endif

#include <kobold/platform.h>

using namespace Farso;


/* Only need those shaders for Ogre 1.x or 2.0 (and with overlays). */
#if FARSO_USE_OGRE_OVERLAY == 1 && \
    (OGRE_VERSION_MAJOR == 1 || \
    (OGRE_VERSION_MAJOR == 2 && OGRE_VERSION_MINOR == 0))

#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID && \
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS
/*! Vertex shader for Farso Ogre3d renderer. Basically,
 * defines texture and polygon coordinates. */
static Ogre::String farso_ogre_glsl_vs_source(
      "void main(void)"
      "{"
      "    gl_TexCoord[0] = gl_MultiTexCoord0;"
      "    gl_FrontColor = gl_Color;"
      "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
      "}"
);
/*! Fragment shader for Farso Ogre3d rendererer. Just use the texture
 * as color definition, without other influences. */
static Ogre::String farso_ogre_glsl_fs_source(
      "uniform sampler2D texture0;"
      "void main(void)"
      "{"
      "    gl_FragColor = texture2D(texture0, gl_TexCoord[0].st) * gl_Color;"
      "}"
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

/*************************************************************************
 *                            OgreJunction                               *
 *************************************************************************/
OgreJunction::OgreJunction(Kobold::String name, 
      Ogre::SceneManager* sceneManager)
{
   this->sceneManager = sceneManager;

#if FARSO_USE_OGRE_OVERLAY == 0
   /* Default renderer, with movable and renderable implementations */

   /* Must set our needed render queues to v2 */
   sceneManager->getRenderQueue()->setRenderQueueMode(
         FARSO_DEFAULT_RENDER_QUEUE, Ogre::RenderQueue::FAST);

   /* We must register the WidgetMoveble factory */
   ogreWidgetMovableFactory = new OgreWidgetMovableFactory();
   Ogre::Root::getSingleton().addMovableObjectFactory(
         ogreWidgetMovableFactory);

#else
   /* Overlay Renderer (fallback) */

   #if OGRE_VERSION_MAJOR == 1 || \
       (OGRE_VERSION_MAJOR == 2 && OGRE_VERSION_MINOR == 0)
      /* Create our overlay */
      overlay = Ogre::OverlayManager::getSingletonPtr()->create(name);
      
      /* Create and load our shaders */
      vertexShader = 
         Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
               name + "VP_glsl", 
               Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
               farso_ogre_shader_type, Ogre::GPT_VERTEX_PROGRAM);
      vertexShader->setSource(farso_ogre_glsl_vs_source);
      vertexShader->load();

      fragmentShader = 
         Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
               name + "FP_glsl", 
               Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
               farso_ogre_shader_type, Ogre::GPT_FRAGMENT_PROGRAM);
      fragmentShader->setSource(farso_ogre_glsl_fs_source);
      fragmentShader->load();
   #else
      /* Create our overlay */
      overlay = Ogre::v1::OverlayManager::getSingletonPtr()->create(name);
      /* Set our render queue group to farso ( != of overlays one), and show it */
      overlay->setRenderQueueGroup(FARSO_DEFAULT_RENDER_QUEUE);
   #endif
   overlay->show();
#endif
}

/*************************************************************************
 *                           ~OgreJunction                               *
 *************************************************************************/
OgreJunction::~OgreJunction()
{

#if FARSO_USE_OGRE_OVERLAY == 0
   /* Unregister our widget's movable factory */
   if(ogreWidgetMovableFactory)
   {
      Ogre::Root::getSingleton().removeMovableObjectFactory(
            ogreWidgetMovableFactory);
      delete ogreWidgetMovableFactory;
      ogreWidgetMovableFactory = NULL;
   }
#else
   /* Destroy our overlays */
   #if OGRE_VERSION_MAJOR == 1 || \
       (OGRE_VERSION_MAJOR == 2 && OGRE_VERSION_MINOR == 0)
      Ogre::OverlayManager::getSingletonPtr()->destroy(overlay);
   #else
      Ogre::v1::OverlayManager::getSingletonPtr()->destroy(overlay);
   #endif
#endif
}

/*************************************************************************
 *                          getSceneManager                              *
 *************************************************************************/
Ogre::SceneManager* OgreJunction::getSceneManager()
{
   return sceneManager;
}

#if OGRE_VERSION_MAJOR == 1 || \
    (FARSO_USE_OGRE_OVERLAY == 1 && OGRE_VERSION_MAJOR == 2 && \
     OGRE_VERSION_MINOR == 0)
/*************************************************************************
 *                       getVertexProgramName                            *
 *************************************************************************/
Ogre::String OgreJunction::getVertexProgramName()
{
   return vertexShader->getName();
}

/*************************************************************************
 *                       getFragmentProgramName                          *
 *************************************************************************/
Ogre::String OgreJunction::getFragmentProgramName()
{
   return fragmentShader->getName();
}
#endif

