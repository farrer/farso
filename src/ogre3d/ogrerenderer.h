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

#ifndef _farso_ogre_renderer_h
#define _farso_ogre_renderer_h

#include <OGRE/Ogre.h>
#include "../renderer.h"

#include <OGRE/OgreRenderSystem.h>

#include <OGRE/OgreSceneManager.h>
#include "ogrewidgetmovable.h"

#if OGRE_VERSION_MAJOR == 1
   #include <OGRE/OgreHighLevelGpuProgramManager.h>
   #include <OGRE/OgreHighLevelGpuProgram.h>
#endif

namespace Farso
{

#define FARSO_DEFAULT_RENDER_QUEUE   240

/*! SDL Renderer implemenation */
class OgreRenderer : public Renderer
{
   public:
      /*! Constructor 
       * \param sceneManager pointer to the used Ogre::SceneManager
       * \param renderSystem pointer to the used Ogre::RenderSystem */
      OgreRenderer(Ogre::SceneManager* sceneManager,
            Ogre::RenderSystem* renderSystem);
      /*! Destructor */
      virtual ~OgreRenderer();

      /*! \return new OgreWidgetRenderer */
      WidgetRenderer* createWidgetRenderer(int width, int height) override;

      void enter2dMode() override {};
      void restore3dMode() override {};
      const bool shouldManualRender() const override { return false; };
      Surface* loadImageToSurface(const Kobold::String& filename) override;

      /*! \return pointer to the used Ogre::SceneManager */
      Ogre::SceneManager* getSceneManager() { return sceneManager; };
      /*! \return pointer to the used Ogre::RenderSystem */
      Ogre::RenderSystem* getRenderSystem() { return renderSystem; };

#if OGRE_VERSION_MAJOR == 1 
      /*! \return name of the loaded vertex program */
      Ogre::String getVertexProgramName();
      /*! \return name of the loaded fragment program */
      Ogre::String getFragmentProgramName();
#endif

   private:
      Ogre::SceneManager* sceneManager;
      Ogre::RenderSystem* renderSystem;

#if OGRE_VERSION_MAJOR == 1
      Ogre::HighLevelGpuProgramPtr vertexShader;   /**< Vertex shader to use */
      Ogre::HighLevelGpuProgramPtr fragmentShader;/**< Fragment shader to use */
#endif

      OgreWidgetMovableFactory* ogreWidgetMovableFactory;
};

}

#endif

