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

#ifndef _farso_ogre_widget_renderer_h
#define _farso_ogre_widget_renderer_h

#include "farsoconfig.h"

#include <OGRE/Ogre.h>
#include <kobold/kstring.h>
#include <kobold/target.h>

#include "../widgetrenderer.h"
#include "../surface.h"


#if (OGRE_VERSION_MAJOR == 2 && OGRE_VERSION_MINOR >= 2)
   #include <OGRE/OgrePixelFormatGpu.h>
#else
   #include <OGRE/OgrePixelFormat.h>
#endif

#include <OGRE/OgreSceneNode.h>
#include "ogrewidgetrenderable.h"
#include "ogrewidgetmovable.h"

#if OGRE_VERSION_MAJOR == 1 || \
    (OGRE_VERSION_MAJOR == 2 && OGRE_VERSION_MINOR == 0)
   #include <OGRE/OgreTechnique.h>
   #include <OGRE/OgreMaterialManager.h>
   #include <OGRE/OgreMaterial.h>
#else
   #include <OGRE/Hlms/Unlit/OgreHlmsUnlitDatablock.h>
#endif

namespace Farso
{

/*! The renderer interface. By default, on OGRE 2.1+ it will use a renderable
 * (using a Vao) and a movable to represent the WidgetRenderer of a root 
 * Widget on screen with a scene node.
 * On 1.x or if enabled, will use a fallback renderer, on top of a 
 * Ogre::OverlayElement. */
class OgreWidgetRenderer : public WidgetRenderer
{
   public:
      /*! Constructor
       * \param width -> width of the drawable surface 
       * \param width -> height of the drawable surface */
      OgreWidgetRenderer(int width, int height); 
      /*! Destructor */
      ~OgreWidgetRenderer();

      /*! Not needed for Ogre3D */
      void uploadSurface();
      
      /*! Set render queue subgroup to render */
      void setRenderQueueSubGroup(int renderQueueId);

   protected:
      /*! Create the surface to use. */
      void createSurface();
      /*! Delete the surface (on greater resize). */
      void deleteSurface();

      /*! Do any needed action to set renderer position to x, y (absolute
       * screen coordinates */
      void doSetPosition(float x, float y);

      /*! Do needed actions to hide the renderer */
      void doHide();

      /*! Do needed actions to show the renderer */
      void doShow();

      /*! Do the render (in case of Ogre3D, nothing, as controlled
       * by the engine itself) */
      void doRender();

      /*! Update the texture renderer of this widget. */
      void defineTexture();

      OgreWidgetMovable* movable;
      OgreWidgetRenderable* renderable;
      Ogre::SceneNode* sceneNode;       /**< Scene Node used for 2D object */

#if OGRE_VERSION_MAJOR == 2 && OGRE_VERSION_MINOR >= 2
      Ogre::TextureGpu* texture; /**< The texture used to render the widget */
#else
      Ogre::TexturePtr texture; /**< The texture used to render widget */
      Ogre::PixelBox pixelBox; /**< A pixel box that will contains the 
                                    WidgetRenderer surface pixels */
#endif

#if OGRE_VERSION_MAJOR == 1 || \
    (OGRE_VERSION_MAJOR == 2 && OGRE_VERSION_MINOR == 0)
      Ogre::MaterialPtr material;        /**< Material used */
      Ogre::TextureUnitState* texState;  /**< Texture Unit State */
#else
      Ogre::HlmsUnlitDatablock* datablock; /**< Material datablock used */
#endif
};

}


#endif

