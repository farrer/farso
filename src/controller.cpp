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

#include "controller.h"
#include "colors.h"
#include "font.h"
#include "widgetjsonparser.h"

#if FARSO_HAS_OGRE == 1
   #include "ogre3d/ogredraw.h"
   #include "ogre3d/ogrejunction.h"
   #include "ogre3d/ogresurface.h"
   #include "ogre3d/ogrewidgetrenderer.h"
#endif

#if FARSO_HAS_OPENGL == 1
   #include "opengl/opengldraw.h"
   #include "opengl/opengljunction.h"
   #include "opengl/openglsurface.h"
   #include "opengl/openglwidgetrenderer.h"
#endif

#include "sdl/sdldraw.h"
#include "sdl/sdljunction.h"
#include "sdl/sdlsurface.h"
#include "sdl/sdlwidgetrenderer.h"


#include <kobold/log.h>

#include <assert.h>

using namespace Farso;

/***********************************************************************
 *                                 init                                *
 ***********************************************************************/
void Controller::init(FontLoader* fontLoader, const RendererType& rendererType,
      int screenWidth, int screenHeight, int maxCursorSize,
      const Kobold::String& baseDir, RendererJunctionInfo* extraInfo)
{
   mutex.lock();
   if(!inited)
   {
      /* Define parameters */
      Controller::rendererType = rendererType;
      Controller::width = screenWidth;
      Controller::height = screenHeight;
      Controller::baseDir = baseDir;

      /* Create based on renderer type */
      Controller::junction = createNewJunction("farso_default_junction",
                                               extraInfo);
      switch(rendererType)
      {
         case RENDERER_TYPE_SDL:
            Controller::draw = new SDLDraw();
         break;
         case RENDERER_TYPE_OPENGL:
#if FARSO_HAS_OPENGL == 1
            Controller::draw = new OpenGLDraw();
#else
            Kobold::Log::add("ERROR: OpenGL isn't available!");
#endif
         break;
         case RENDERER_TYPE_OGRE3D:
#if FARSO_HAS_OGRE == 1
            Controller::draw = new OgreDraw();
#else
            Kobold::Log::add("ERROR: Ogre3d isn't available!");
#endif
        break;
      }

      skin = NULL;
      activeWidget = NULL;
      event.set(NULL, EVENT_NONE);
      renderers = new Kobold::List();
      widgets = new Kobold::List();
      toRemoveWidgets = new Kobold::List();
      inited = true;
      Colors::init();
      FontManager::init(fontLoader);
#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID && \
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS
      Cursor::init(maxCursorSize);
#endif
   }
   else
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "WARN: Tried to double init Farso!");
   }
   mutex.unlock();
}

/***********************************************************************
 *                                finish                               *
 ***********************************************************************/
void Controller::finish()
{
   mutex.lock();

   assert(inited);

#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID && \
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS
   Cursor::finish();
#endif

   if(draw)
   {
      delete draw;
      draw = NULL;
   }
   if(skin)
   {
      delete skin;
      skin = NULL;
   }
   if(toRemoveWidgets)
   {
      /* The widget will be deleted on widgets list destructor. */
      while(toRemoveWidgets->getTotal() > 0)
      {
         toRemoveWidgets->remove(toRemoveWidgets->getFirst());
      }
      delete toRemoveWidgets;
      toRemoveWidgets = NULL;
   }
   if(widgets)
   {
      delete widgets;
      widgets = NULL;
   }
   if(junction)
   {
      delete junction;
      junction = NULL;
   }
   if(renderers)
   {
      /* All renderers should be freed before (specially at 
       * widget's destructor). If not, although nothing will leak as
       * the delete here will free them, its good to be aware of it
       * when debuging. */
      assert(renderers->getTotal() == 0);
      delete renderers;
      renderers = NULL;
   }
   FontManager::finish();

   idMap.clear();

   inited = false;

   mutex.unlock();
}

/***********************************************************************
 *                               getWidth                              *
 ***********************************************************************/
int Controller::getWidth()
{
   return width;
}

/***********************************************************************
 *                              getHeight                              *
 ***********************************************************************/
int Controller::getHeight()
{
   return height;
}

/***********************************************************************
 *                              setCursor                              *
 ***********************************************************************/
void Controller::setCursor(const Kobold::String& filename)
{
#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID && \
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS
   mutex.lock();
      Cursor::set(getRealFilename(filename));
   mutex.unlock();
#endif
}

/***********************************************************************
 *                           getRealFilename                           *
 ***********************************************************************/
Kobold::String Controller::getRealFilename(const Kobold::String& filename)
{
   if(rendererType != RENDERER_TYPE_OGRE3D)
   {
      return baseDir + filename;
   }

   return filename;
}

/***********************************************************************
 *                           createNewJunction                         *
 ***********************************************************************/
ControllerRendererJunction* Controller::createNewJunction(
      const Kobold::String& name, RendererJunctionInfo* extraInfo)
{
   switch(rendererType)
   {
      case RENDERER_TYPE_SDL:
         return new SDLJunction(static_cast<SDLJunctionInfo*>(extraInfo));
      case RENDERER_TYPE_OPENGL:
#if FARSO_HAS_OPENGL == 1
         return new OpenGLJunction();
#else
         Kobold::Log::add(Kobold::LOG_LEVEL_ERROR, 
               "ERROR: OpenGL isn't available!");
      break;
#endif
      case RENDERER_TYPE_OGRE3D:
#if FARSO_HAS_OGRE == 1
         return new OgreJunction(name, 
            static_cast<OgreJunctionInfo*>(extraInfo));
#else
         Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
               "ERROR: Ogre3d isn't available!");
      break;
#endif
   }

   return NULL;
}

/***********************************************************************
 *                       createNewWidgetRenderer                       *
 ***********************************************************************/
WidgetRenderer* Controller::createNewWidgetRenderer(int width, int height,
      bool insertAtList)
{
   WidgetRenderer* renderer = NULL;

   switch(rendererType)
   {
      case RENDERER_TYPE_SDL:
         renderer = new SDLWidgetRenderer(width, height, junction);
      break;
      case RENDERER_TYPE_OPENGL:
#if FARSO_HAS_OPENGL == 1
         renderer = new OpenGLWidgetRenderer(width, height, junction);
#endif
      break;
      case RENDERER_TYPE_OGRE3D:
#if FARSO_HAS_OGRE == 1
         renderer = new OgreWidgetRenderer(width, height, junction);
#else
      break;
#endif
   }

   if((renderer) && (insertAtList))
   {
      renderers->insertAtBegin(renderer);
   }

   return renderer;
}

/***********************************************************************
 *                       removeNewWidgetRenderer                       *
 ***********************************************************************/
void Controller::removeWidgetRenderer(WidgetRenderer* renderer)
{
   if(renderers)
   {
      renderers->remove(renderer);
   }
}

/***********************************************************************
 *                          getDefaultGroupName                        *
 ***********************************************************************/
Kobold::String Controller::getDefaultGroupName()
{
   switch(rendererType)
   {
      case RENDERER_TYPE_OGRE3D:
#if FARSO_HAS_OGRE == 1
         return Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
#endif
      break;
      case RENDERER_TYPE_OPENGL:
      case RENDERER_TYPE_SDL:
         return ""; 
   }

   return "";
}

/***********************************************************************
 *                        loadImageToSurface                           *
 ***********************************************************************/
Surface* Controller::loadImageToSurface(const Kobold::String& filename)
{
   switch(rendererType)
   {
      case RENDERER_TYPE_SDL:
         return new SDLSurface(filename, getDefaultGroupName());
      case RENDERER_TYPE_OPENGL:
#if FARSO_HAS_OPENGL == 1
         return new OpenGLSurface(filename, getDefaultGroupName()); 
#endif
         break;
      case RENDERER_TYPE_OGRE3D:
#if FARSO_HAS_OGRE == 1
         return new OgreSurface(filename, getDefaultGroupName()); 
#else
      break;
#endif
   }

   return NULL;
}

/***********************************************************************
 *                              setSkin                                *
 ***********************************************************************/
void Controller::setSkin(Skin* skin)
{
   if(skin != NULL)
   {
      mutex.lock();
      if(Controller::skin)
      {
         /* Must unload the current one */
         delete Controller::skin;
      }
      Controller::skin = skin;
      markAllDirty();
      mutex.unlock();
   }
}

/***********************************************************************
 *                              loadSkin                               *
 ***********************************************************************/
bool Controller::loadSkin(const Kobold::String& filename)
{
   mutex.lock();
   if(skin != NULL)
   {
      /* Unload previous defined skin. */
      delete skin;
   }

   bool success = true;
   skin = new Skin();
   if((!skin->load(filename)) ||(!skin->getSurface()))
   {
      /* Skin couldn't be loaded. Must not use it. */
      delete skin;
      skin = NULL;
      success = false;
   }

   /* Must mark all widgets dirty (to be redraw with the new skin - or
    * the failsafe 'no-skins' mode when failed to load the skin). */
   markAllDirty();

   mutex.unlock();

   return success;
}

/***********************************************************************
 *                            unloadSkin                               *
 ***********************************************************************/
void Controller::unloadSkin()
{
   mutex.lock();
   if(skin != NULL)
   {
      delete skin;
      skin = NULL;
      markAllDirty();
   }
   mutex.unlock();
}

/***********************************************************************
 *                           markAllDirty                              *
 ***********************************************************************/
void Controller::markAllDirty()
{
   Widget* w = (Widget*) widgets->getFirst();
   for(int i = 0; i < widgets->getTotal(); i++)
   {
      w->setDirty();
      w = (Widget*) w->getNext();
   }
}

/***********************************************************************
 *                               getSkin                               *
 ***********************************************************************/
Skin* Controller::getSkin()
{
   Skin* res;

   mutex.lock();
   res = skin;
   mutex.unlock();

   return res;
}

/***********************************************************************
 *                              getDraw                                *
 ***********************************************************************/
Draw* Controller::getDraw()
{
   return draw;
}

/***********************************************************************
 *                            getJunction                              *
 ***********************************************************************/
ControllerRendererJunction* Controller::getJunction()
{
   return junction;
}
 
/***********************************************************************
 *                             getLastEvent                            *
 ***********************************************************************/
const Event Controller::getLastEvent()
{
   return event;
}

/***********************************************************************
 *                               setEvent                              *
 ***********************************************************************/
void Controller::setEvent(Widget* owner, EventType type)
{
   mutex.lock();
   event.set(owner, type);
   if(owner)
   {
      owner->onEvent(type);
   }
   mutex.unlock();
}

/***********************************************************************
 *                           removeListener                            *
 ***********************************************************************/
void Controller::removeEventListener(Widget* owner, 
      WidgetEventListener* listener)
{
   assert(owner != NULL && listener != NULL);
   mutex.lock();
   owner->removeEventListener(listener);
   mutex.unlock();
}

/***********************************************************************
 *                             addListener                             *
 ***********************************************************************/
void Controller::addEventListener(Widget* owner, WidgetEventListener* listener)
{
   assert(owner != NULL && listener != NULL);
   mutex.lock();
   owner->addEventListener(listener);
   mutex.unlock();
}

/***********************************************************************
 *                              addWidget                              *
 ***********************************************************************/
bool Controller::addWidget(Widget* widget)
{
   assert(widget != NULL);
   assert(widget->getParent() == NULL);

   if(widget->getParent() != NULL)
   {
      Kobold::Log::add(Kobold::LOG_LEVEL_ERROR,
            "ERROR: couldn't add a child widget to Controller!");
      return false;
   }

   mutex.lock();
   bool res = widgets->insert(widget);
   if((widget->getWidgetRenderer() != NULL) && (widget->getParent() == NULL))
   {
      bringFront(widget);
   }
   mutex.unlock();

   return res;
}

/***********************************************************************
 *                         getActiveWidget                             *
 ***********************************************************************/
Widget* Controller::getActiveWidget()
{
   mutex.lock();
   Widget* res = activeWidget;
   mutex.unlock();

   return res;
}

/***********************************************************************
 *                         setActiveWidget                             *
 ***********************************************************************/
void Controller::setActiveWidget(Widget* widget)
{
   mutex.lock();
   if(widget != activeWidget)
   {
      Widget* lastActive = activeWidget;
      activeWidget = widget;

      if((widget != NULL) && (widget->getType() == Widget::WIDGET_TYPE_WINDOW))
      {
         Window* window = static_cast<Window*>(widget);
         if(!window->isActive())
         {
            window->activate();
         }
      }
      if(((lastActive != NULL) && 
          (lastActive->getType() == Widget::WIDGET_TYPE_WINDOW)) &&
          ((widget == NULL) || (widget->getParent() == NULL) ||
           (widget->getType() == Widget::WIDGET_TYPE_WINDOW)))
      {
         Window* lastWindow = static_cast<Window*>(lastActive);
         if(lastWindow->isActive())
         {
            lastWindow->inactivate();
         }
      }
   }
   forceBringToFrontCall = true;
   mutex.unlock();
}

/***********************************************************************
 *                            bringFront                               *
 ***********************************************************************/
void Controller::bringFront(Widget* widget)
{
   assert(widget != NULL);

   forceBringToFrontCall = false;

   /* Bring the widget to list's front, if not yet (to treat first) */
   if((widget->getParent() == NULL) && (widget != widgets->getFirst()))
   {
      widgets->removeWithoutDelete(widget);
      widgets->insertAtBegin(widget);
   }
   
   /* Must bring its own renderer to the front */
   if((widget->haveOwnRenderer()) && 
         (widget->getWidgetRenderer() != renderers->getFirst()))
   {
      renderers->removeWithoutDelete(widget->getWidgetRenderer());
      renderers->insertAtBegin(widget->getWidgetRenderer());
   }

   if(!junction->shouldManualRender())
   {
      /* Must reset all WidgetRenderers sub group.
       * Note: Starting at LAST-1, as mouse cursor should be at last */
      int curSubId = FARSO_WIDGET_RENDERER_LAST_SUB_GROUP - 1;

      WidgetRenderer* renderer = static_cast<WidgetRenderer*>(
            renderers->getFirst());
      for(int i = 0; i < renderers->getTotal(); i++)
      {
         renderer->setRenderQueueSubGroup(curSubId);

         /* Go to next level, if available */
         if(curSubId > FARSO_WIDGET_RENDERER_FIRST_SUB_GROUP)
         {
            curSubId--;
         }

         renderer = static_cast<WidgetRenderer*>(renderer->getNext());
      }
   }
}

/***********************************************************************
 *                       markToRemoveWidget                            *
 ***********************************************************************/
void Controller::markToRemoveWidget(Widget* widget)
{
   mutex.lock();
   toRemoveWidgets->insert(new WidgetToRemove(widget));
   mutex.unlock();
}

/***********************************************************************
 *                            verifyEvents                             *
 ***********************************************************************/
bool Controller::verifyEvents(Widget* widget, 
      bool leftButtonPressed, bool rightButtonPressed,
      int mouseX, int mouseY, bool checkEvents)
{
   bool gotEvent = false;

   if(!widget->isVisible())
   {
      /* No need to treat invisible widgets. */
      return false;
   }

   /* Verify widget events */
   if(checkEvents)
   {
      gotEvent = widget->treat(leftButtonPressed, rightButtonPressed, 
               mouseX, mouseY);
   }

   /* redraw the widget (and its children, if needed) */
   if((widget->isDirty()) && (widget->isVisible()))
   {
      if(skin != NULL)
      {
         skin->getSurface()->lock();
      }
      widget->draw();
      if(skin != NULL)
      {
         skin->getSurface()->unlock();
      }
   }
   /* Update the widget renderer, if needed */
   if(widget->getWidgetRenderer()->needUpdate())
   {
      widget->getWidgetRenderer()->update();
   }

   return gotEvent;
}

/***********************************************************************
 *                            verifyEvents                             *
 ***********************************************************************/
bool Controller::verifyEvents(bool leftButtonPressed, bool rightButtonPressed,
      int mouseX, int mouseY)
{
   bool gotEvent = false;
   
   mutex.lock();
   mouseOverWidget = false;

   /* Enter 2d rendering mode */
   junction->enter2dMode();

   event.set(NULL, EVENT_NONE);

   /* Let's remove all to be removed widgets. */
   bool removed = false;
   
   while(toRemoveWidgets->getTotal() > 0)
   {
      WidgetToRemove* wPtr = (WidgetToRemove*) toRemoveWidgets->getFirst();
      widgets->remove(wPtr->widget);
      toRemoveWidgets->remove(wPtr);
      removed = true;
   }

   /* If removed and have widgets, must bring it to front */
   if((removed) && (widgets->getTotal() > 0))
   {
      bringFront((activeWidget) ? activeWidget
                   : static_cast<Widget*>(widgets->getFirst()));
   }

   if((activeWidget) && (forceBringToFrontCall))
   {
      /* Make sure the active root widget is first
       * Note: doing here, instead of inside setActiveWidget because
       * that function could be called while looping through the list,
       * which could mess things up on the list. */
      bringFront(activeWidget);
   }

   /* Check active widget root first, if any. */
   Widget* activeRoot = (activeWidget) ? activeWidget->getRoot() : NULL;
   if(activeRoot)
   {
      gotEvent |= verifyEvents(activeRoot, leftButtonPressed,
            rightButtonPressed, mouseX, mouseY, !gotEvent);
   }

   /* Check all other widgets */
   Widget* w = static_cast<Widget*>(widgets->getFirst());
   for(int i = 0; i < widgets->getTotal(); i++)
   {
      if(w != activeRoot)
      {
         gotEvent |= verifyEvents(w, leftButtonPressed, rightButtonPressed, 
               mouseX, mouseY, !gotEvent);
      }

      /* Verify if mouse is over any widget (note: stop checking after
       * got that is over a widget) */
      if((!mouseOverWidget) && (w->isVisible()))
      {
         int relMouseX = mouseX - w->getWidgetRenderer()->getPositionX();
         int relMouseY = mouseY - w->getWidgetRenderer()->getPositionY();
         mouseOverWidget = w->isInner(relMouseX, relMouseY);
      }

      w = static_cast<Widget*>(w->getNext());
   }

   if(junction->shouldManualRender())
   {
      /* Most render widgets from back to front */
      WidgetRenderer* renderer = static_cast<WidgetRenderer*>(
            renderers->getLast());
      for(int i = 0; i < renderers->getTotal(); i++)
      {
         if(renderer->isVisible())
         {
            renderer->render();
         }

         renderer = static_cast<WidgetRenderer*>(renderer->getPrevious());
      }
   }
   
   /* Must render mouse cursor on top */
#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID && \
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS

   /* Render cursor */
   WidgetRenderer* cursorRenderer = Farso::Cursor::getRenderer();
   if( (cursorRenderer) && (cursorRenderer->isVisible()) )
   {
      cursorRenderer->setPosition(Farso::Cursor::getX(),
                                  Farso::Cursor::getY());
      if(junction->shouldManualRender())
      {
         cursorRenderer->render();
      }
   }

   /* Check if current tip, if any, expired */
   Farso::Cursor::checkTipExpiration();
   
   /* Render cursor tip */
   cursorRenderer = Farso::Cursor::getTipRenderer();
   bool shouldBeVisible = !Farso::Cursor::getTextualTip().empty();
   if((cursorRenderer) && ((cursorRenderer->isVisible()) || (shouldBeVisible)))
   {
      /* Check position (keeping tip on screen) */
      int pX = Farso::Cursor::getX();
      int pY = Farso::Cursor::getY() - Farso::Cursor::getTipHeight();
      
      /* Note that we don't need to verify after getHeight, as
       * always above cursor, its position will always be inside,
       * even on cursor 'outside' (hotspot at limit) screen.
       * Same for pX < 0. Never happens. */
      if(pX + Farso::Cursor::getTipWidth() > getWidth())
      {
         pX = getWidth() - Farso::Cursor::getTipWidth();
      }
      if(pY < 0)
      {
         pY = 0;
      }

      cursorRenderer->setPosition(pX, pY);
      if(!cursorRenderer->isVisible())
      {
         /* Should be visible, let's show it */
         cursorRenderer->show();
      }
      
      if(junction->shouldManualRender())
      {
         cursorRenderer->render();
      }
   }

#endif

   /* Restore rendering mode to previously enter 2d one. */
   junction->restore3dMode();

   mutex.unlock();

   return gotEvent;
}

/***********************************************************************
 *                        getRendererType                              *
 ***********************************************************************/
const RendererType& Controller::getRendererType()
{
   return rendererType;
}

/***********************************************************************
 *                         getTotalRootWidgets                         *
 ***********************************************************************/
int Controller::getTotalRootWidgets()
{
   mutex.lock();
   int total = widgets->getTotal();
   mutex.unlock();

   return total;
}

#if FARSO_HAS_RAPIDJSON == 1
/***********************************************************************
 *                           insertFromJson                            *
 ***********************************************************************/
const bool Controller::insertFromJson(const Kobold::String& jsonStr,
            WidgetEventListener* listener, bool loadWindows)
{
   WidgetJsonParser parser;
   return insertFromJson(jsonStr, &parser, listener, loadWindows);
}
const bool Controller::insertFromJson(const Kobold::String& jsonStr, 
      WidgetJsonParser* parser, WidgetEventListener* listener, bool loadWindows)
{
   return parser->loadFromJson(jsonStr, listener, loadWindows);
}
#endif

/***********************************************************************
 *                           setIdReference                            *
 ***********************************************************************/
void Controller::setIdReference(const Kobold::String& id, Widget* ref)
{
   assert(getWidgetById(id) == NULL);
   mutex.lock();
   idMap[id] = ref;
   mutex.unlock();
}

/***********************************************************************
 *                          clearIdReference                           *
 ***********************************************************************/
void Controller::clearIdReference(const Kobold::String& id)
{
   if(getWidgetById(id) != NULL)
   {
      mutex.lock();
      idMap[id] = NULL;
      mutex.unlock();
   }
}

/***********************************************************************
 *                            getWidgetById                            *
 ***********************************************************************/
Widget* Controller::getWidgetById(const Kobold::String& id)
{
   Widget* res = NULL;

   mutex.lock();
   std::map<Kobold::String, Widget*>::iterator it = idMap.find(id);
   if(it != idMap.end())
   {
      res = (*it).second;
   }
   mutex.unlock();

   return res;
}

/***********************************************************************
 *                               Static                                *
 ***********************************************************************/
Skin* Controller::skin = NULL;
Draw* Controller::draw = NULL;
ControllerRendererJunction* Controller::junction = NULL;
Kobold::List* Controller::renderers = NULL;
Kobold::List* Controller::widgets = NULL;
Kobold::List* Controller::toRemoveWidgets = NULL;
bool Controller::inited = false;
Widget* Controller::activeWidget = NULL;
Event Controller::event(NULL, EVENT_NONE);
RendererType Controller::rendererType = RENDERER_TYPE_SDL; 
int Controller::width = 0;
int Controller::height = 0;
Kobold::String Controller::baseDir;
bool Controller::forceBringToFrontCall = false;
bool Controller::mouseOverWidget = false;
Kobold::Mutex Controller::mutex;
std::map<Kobold::String, Widget*> Controller::idMap;

