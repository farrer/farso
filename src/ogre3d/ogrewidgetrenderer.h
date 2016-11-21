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

#include <kobold/kstring.h>
#include <kobold/target.h>

#if FARSO_USE_OGRE_OVERLAY == 1
   #include <OGRE/Overlay/OgreOverlay.h>
   #include <OGRE/Overlay/OgreOverlayManager.h>
   #include <OGRE/Overlay/OgreOverlayContainer.h>
#else
   #include <OGRE/OgreManualObject.h>
   #include <OGRE/OgreSceneNode.h>
#endif

#include <OGRE/OgreTechnique.h>
#include <OGRE/OgreMaterialManager.h>
#include <OGRE/OgreMaterial.h>

#include "../surface.h"
#include "../widgetrenderer.h"

namespace Farso
{

/*! The renderer interface. In fact, just the junction between
 * the created widget's image and Ogre Overlay. A better approach
 * would set big textures atlas and vertex buffers, but it's just a
 * TODO for now. */
class OgreWidgetRenderer : public WidgetRenderer
{
   public:
      /*! Constructor
       * \param width -> width of the drawable surface 
       * \param width -> height of the drawable surface */
      OgreWidgetRenderer(int width, int height, 
            ControllerRendererJunction* junction);
      /*! Destructor */
      ~OgreWidgetRenderer();

#if FARSO_USE_OGRE_OVERLAY == 1
      /*! \return respective OverlayContainer pointer */
      Ogre::OverlayContainer* getOverlayContainer();
#else
      /*! Set Z position (bringing the renderer to front or back */
      void setZ(float z);
#endif

      /*! Not needed for Ogre3D */
      void uploadSurface();

   protected:

      /*! Do any needed action to set renderer position to x, y (absolute
       * screen coordinates */
      void doSetPosition(float x, float y);

      /*! Do needed actions to hide the renderer */
      void doHide();

      /*! Do needed actions to show the renderer */
      void doShow();

      /*! Do the render (in case of Ogre3D, nothing, as controlled
       * by the engine itself) */
      void doRender(float depth);

      /*! Update the texture renderer of this widget. */
      void defineTexture();


#if FARSO_USE_OGRE_OVERLAY == 1
      Ogre::OverlayContainer* container; /**< Container of the texture */
#else
      Ogre::ManualObject* manualObject; /**< 2D object for texture */
      Ogre::SceneNode* sceneNode;       /**< Scene Node used for 2D object */
      float manualWidth;                /**< Width for the manual object */
      float manualHeight;               /**< Height for the manual object */

      float curZ;  /**< current Z position [-1, 1]. */

      /* Bellow to avoid update manual object at same position */
      float curX;                       /**< Current X position */
      float curY;                       /**< Current Y position */
#endif
      Ogre::MaterialPtr material;        /**< Material used */
      Ogre::TextureUnitState* texState;  /**< Texture Unit State */
};

}


#endif

