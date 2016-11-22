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
#include <OGRE/Overlay/OgreOverlayManager.h>

using namespace Farso;

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


/*************************************************************************
 *                            OgreJunction                               *
 *************************************************************************/
OgreJunction::OgreJunction(Kobold::String name)
{
#if FARSO_USE_OGRE_OVERLAY == 1
   overlay = Ogre::OverlayManager::getSingletonPtr()->create(name);
   overlay->show();
#endif
   sceneManager = NULL;

   /* Create and load our shaders */
   vertexShader = 
      Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
         name + "VP_glsl", 
         Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
         "glsl", Ogre::GPT_VERTEX_PROGRAM);
   vertexShader->setSource(farso_ogre_glsl_vs_source);
   vertexShader->load();

   fragmentShader = 
      Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
         name + "FP_glsl", 
         Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
         "glsl", Ogre::GPT_FRAGMENT_PROGRAM);
   fragmentShader->setSource(farso_ogre_glsl_fs_source);
   fragmentShader->load();
}

/*************************************************************************
 *                           ~OgreJunction                               *
 *************************************************************************/
OgreJunction::~OgreJunction()
{
#if FARSO_USE_OGRE_OVERLAY == 1
   Ogre::OverlayManager::getSingletonPtr()->destroy(overlay);
#endif
}

#if FARSO_USE_OGRE_OVERLAY == 1
/*************************************************************************
 *                             getOverlay                                *
 *************************************************************************/
Ogre::Overlay* OgreJunction::getOverlay()
{
   return overlay;
}
#endif

/*************************************************************************
 *                          setSceneManager                              *
 *************************************************************************/
void OgreJunction::setSceneManager(Ogre::SceneManager* sceneManager)
{
   this->sceneManager = sceneManager;
}

/*************************************************************************
 *                          getSceneManager                              *
 *************************************************************************/
Ogre::SceneManager* OgreJunction::getSceneManager()
{
   return sceneManager;
}

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

