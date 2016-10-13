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

#ifndef _farso_controller_h
#define _farso_controller_h

#include "button.h"
#include "checkbox.h"
#include "container.h"
#include "cursor.h"
#include "draw.h"
#include "event.h"
#include "label.h"
#include "picture.h"
#include "progressbar.h"
#include "scrollbar.h"
#include "scrolltext.h"
#include "skin.h"
#include "stacktab.h"
#include "textentry.h"
#include "textselector.h"
#include "widget.h"
#include "window.h"

#include <kobold/list.h>

namespace Farso
{

/*! Interface to some junction data between the controller and each renderer
 * implementation. (For example, Ogre3d will keep overlay info here) */
class ControllerRendererJunction
{
   public:
      ControllerRendererJunction(){};
      virtual ~ControllerRendererJunction(){};

      /*! Define rendering state to 2d */
      virtual void enter2dMode() = 0;
      /*! Restore the rendering to the state before call to enter2dMode() */
      virtual void restore3dMode() = 0;
};

enum RendererType
{
   RENDERER_TYPE_OPENGL,
   RENDERER_TYPE_OGRE3D
};

/*! The controller is the main access point to Farso, with all
 * current options and widgets. It's a static class, single per
 * application.
 * \note this class isn't tread safe. */
class Controller
{
   public:
      /*! Init the farso controller system to use.
       * \param rendererType the renderer to use for Farso.
       * \param screenWidth current screen width
       * \param screenHeight current screen height 
       * \param maxCursorSize -> maximum cursor size, in pixels 
       * \param baseDir base directory to open font, skin and cursor files.
       *                Must end with a trail '/'.
       * \note baseDir is ignored on Ogre3d, in favor of its own resource 
       *               manager (thus make sure that baseDir is at a defined
       *               resource group). */
      static void init(RendererType rendererType, 
            int screenWidth, int screenHeight, int maxCursorSize, 
            Kobold::String baseDir);
      /*! Finish with the farso controller (usually called at exit). */
      static void finish();

      /*! \return width available to farso */
      static int getWidth();

      /*! \return height available to farso */
      static int getHeight();

      /*! Set cursor image to use, when mouse cursor is defined
       * \param filename cursor image's filename */
      static void setCursor(Kobold::String filename);

      /*! Load a skin to be the current default one.
       * \param filename skin definition file to load. 
       * \note no need to call unloadSkin first, as it will be automatically
       *       unloaded here.
       * \return if load was successfull. */
      static bool loadSkin(Kobold::String filename);

      /*! Set the current skin to an already loaded one.
       * \note this function is usually called to set to a derived Skin class
       *       implementation, that is created outside the usual 'loadSkin'
       *       function call. */
      static void setSkin(Skin* skin);

      /*! Unload the current loaded skin (if any). */
      static void unloadSkin();

      /*! \return current skin pointer. NULL for no skin use. */
      static Skin* getSkin();

      /*! \return pointer to current draw interface to use */
      static Draw* getDraw();
      
      /*! \return pointer to current overlay used */
      static ControllerRendererJunction* getJunction();

      /*! Create a new ControllerRendererJunction for the current renderer
       *  type, to be used elsewhere.
       *  \param name name to be used for the junction. Must be unique.
       *  \return junction created (or NULL if error) */
      static ControllerRendererJunction* createNewJunction(Kobold::String name);
 
      /*! Add a widget that has no parent.
       * \note: widget's should have no parent.
       * \param widget pointer to the widget to add.
       * \return true if added, false if something went worng. */
      static bool addWidget(Widget* widget);

      /*! Remove a previously added widget from the list 
       * \param widget pointer to the Widget to remove.
       * \return true if success. */
      static bool removeWidget(Widget* widget);

      /*! Mark a widget (from the list) to be removed on next cycle.
       * \note this function is usefull when the widget itself
       *       knowns that it needs to be deleted but still couldn't
       *       (for example, when a window got the its close button was 
       *       pressed).
       * \param widget -> pointer to the widget to be removed on next cycle */
      static void markToRemoveWidget(Widget* widget);

      /*! Verify events on active Farso Widgets accessible throught the 
       * Controller.
       * \param leftButtonPressed state of left mouse button equivalent
       * \param rightButtonPressed state of right mouse button equivalent
       * \param mouseX cursor (mouse or finger) current X coordinate
       * \param mouseY cursor (mouse or finger) current Y coordinate
       * \return true if got an event, that should be accessible with
       *         getLastEvent. */
      static bool verifyEvents(bool leftButtonPressed, bool rightButtonPressed,
            int mouseX, int mouseY);

      /*! Set the window as the current active on (with focus), also
       * setting the current active one as inactive.
       * \param window pointer to the window to set as the active one */
      static void setActiveWindow(Window* window);

      /*! \return pointer to the current active window (or NULL if none). */
      static Window* getActiveWindow();

      /*! The the event got at last verifyEvents call */
      static const Event getLastEvent();

      /*! Set an event happened. Only valid if called by the widget when
       * treating events (otherwise will be cleaned on next 
       * verifyEvent call.
       * \param owner -> pointer to the widget where the event happened
       * \param type -> type of the happened event. */
      static void setEvent(Widget* owner, EventType type);

      /*! Create a new WidgetRenderer based on current renderer type.
       * \param width desired width for widget renderer
       * \param height desired height for widget renderer
       * \param junction pointer to junction to use or NULL to use the
       *        current controller's one.
       * \note The caller is responsible to delete it when no more needed */
      static WidgetRenderer* createNewWidgetRenderer(int width, int height,
            ControllerRendererJunction* junction = NULL);

      /*! Load an image from file to a new surface.
       * \param filename name of the image's file to load
       * \return pointer to the created surface.
       * \note The caller is responsible to delete the surface when no 
       * more needed. */
      static Surface* loadImageToSurface(Kobold::String filename);
      
      /*! \return default group name for the current using renderer */
      static Kobold::String getDefaultGroupName();

      /*! \return current renderer type */
      static RendererType getRendererType();

      /*! \return real filename for fonts, skins and cursors files */
      static Kobold::String getRealFilename(Kobold::String filename);

   private:
      /*! No instances allowed */
      Controller(){};

      /*! Internal class to use the widget on two lists */
      class WidgetToRemove: public Kobold::ListElement
      {
         public:
            WidgetToRemove(Widget* w) { widget = w; };
            Widget* widget;
      };

      /*! Verify events for specific widget */
      static bool verifyEvents(Widget* widget, 
            bool leftButtonPressed, bool rightButtonPressed,
            int mouseX, int mouseY, bool checkEvents);

      /*! Mark all widgets dirty. Usually called when changed skins. */
      static void markAllDirty();

      static Skin* skin; /**< Current skin used, if any (null for no skins). */
      static Draw* draw; /**< Current draw interface. */
      static ControllerRendererJunction* junction; 
      static Kobold::List* toRemoveWidgets; /**< List with current 
                                      'first-level' widgets to be removed */
      static Kobold::List* widgets; /**< List with current 'first-level' 
                                     (ie: without a parent) widgets. */
      static bool inited; /**< Inited flag.*/
      static Window* activeWindow; /**< Current active window */
      static Event event; /**< Last event. */
      static RendererType rendererType; /**< Current controller renderer type*/

      static int width; /**< Width available */
      static int height; /**< height available */

      static Kobold::String baseDir; /**< Base directory */
};

};

#endif

