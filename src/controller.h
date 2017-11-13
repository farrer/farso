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

#include "farsoconfig.h"

#include "button.h"
#include "checkbox.h"
#include "clickablepicture.h"
#include "container.h"
#include "combobox.h"
#include "cursor.h"
#include "draw.h"
#include "event.h"
#include "fileselector.h"
#include "grid.h"
#include "label.h"
#include "labelledpicture.h"
#include "menu.h"
#include "picture.h"
#include "progressbar.h"
#include "scrollbar.h"
#include "scrolltext.h"
#include "skin.h"
#include "spin.h"
#include "stacktab.h"
#include "textentry.h"
#include "textselector.h"
#include "treeview.h"
#include "widget.h"
#include "widgetjsonparser.h"
#include "window.h"

#include <kobold/list.h>
#include <kobold/mutex.h>

#include <map>

namespace Farso
{

/*! Interface to some junction data between the controller and each renderer
 * implementation. 
 * \note Will only have one instance. */
class ControllerRendererJunction
{
   public:
      /*! Construct and define any needed information. */
      ControllerRendererJunction(){};
      virtual ~ControllerRendererJunction(){};

      /*! Define rendering state to 2d */
      virtual void enter2dMode() = 0;
      /*! Restore the rendering to the state before call to enter2dMode() */
      virtual void restore3dMode() = 0;

      /*! \return if widget's render should be made by ourselves (true),
       *  or by the original renderer engine (ie: Ogre) */
      virtual const bool shouldManualRender() const = 0;
};

enum RendererType
{
   RENDERER_TYPE_SDL,
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
       *               resource group). 
       * \param any extra information needed for renderer. For OpenGL, just 
       *        pass NULL, for Ogre3D, the pointer to the sceneManager,
       *        for SDL, the pointer to SDL_Renderer. */
      static void init(const RendererType& rendererType, 
            int screenWidth, int screenHeight, int maxCursorSize, 
            const Kobold::String& baseDir, void* extraInfo);
      /*! Finish with the farso controller (usually called at exit). */
      static void finish();

      /*! \return width available to farso */
      static int getWidth();

      /*! \return height available to farso */
      static int getHeight();

      /*! Set cursor image to use, when mouse cursor is defined
       * \param filename cursor image's filename */
      static void setCursor(const Kobold::String& filename);

      /*! Load a skin to be the current default one.
       * \param filename skin definition file to load. 
       * \note no need to call unloadSkin first, as it will be automatically
       *       unloaded here.
       * \return if load was successfull. */
      static bool loadSkin(const Kobold::String& filename);

      /*! Set the current skin to an already loaded one.
       * \note this function is usually called to set to a derived Skin class
       *       implementation, that is created outside the usual 'loadSkin'
       *       function call. 
       * \note Skin pointer will be therefore owned by the Controller who
       *       should delete it when no longer needed. */
      static void setSkin(Skin* skin);

      /*! Unload the current loaded skin (if any). */
      static void unloadSkin();

      /*! \return current skin pointer. NULL for no skin use. */
      static Skin* getSkin();

      /*! \return pointer to current draw interface to use */
      static Draw* getDraw();
      
      /*! \return pointer to current overlay used */
      static ControllerRendererJunction* getJunction();

      /*! Add a widget that has no parent.
       * \note: widget's should have no parent.
       * \param widget pointer to the widget to add.
       * \return true if added, false if something went worng. */
      static bool addWidget(Widget* widget);

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

      /*! Set a widget to be the current active on (with focus), also
       * setting the current active one as inactive.
       * \note widget must own its renderer to be set as active. Otherwise,
       *       the call will be ineffective.
       * \param widget pointer to the widget to set as the active one */
      static void setActiveWidget(Widget* widget);

      /*! \return pointer to the current active window (or NULL if none). */
      static Widget* getActiveWidget();

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
       * \param insertAtList if will insert at the controller's list or not.
       *        If true, when no more used, should be discarded with a call to
       *        removeWidgetRenderer. If false, should be manually deleted. */
      static WidgetRenderer* createNewWidgetRenderer(int width, int height,
            bool insertAtList=true);
      /*! Remove (and free its memory) a WidgetRenderer 
       * \param renderer pointer to the renderer to remove and free. */
      static void removeWidgetRenderer(WidgetRenderer* renderer);

      /*! Load an image from file to a new surface.
       * \param filename name of the image's file to load
       * \return pointer to the created surface.
       * \note The caller is responsible to delete the surface when no 
       * more needed. */
      static Surface* loadImageToSurface(const Kobold::String& filename);
      
      /*! \return default group name for the current using renderer */
      static Kobold::String getDefaultGroupName();

      /*! \return current renderer type */
      static const RendererType& getRendererType();

      /*! \return real filename for fonts, skins and cursors files */
      static Kobold::String getRealFilename(const Kobold::String& filename);

      /*! \return total number of root (ie: without parents) widgets. */
      static int getTotalRootWidgets();

      /*! \return if mouse was over any farso widget on last call to 
       *          verifyEvents */
      static const bool wasMouseOverWidget() { return mouseOverWidget; };

      /*! Set Widget reference for a string identifier.
       * \note id must be unique.
       * \param id string identifier
       * \param ref widget related. */
      static void setIdReference(const Kobold::String& id, Widget* ref);

      /*! Clear the reference for a string identifier 
       * \param id string identifier to clear the reference to */
      static void clearIdReference(const Kobold::String& id);

      /*! \return Widget pointer by its unique string identifier */
      static Widget* getWidgetById(const Kobold::String& id);

#if FARSO_HAS_RAPIDJSON == 1
      /*! Insert widgets from a JSON string.
       * \param jsonStr JSON string with the widgets to insert.
       * \param listener pointer to the event listener to use for the 
       *        loaded widgets or NULL, for none.
       * \param openWindows if open all windows and set their positions.
       *        If you plan to reset the position to one not defined by the
       *        JSON, you should pass false here, and call to setPosition
       *        and open the window latter.
       * \return true if all defined widgets were inserted, false otherwise */
      static const bool insertFromJson(const Kobold::String& jsonStr, 
            WidgetEventListener* listener=NULL, bool loadWindows=true);
     
      /*! Insert widgets from a JSON string using an specific JSON parser.
       * \note This is used when loading extended widgets.
       * \param jsonStr JSON string with the widgets to insert
       * \param parser pointer to the parser to use
       * \param listener pointer to the event listener to use for the 
       *        loaded widgets or NULL, for none.
       * \param openWindows if open all windows and set their positions.
       *        If you plan to reset the position to one not defined by the
       *        JSON, you should pass false here, and call to setPosition
       *        and open the window latter.
       * \return true if all defined widgets were inserted. */
      static const bool insertFromJson(const Kobold::String& jsonStr, 
            WidgetJsonParser* parser, WidgetEventListener* listener=NULL,
            bool loadWindows=true);
#endif

      /*! Remove an event listener from a widget, thread safelly */
      static void removeEventListener(Widget* owner, 
            WidgetEventListener* listener);
      /*! Add an event listener to a widget, thread safelly */
      static void addEventListener(Widget* owner, 
            WidgetEventListener* listener);

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

      /*! Create a new ControllerRendererJunction for the current renderer
       *  type, to be used elsewhere.
       *  \param name name to be used for the junction. Must be unique.
       *  \param extraInfo needed exra info for renderer. See @init for what 
       *         to pass here.
       *  \return junction created (or NULL if error) */
      static ControllerRendererJunction* createNewJunction(
            const Kobold::String& name, void* extraInfo);

      /*! Verify events for specific widget */
      static bool verifyEvents(Widget* widget, 
            bool leftButtonPressed, bool rightButtonPressed,
            int mouseX, int mouseY, bool checkEvents);

      /*! Mark all widgets dirty. Usually called when changed skins. */
      static void markAllDirty();

      /*! Bring a widget to the front (be rendered first).
       * \param widget pointer to the Widget to be at front. 
       * \note widget must be a 'root' widget (without parents). */
      static void bringFront(Widget* widget);

      static Skin* skin; /**< Current skin used, if any (null for no skins). */
      static Draw* draw; /**< Current draw interface. */
      static ControllerRendererJunction* junction; 
      static Kobold::List* toRemoveWidgets; /**< List with current 
                                      'first-level' widgets to be removed */
      static Kobold::List* widgets; /**< List with current 'first-level' 
                                     (ie: without a parent) widgets. */
      static Kobold::List* renderers; /**< List of active WidgetRenderers */
      static bool inited; /**< Inited flag.*/
      static Widget* activeWidget; /**< Current active widget */
      static Event event; /**< Last event. */
      static RendererType rendererType; /**< Current controller renderer type*/

      static bool forceBringToFrontCall; /**< When the active widget changed 
                                           and we must call bringToFront */

      static int width; /**< Width available */
      static int height; /**< height available */

      static Kobold::String baseDir; /**< Base directory */

      static bool mouseOverWidget; /**< If mouse is under any widget */

      static Kobold::Mutex mutex; /**< Mutex for thread-safe use */

      static std::map<Kobold::String, Widget*> idMap; /**< Map for id->widget */
};

};

#endif

