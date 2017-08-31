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

#include <kobold/log.h>

#include <assert.h>

using namespace Farso;

/***********************************************************************
 *                                 init                                *
 ***********************************************************************/
void Controller::init(RendererType rendererType,
      int screenWidth, int screenHeight, int maxCursorSize,
      Kobold::String baseDir, void* extraInfo)
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
      activeWindow = NULL;
      event.set(NULL, EVENT_NONE);
      widgets = new Kobold::List();
      toRemoveWidgets = new Kobold::List();
      inited = true;
      Colors::init();
      FontManager::init();
#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID && \
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS
      Cursor::init(maxCursorSize);
#endif
   }
   else
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
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
void Controller::setCursor(Kobold::String filename)
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
Kobold::String Controller::getRealFilename(Kobold::String filename)
{
   if(rendererType == RENDERER_TYPE_OPENGL)
   {
      return baseDir + filename;
   }

   return filename;
}

/***********************************************************************
 *                           createNewJunction                         *
 ***********************************************************************/
ControllerRendererJunction* Controller::createNewJunction(Kobold::String name,
      void* extraInfo)
{
   switch(rendererType)
   {
      case RENDERER_TYPE_OPENGL:
#if FARSO_HAS_OPENGL == 1
         return new OpenGLJunction();
#else
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
               "ERROR: OpenGL isn't available!");
      break;
#endif
      case RENDERER_TYPE_OGRE3D:
#if FARSO_HAS_OGRE == 1
         OgreJunction* ogreJunction = new OgreJunction(name, 
               static_cast<Ogre::SceneManager*>(extraInfo));
         return ogreJunction;
#else
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
               "ERROR: Ogre3d isn't available!");
      break;
#endif
   }

   return NULL;
}

/***********************************************************************
 *                       createNewWidgetRenderer                       *
 ***********************************************************************/
WidgetRenderer* Controller::createNewWidgetRenderer(int width, int height)
{
   switch(rendererType)
   {
      case RENDERER_TYPE_OPENGL:
#if FARSO_HAS_OPENGL == 1
         return new OpenGLWidgetRenderer(width, height, junction);
#endif
         break;
      case RENDERER_TYPE_OGRE3D:
#if FARSO_HAS_OGRE == 1
         return new OgreWidgetRenderer(width, height, junction);
#else
      break;
#endif
   }

   return NULL;
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
      default:
         return ""; 
   }

   return "";
}

/***********************************************************************
 *                        loadImageToSurface                           *
 ***********************************************************************/
Surface* Controller::loadImageToSurface(Kobold::String filename)
{
   switch(rendererType)
   {
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
bool Controller::loadSkin(Kobold::String filename)
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
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "ERROR: couldn't add a child widget to Controller!");
      return false;
   }

   mutex.lock();
   bool res = widgets->insert(widget);
   if(widget->getWidgetRenderer() != NULL)
   {
      bringFront(widget);
   }
   mutex.unlock();

   return res;
}

/***********************************************************************
 *                         getActiveWindow                             *
 ***********************************************************************/
Window* Controller::getActiveWindow()
{
   mutex.lock();
   Window* res = activeWindow;
   mutex.unlock();
   return res;
}

/***********************************************************************
 *                         setActiveWindow                             *
 ***********************************************************************/
void Controller::setActiveWindow(Window* window)
{
   mutex.lock();
   if(window != activeWindow)
   {
      Window* lastActive = activeWindow;
      activeWindow = window;

      if((window != NULL) && (!window->isActive()))
      {
         window->activate();
      }
      if((lastActive != NULL) && (lastActive->isActive()) &&
            (window != lastActive))
      {
         lastActive->inactivate();
      }
   }
   forceBringToFrontCall = true;
   mutex.unlock();
}

/***********************************************************************
 *                         setActiveMenu                               *
 ***********************************************************************/
void Controller::setActiveMenu(Menu* menu)
{
   assert(menu == NULL || activeMenu == NULL);
   mutex.lock();
   activeMenu = menu;
   forceBringToFrontCall = true;
   mutex.unlock();
}

/***********************************************************************
 *                            bringFront                               *
 ***********************************************************************/
void Controller::bringFront(Widget* widget)
{
   assert(widget != NULL);
   assert(widget->getParent() == NULL);

   forceBringToFrontCall = false;

   /* Bring the widget to list's front, if not yet */
   if(widget != widgets->getFirst())
   {
      widgets->removeWithoutDelete(widget);
      widgets->insertAtBegin(widget);
   }

   if(!junction->shouldManualRender())
   {
      /* Must reset all widgets sub group renderer.
       * Note: Starting at LAST-1, as mouse cursor should be at last */
      int curSubId = FARSO_WIDGET_RENDERER_LAST_SUB_GROUP -1;

      Widget* w = static_cast<Widget*>(widgets->getFirst());
      for(int i = 0; i < widgets->getTotal(); i++)
      {
         w->getWidgetRenderer()->setRenderQueueSubGroup(curSubId);

         /* Go to next level, if available */
         if(curSubId > FARSO_WIDGET_RENDERER_FIRST_SUB_GROUP)
         {
            curSubId--;
         }

         w = static_cast<Widget*>(w->getNext());
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
      Surface* surface = widget->getWidgetRenderer()->getSurface();
      if(skin != NULL)
      {
         skin->getSurface()->lock();
      }
      surface->lock();
      /* Only need to clear the surface if is itself dirty.
       * In case the children is dirty and will need to affect the
       * parent's (changind its area, for example), the child is responsable
       * for marking parent as dirty to it to work as expected. */
      if(widget->isSelfDirty())
      {
         surface->clear();
      }
      widget->draw();
      surface->unlock();
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
      bringFront((activeWindow) ? activeWindow 
                   : static_cast<Widget*>(widgets->getFirst()));
   }

   Window* curActive = activeWindow;

   if(activeMenu)
   {
      /* When a menu is active, must make sure we have it at front of
       * all other 'root' widgets (even of windows). */
      if( (forceBringToFrontCall) || (activeMenu != widgets->getFirst()) )
      {
         bringFront(activeMenu);
      }
   }
   else
   {
      /* Must always treat active window events before others. */
      if(curActive)
      {
         /* Make sure the active window is first
          * Note: doing here, instead of inside setActiveWindow because
          * that function could be called while looping through the list,
          * which could mess things up on the list. */
         if( (forceBringToFrontCall) || (curActive != widgets->getFirst()) )
         {
            bringFront(curActive);
         }

         gotEvent |= verifyEvents(curActive, leftButtonPressed, 
               rightButtonPressed, mouseX, mouseY, !gotEvent);
      }
   }

   /* Check all other widgets */
   Widget* w = (Widget*) widgets->getFirst();
   for(int i = 0; i < widgets->getTotal(); i++)
   {
      if((w != curActive))
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

      w = (Widget*) w->getNext();
   }

   if(junction->shouldManualRender())
   {
      /* Most render widgets from back to front */
      w = (Widget*) widgets->getLast();
      for(int i = 0; i < widgets->getTotal(); i++)
      {
         if(w->isVisible())
         {
            w->getWidgetRenderer()->render();
         }

         w = (Widget*) w->getPrevious();
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
      cursorRenderer->render();
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
      
      cursorRenderer->render();
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
RendererType Controller::getRendererType()
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
const bool Controller::insertFromJson(const Kobold::String& jsonStr)
{
   WidgetJsonParser parser;
   return parser.loadFromJson(jsonStr);
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
Kobold::List* Controller::widgets = NULL;
Kobold::List* Controller::toRemoveWidgets = NULL;
bool Controller::inited = false;
Window* Controller::activeWindow = NULL;
Menu* Controller::activeMenu = NULL;
Event Controller::event(NULL, EVENT_NONE);
RendererType Controller::rendererType = RENDERER_TYPE_OPENGL; 
int Controller::width = 0;
int Controller::height = 0;
Kobold::String Controller::baseDir;
bool Controller::forceBringToFrontCall = false;
bool Controller::mouseOverWidget = false;
Kobold::Mutex Controller::mutex;
std::map<Kobold::String, Widget*> Controller::idMap;

