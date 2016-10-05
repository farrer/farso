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
      int screenWidth, int screenHeight, Kobold::String baseDir)
{
   if(!inited)
   {
      /* Define parameters */
      Controller::rendererType = rendererType;
      Controller::width = screenWidth;
      Controller::height = screenHeight;
      Controller::baseDir = baseDir;

      /* Create based on renderer type */
      switch(rendererType)
      {
         case RENDERER_TYPE_OPENGL:
#if FARSO_HAS_OPENGL == 1
            Controller::junction = new OpenGLJunction();
            Controller::draw = new OpenGLDraw();
#else
            Kobold::Log::add("ERROR: OpenGL isn't available!");
#endif
         break;
         case RENDERER_TYPE_OGRE3D:
#if FARSO_HAS_OGRE == 1
            Controller::junction = new OgreJunction();
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
   }
   else
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "WARN: Tried to double init Farso!");
   }
}

/***********************************************************************
 *                                finish                               *
 ***********************************************************************/
void Controller::finish()
{
   assert(inited);
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
   inited = false;
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
 *                       createNewWidgetRenderer                       *
 ***********************************************************************/
WidgetRenderer* Controller::createNewWidgetRenderer(int width, int height)
{
   switch(rendererType)
   {
      case RENDERER_TYPE_OPENGL:
#if FARSO_HAS_OPENGL == 1
         return new OpenGLWidgetRenderer(width, height);
#endif
         break;
      case RENDERER_TYPE_OGRE3D:
#if FARSO_HAS_OGRE == 1
         return new OgreWidgetRenderer(width, height);
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
 *                              loadSkin                               *
 ***********************************************************************/
bool Controller::loadSkin(Kobold::String filename)
{
   if(skin != NULL)
   {
      /* Unload previous defined skin. */
      delete skin;
   }

   bool success = true;
   skin = new Skin(filename);
   if(!skin->getSurface())
   {
      /* Skin couldn't be loaded. Must not use it. */
      delete skin;
      skin = NULL;
      success = false;
   }

   /* Must mark all widgets dirty (to be redraw with the new skin - or
    * the failsafe 'no-skins' mode with failed to load the skin). */
   markAllDirty();

   return success;
}

/***********************************************************************
 *                            unloadSkin                               *
 ***********************************************************************/
void Controller::unloadSkin()
{
   if(skin != NULL)
   {
      delete skin;
      skin = NULL;
      markAllDirty();
   }
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
   return skin;
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
  event.set(owner, type);
}

/***********************************************************************
 *                              addWidget                              *
 ***********************************************************************/
bool Controller::addWidget(Widget* widget)
{
   if(widget->getParent() != NULL)
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR,
            "ERROR: couldn't add a child widget to Controller!");
      return false;
   }
   
   return widgets->insert(widget);
}

/***********************************************************************
 *                         getActiveWindow                             *
 ***********************************************************************/
Window* Controller::getActiveWindow()
{
   return activeWindow;
}

/***********************************************************************
 *                         setActiveWindow                             *
 ***********************************************************************/
void Controller::setActiveWindow(Window* window)
{
   if(window != activeWindow)
   {
#if FARSO_HAS_OGRE == 1
      if(rendererType == RENDERER_TYPE_OGRE3D)
      {
         /* Bring the window to front (removing and reinserting at 
          *  the overlay ) */
         OgreJunction* ogreJunction = (OgreJunction*) junction;
         if(window != NULL)
         {
            OgreWidgetRenderer* ogreRenderer = (OgreWidgetRenderer*) 
               window->getWidgetRenderer();

            ogreJunction->getOverlay()->remove2D(
                  ogreRenderer->getOverlayContainer());
            ogreJunction->getOverlay()->add2D(
                  ogreRenderer->getOverlayContainer());
         }
      }
#endif

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
}

/***********************************************************************
 *                       markToRemoveWidget                            *
 ***********************************************************************/
void Controller::markToRemoveWidget(Widget* widget)
{
   toRemoveWidgets->insert(new WidgetToRemove(widget));
}

/***********************************************************************
 *                            removeWidget                             *
 ***********************************************************************/
bool Controller::removeWidget(Widget* widget)
{
   return widgets->remove(widget);
}

/***********************************************************************
 *                            verifyEvents                             *
 ***********************************************************************/
bool Controller::verifyEvents(Widget* widget, 
      bool leftButtonPressed, bool rightButtonPressed,
      int mouseX, int mouseY, bool checkEvents)
{
   bool gotEvent = false;

   /* Verify widget events */
   if(checkEvents)
   {
      gotEvent = widget->treat(leftButtonPressed, rightButtonPressed, 
               mouseX, mouseY);
   }

   /* redraw the widget (and its children, if needed) */
   if(widget->isDirty())
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

   /* Enter 2d rendering mode */
   junction->enter2dMode();

   event.set(NULL, EVENT_NONE);

   /* Let's remove all to be removed widgets. */
   while(toRemoveWidgets->getTotal() > 0)
   {
      WidgetToRemove* wPtr = (WidgetToRemove*) toRemoveWidgets->getFirst();
      removeWidget(wPtr->widget);
      toRemoveWidgets->remove(wPtr);
   }

   float depth = 0.0f;

   /* Must always treat active window events before others. */
   Window* curActive = activeWindow;
   if(curActive)
   {
      gotEvent |= verifyEvents(curActive, leftButtonPressed, 
            rightButtonPressed, mouseX, mouseY, !gotEvent);
   }

   /* Check all other widgets */
   Widget* w = (Widget*) widgets->getFirst();
   for(int i = 0; i < widgets->getTotal(); i++)
   {
      if((w != curActive))
      {
         gotEvent |= verifyEvents(w, leftButtonPressed, rightButtonPressed, 
               mouseX, mouseY, !gotEvent);
         w->getWidgetRenderer()->render(depth);
         depth += 0.001f;
      }

      w = (Widget*) w->getNext();
   }
   
   if(curActive)
   {
      curActive->getWidgetRenderer()->render(depth);
   }

   /* Restore rendering mode to previously enter 2d one. */
   junction->restore3dMode();

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
 *                               Static                                *
 ***********************************************************************/
Skin* Controller::skin = NULL;
Draw* Controller::draw = NULL;
ControllerRendererJunction* Controller::junction = NULL;
Kobold::List* Controller::widgets = NULL;
Kobold::List* Controller::toRemoveWidgets = NULL;
bool Controller::inited = false;
Window* Controller::activeWindow = NULL;
Event Controller::event(NULL, EVENT_NONE);
RendererType Controller::rendererType = RENDERER_TYPE_OPENGL; 
int Controller::width = 0;
int Controller::height = 0;
Kobold::String Controller::baseDir;
