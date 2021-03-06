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

#ifndef _farso_widget_renderer_h
#define _farso_widget_renderer_h

#include <kobold/kstring.h>
#include <kobold/target.h>
#include <kobold/list.h>

#include "farsoconfig.h"
#include "surface.h"

namespace Farso
{

#define FARSO_WIDGET_RENDERER_FIRST_SUB_GROUP     0
#define FARSO_WIDGET_RENDERER_LAST_SUB_GROUP      6

/*! The renderer interface.
 * \note the surface used should be created only by the createSurface method
 * call. This is needed for the surface creation always be at the 'renderer' 
 * thread (ie: the creation of a texture will require, for some renderers,
 * that the graphical thread is used). */
class WidgetRenderer : public Kobold::ListElement
{
   public:
      /*! Constructor.
       * \param width -> width of the drawable surface 
       * \param width -> height of the drawable surface */
      WidgetRenderer(int width, int height);
      /*! Destructor
       * \note children destructor must delete the created surface. */
      virtual ~WidgetRenderer();

      /*! Set the renderer texture position on screen.
       * \param x -> absolute X coordinate on screen.
       * \param y -> absolute Y coordinate on screen. */
      void setPosition(float x, float y);
     
      /*! Set the target position to the renderer texture on screen,
       * as an animation. This will take steps frames to complete 
       * \param x -> absolute X coordinate on screen.
       * \param y -> absolute Y coordinate on screen. */
      void setTargetPosition(float x, float y, 
            int steps = TARGET_DEFAULT_STEPS);

      /*! \return absolute X position on screen where the widget is rendered.*/
      int getPositionX();
      /*! \return absolute Y position on screen where the widget is rendered.*/
      int getPositionY();

      /*! Reset the size of the widget renderer.
       * \note if the new size is greater than the previous one, will
       *       recreate the used surface, which could be expensive. Thus,
       *       either avoid call it often, or, at first, create it with
       *       the maximum size. */
      void setSize(int width, int height);

      /*! \return verify if the widget texture is animating (true)
       *           or currently static (false). */
      bool needUpdate();

      /*! Do any active animation of the widget */
      void update();

      /*! Show the widget */
      void show();

      /*! Hide the widget */
      void hide();

      /*! \return if widget is actually displayed or hidden */
      bool isVisible();

      /*! Render the widget surface */ 
      void render();

      /*! \return the drawabe surface */
      Surface* getSurface();

      /*! Upload the surface to the renderer */
      virtual void uploadSurface() = 0;

      /*! Set on which render queue should render the widget. This is 
       * used to keep some widgets over others when the render order isn't
       * controlled by their position on the list (ie: when their render 
       * information is grouped at a Vertex Buffer or Vao, for example)
       * \param renderQueueId identifier of the render queue to use.
       * \note the range of render queue ids could vary between each renderer,
       *       (use WIDGET_RENDERER_FIRST_SUB_GROUP and 
       *            WIGET_RENDERER_LAST_SUB_GROUP) */
      virtual void setRenderQueueSubGroup(int renderQueueId) = 0;

   protected:

      /*! Create the surface (and its related structures) to use. */
      virtual void createSurface() = 0;

      /*! Delete the surface (and its size-related structures).
       * \note this function could be called on setSize. */
      virtual void deleteSurface();

      /*! Do any needed action to set renderer position to x, y (absolute
       * screen coordinates */
      virtual void doSetPosition(float x, float y) = 0;

      /*! Do needed actions to hide the renderer */
      virtual void doHide() = 0;

      /*! Do needed actions to show the renderer */
      virtual void doShow() = 0;

      /*! Do the render of the widget surface */ 
      virtual void doRender() = 0;

      Surface* surface;      /**< The drawable surface */

      Kobold::String name; /**< internal name of this renderer */

      bool visible; /**< if visible or not. */

      int width;    /**< widget width */
      int height;   /**< widget height */

      int realWidth;  /**< Width converted to power of two. */
      int realHeight; /**< Height converted to power of two. */

      bool updating; /**< If need to update or not */

      Kobold::Target targetX; /**< Target X position */
      Kobold::Target targetY; /**< Target Y position */

      static int counter; /**< Counter to avoid name clash. */
};

}


#endif

