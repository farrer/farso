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

#include "cursor.h"

#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID && \
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS

#include "controller.h"
#include <assert.h>

#if FARSO_HAS_OGRE == 1
   #include "ogre3d/ogrejunction.h"
#endif

using namespace Farso;

/************************************************************************
 *                                 init                                 *
 ************************************************************************/
void Cursor::init(int size)
{
   SDL_ShowCursor(SDL_DISABLE);

   cursors = new Kobold::List();
   junction = Controller::createNewJunction("farso_curso_junction");
#if FARSO_HAS_OGRE == 1
   if(Controller::getRendererType() == RENDERER_TYPE_OGRE3D)
   {
      /* Make sure the cursor overlay is above normal widget's overlay */
      OgreJunction* ogreJunction = (OgreJunction*) junction;
      ogreJunction->getOverlay()->setZOrder(200);
   }
#endif
   renderer = Controller::createNewWidgetRenderer(size, size, junction);
   maxSize = size;
   current = NULL;
}

/************************************************************************
 *                              finish                                  *
 ************************************************************************/
void Cursor::finish()
{
   /* Renable HW Mouse cursor */
   SDL_ShowCursor(SDL_ENABLE);
   current = NULL;

   /* Free our memory */
   if(renderer != NULL)
   {
      delete renderer;
      renderer = NULL;
   }
   if(junction != NULL)
   {
      delete junction;
      junction = NULL;
   }
   if(cursors != NULL)
   {
      delete cursors;
      cursors = NULL;
   }
}

/************************************************************************
 *                                 set                                  *
 ************************************************************************/
void Cursor::set(Kobold::String filename)
{
   current = getCursorImage(filename);
   Surface* imageSurface = current->getImage();
   int imgWidth = imageSurface->getWidth();
   int imgHeight = imageSurface->getHeight();

   /* Verify sizes */
   assert(imgWidth <= maxSize);
   assert(imgHeight <= maxSize);

   /* Draw it to our renderer */
   Draw* draw = Controller::getDraw();
   Surface* rendererSurface = renderer->getSurface();


   rendererSurface->lock();
   imageSurface->lock();

   rendererSurface->clear();

   draw->doStampFill(rendererSurface, 0, 0, imgWidth - 1, imgHeight - 1,
         imageSurface, 0, 0, imgWidth - 1, imgHeight - 1, 
         Draw::STAMP_TYPE_COPY);

   rendererSurface->unlock();
   imageSurface->unlock();

   /* Reupload renderer surface */
   renderer->uploadSurface();
}

/************************************************************************
 *                               update                                 *
 ************************************************************************/
void Cursor::update()
{
   Kobold::Mouse::update();
}

/************************************************************************
 *                           updateByEvent                              *
 ************************************************************************/
void Cursor::updateByEvent(SDL_Event event)
{
   Kobold::Mouse::updateByEvent(event);
}

/************************************************************************
 *                                 getX                                 *
 ************************************************************************/
int Cursor::getX()
{
   return Kobold::Mouse::getX();
}

/************************************************************************
 *                                 getY                                 *
 ************************************************************************/
int Cursor::getY()
{
   return Kobold::Mouse::getY();
}

/************************************************************************
 *                             getRelativeX                             *
 ************************************************************************/
int Cursor::getRelativeX()
{
   return Kobold::Mouse::getRelativeX();
}

/************************************************************************
 *                             getRelativeY                             *
 ************************************************************************/
int Cursor::getRelativeY()
{
   return Kobold::Mouse::getRelativeY();
}

/************************************************************************
 *                           getRelativeWheel                           *
 ************************************************************************/
int Cursor::getRelativeWheel()
{
   return Kobold::Mouse::getRelativeWheel();
}

/************************************************************************
 *                         isLeftButtonPressed                          *
 ************************************************************************/
bool Cursor::isLeftButtonPressed()
{
   return Kobold::Mouse::isLeftButtonPressed();
}

/************************************************************************
 *                       isMiddleButtonPressed                          *
 ************************************************************************/
bool Cursor::isMiddleButtonPressed()
{
   return Kobold::Mouse::isMiddleButtonPressed();
}

/************************************************************************
 *                           isRightButtonPressed                       *
 ************************************************************************/
bool Cursor::isRightButtonPressed()
{
   return Kobold::Mouse::isRightButtonPressed();
}

/************************************************************************
 *                         checkButtonRelease                           *
 ************************************************************************/
bool Cursor::checkButtonRelease(int buttonNumber)
{
   return Kobold::Mouse::checkButtonRelease(buttonNumber);
}

/************************************************************************
 *                         unloadUnusedCursors                          *
 ************************************************************************/
void Cursor::unloadUnusedCursors()
{
   int total = cursors->getTotal();
   CursorImage* cur = (CursorImage*) cursors->getFirst();
   for(int i = 0; i < total; i++)
   {
      if(cur != current)
      {
         /* remove from list */
         CursorImage* next = (CursorImage*) cur->getNext();
         cursors->remove(cur);
         cur = next;
      }
      else
      {
         cur = (CursorImage*) cur->getNext();
      }
   }
}

/************************************************************************
 *                            setTextualTip                             *
 ************************************************************************/
void Cursor::setTextualTip(Kobold::String tip)
{
   //TODO
}

/************************************************************************
 *                            getRenderer                               *
 ************************************************************************/
Farso::WidgetRenderer* Cursor::getRenderer()
{
   return renderer;
}

/************************************************************************
 *                            getCursorImage                            *
 ************************************************************************/
Cursor::CursorImage* Cursor::getCursorImage(Kobold::String filename)
{
   CursorImage* cur = (CursorImage*) cursors->getFirst();
   for(int i = 0; i < cursors->getTotal(); i++)
   {
      if(cur->getFilename() == filename)
      {
         return cur;
      }
      cur = (CursorImage*) cur->getNext();
   }

   /* Cursor not yet loaded, must create (and load) a new one */
   cur = new CursorImage(filename);
   cursors->insert(cur);

   return cur;
}

/************************************************************************
 *                            static members                            *
 ************************************************************************/
Kobold::List* Cursor::cursors = NULL;
Farso::WidgetRenderer* Cursor::renderer = NULL;
int Cursor::maxSize = 0;
Cursor::CursorImage* Cursor::current = NULL;
ControllerRendererJunction* Cursor::junction = NULL;

/************************************************************************
 *                             CursorImage                              *
 ************************************************************************/
Cursor::CursorImage::CursorImage(Kobold::String filename)
{
   this->image = Controller::loadImageToSurface(filename);
   this->filename = filename;
}

/************************************************************************
 *                            ~CursorImage                              *
 ************************************************************************/
Cursor::CursorImage::~CursorImage()
{
   if(image)
   {
      delete image;
   }
}

/************************************************************************
 *                               getImage                               *
 ************************************************************************/
Surface* Cursor::CursorImage::getImage()
{
   return image;
}

/************************************************************************
 *                             getFilename                              *
 ************************************************************************/
Kobold::String Cursor::CursorImage::getFilename()
{
   return filename;
}


#endif

