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
   #include "ogrewidgetmovable.h"
#endif

#include <OGRE/OgreHighLevelGpuProgramManager.h>
#include <OGRE/OgreHighLevelGpuProgram.h>

namespace Farso
{

#define FARSO_DEFAULT_RENDER_QUEUE   240

/*! The junction between controller and renderer for Ogre3d.
 * \note Farso will use render queue from 240 to 244 to render itself. */
class OgreJunction : public ControllerRendererJunction
{
   public:
      /*! Constructor */
      OgreJunction(Kobold::String name, Ogre::SceneManager* sceneManager);
      /*! Destructor */
      ~OgreJunction();

#if FARSO_USE_OGRE_OVERLAY == 1
   #if OGRE_VERSION_MAJOR == 1 || \
       (OGRE_VERSION_MAJOR == 2 && OGRE_VERSION_MINOR == 0)
      /*! \return name of the loaded vertex program */
      Ogre::String getVertexProgramName();
      /*! \return name of the loaded fragment program */
      Ogre::String getFragmentProgramName();
      /*! \return current overlay used */
      Ogre::Overlay* getOverlay() { return overlay; };
   #else
      /*! \return current overlay used */
      Ogre::v1::Overlay* getOverlay() { return overlay; };
   #endif
#endif
      
      /*! \return used scene manager */
      Ogre::SceneManager* getSceneManager();

      void enter2dMode(){};
      void restore3dMode(){};
      const bool shouldManualRender() const {return false;};

   private:
      
      Ogre::SceneManager* sceneManager; /**< SceneManager to use */

#if FARSO_USE_OGRE_OVERLAY == 0
      OgreWidgetMovableFactory* ogreWidgetMovableFactory;
#else
   #if OGRE_VERSION_MAJOR == 1 || \
       (OGRE_VERSION_MAJOR == 2 && OGRE_VERSION_MINOR == 0)
      Ogre::Overlay* overlay; /**< Overlay to render farso to */
      Ogre::HighLevelGpuProgramPtr vertexShader;   /**< Vertex shader to use */
      Ogre::HighLevelGpuProgramPtr fragmentShader;/**< Fragment shader to use */
   #else
      Ogre::v1::Overlay* overlay; /**< Overlay to render farso to */
   #endif
#endif
};



}

#endif


