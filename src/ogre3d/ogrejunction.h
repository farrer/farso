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

#ifndef _farso_ogre_junction_h
#define _farso_ogre_junction_h

#include "../controller.h"
#if FARSO_USE_OGRE_OVERLAY == 1
   #include <OGRE/Overlay/OgreOverlay.h>
#else
   #include <OGRE/OgreSceneManager.h>
#endif
#include <OGRE/OgreHighLevelGpuProgramManager.h>
#include <OGRE/OgreHighLevelGpuProgram.h>

namespace Farso
{

/*! The junction between controller and renderer for Ogre3d. */
class OgreJunction : public ControllerRendererJunction
{
   public:
      /*! Constructor */
      OgreJunction(Kobold::String name);
      /*! Destructor */
      ~OgreJunction();

#if FARSO_USE_OGRE_OVERLAY == 1
      /*! \return the overlay used for Farso */
      Ogre::Overlay* getOverlay();
#endif
      /*! Define the scene manager to use */
      void setSceneManager(Ogre::SceneManager* sceneManager);
      /*! \return used scene manager */
      Ogre::SceneManager* getSceneManager();

      /*! \return name of the loaded vertex program */
      Ogre::String getVertexProgramName();
      /*! \return name of the loaded fragment program */
      Ogre::String getFragmentProgramName();


      void enter2dMode(){};
      void restore3dMode(){};

   private:
#if FARSO_USE_OGRE_OVERLAY == 1
      Ogre::Overlay* overlay; /**< Overlay to render farso to */
#endif
      Ogre::SceneManager* sceneManager; /**< SceneManager to use */
      Ogre::HighLevelGpuProgramPtr vertexShader;   /**< Vertex shader to use */
      Ogre::HighLevelGpuProgramPtr fragmentShader;/**< Fragment shader to use */
};



}

#endif


