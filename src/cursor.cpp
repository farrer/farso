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
#include "font.h"
#include <assert.h>

using namespace Farso;

#define CURSOR_TIP_MAX_WIDTH   256
#define CURSOR_TIP_MAX_HEIGHT   64

#define CURSOR_TIP_EXPIRE_TIME  220 /**< Time to expire tip, in ms */

/************************************************************************
 *                                 init                                 *
 ************************************************************************/
void Cursor::init(int size)
{
   SDL_ShowCursor(SDL_DISABLE);

   cursors = new Kobold::List();

   /* Note: Cursor and Tip renderers aren't inserted at the Controller's
    * list, so they can always be over other widgets. */

   renderer = Controller::createNewWidgetRenderer(size, size, false);
   /* Make sure we initialize the surface to use. */
   renderer->getSurface();

   tipRenderer = Controller::createNewWidgetRenderer(CURSOR_TIP_MAX_WIDTH, 
         CURSOR_TIP_MAX_HEIGHT, false);
   tipRenderer->getSurface();
   tipRenderer->hide();

   /* Make sure the cursor will always render above all widgets */
   renderer->setRenderQueueSubGroup(FARSO_WIDGET_RENDERER_LAST_SUB_GROUP);
   tipRenderer->setRenderQueueSubGroup(FARSO_WIDGET_RENDERER_LAST_SUB_GROUP);

   tipHeight = 0;
   tipWidth = 0;
   definedTipFont = false;
   tipFont = "";
   tipFontSize = 10;

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
   if(tipRenderer != NULL)
   {
      delete tipRenderer;
      tipRenderer = NULL;
   }
   if(cursors != NULL)
   {
      delete cursors;
      cursors = NULL;
   }
}

/************************************************************************
 *                                 hide                                 *
 ************************************************************************/
void Cursor::hide()
{
   renderer->hide();
   tipRenderer->hide();
}

/************************************************************************
 *                                 show                                 *
 ************************************************************************/
void Cursor::show()
{
   renderer->show();
   if(!textualTip.empty())
   {
      tipRenderer->show();
   }
}

/************************************************************************
 *                                 set                                  *
 ************************************************************************/
void Cursor::set(const Kobold::String& filename)
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
void Cursor::updateByEvent(const SDL_Event& event)
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
void Cursor::setTextualTip(const Kobold::String& tip)
{
   tipTimer.reset();

   /* Only update tip if changed */
   if(textualTip != tip)
   {
      textualTip = tip;
      if(textualTip.empty())
      {
         /* Empty tip, we must not render it */
         tipRenderer->hide();
      }
      else
      {
         /* Let's set its contents, to later set it visible */
         Draw* draw = Controller::getDraw();
         Surface* surface = tipRenderer->getSurface();
         Skin* skin = Controller::getSkin();

         Font* font;
         Color fontColor;
         
         if( (definedTipFont) || (skin == NULL) || 
             (!skin->isElementDefined(Skin::SKIN_TYPE_CURSOR_TEXTUAL_TIP)) )
         {
            /* Must use the defined or default font */
            if(definedTipFont)
            {
               font = FontManager::getFont(tipFont);
            }
            else
            {
               font = FontManager::getDefaultFont();
            }
            font->setSize(tipFontSize);
            font->setAlignment(Font::TEXT_CENTERED);
            fontColor = Colors::colorText;
         }
         else 
         {
            /* Must use font from skin element */
            Skin::SkinElement skEl = skin->getSkinElement(
                  Skin::SKIN_TYPE_CURSOR_TEXTUAL_TIP);
            font = FontManager::getFont(skEl.getFontName());
            font->setSize(skEl.getFontSize());
            font->setAlignment(skEl.getFontAlignment());
            fontColor = skEl.getFontColor();
         }
         assert(font);

         /* Let's define tip size with current font */
         int w = font->getWidth(textualTip) + 10;
         int h = font->getDefaultHeight() + 2 * FONT_HORIZONTAL_DELTA;

         /* make sure in limits */
         if(w > CURSOR_TIP_MAX_WIDTH)
         {
            w = CURSOR_TIP_MAX_WIDTH;
         }
         if(h > CURSOR_TIP_MAX_HEIGHT)
         {
            h = CURSOR_TIP_MAX_HEIGHT;
         }
        
         /* Finally, let's draw it */
         surface->lock();
         
         surface->clear();

         if(skin == NULL)
         {
            /* Draw background */
            draw->setActiveColor(Colors::colorButton);
            draw->doFilledRectangle(surface, 1, 1, w - 2, h - 2);
            draw->setActiveColor(Colors::colorCont[0]);
            draw->doRoundedRectangle(surface, 0, 0, w - 1, h - 1, 
                  Colors::colorCont[0]);

            /* Render its text */
            draw->setActiveColor(fontColor);
            font->write(surface, Rect(0, 0, w - 1, h - 1), textualTip);
         }
         else
         {
            skin->getSurface()->lock();
            if(definedTipFont)
            {
               /* Render with specific font defined */
               skin->drawElement(surface, Skin::SKIN_TYPE_CURSOR_TEXTUAL_TIP,
                     0, 0, w - 1, h - 1, Rect(0, 0, w - 1, h - 1), textualTip,
                     tipFont, tipFontSize, Font::TEXT_CENTERED, 
                     Colors::colorText);
            }
            else
            {
               /* Render with default element's font */
               skin->drawElement(surface, Skin::SKIN_TYPE_CURSOR_TEXTUAL_TIP,
                     0, 0, w - 1, h - 1, Rect(0, 0, w - 1, h - 1), textualTip);
            }
            skin->getSurface()->unlock();
         }
         
         surface->unlock();

         /* Reupload it */
         tipRenderer->uploadSurface();

         /* Set size */
         tipWidth = w;
         tipHeight = h;
      }
   }
}

/************************************************************************
 *                            setTextualTip                             *
 ************************************************************************/
const Kobold::String& Cursor::getTextualTip()
{
   return textualTip;
}

/************************************************************************
 *                             setTipFont                               *
 ************************************************************************/
void Cursor::setTipFont(const Kobold::String& fontFilename, int size)
{
   definedTipFont = true;
   tipFontSize = size;
   tipFont = fontFilename;
}

/************************************************************************
 *                            getTipWidth                               *
 ************************************************************************/
int Cursor::getTipWidth()
{
   return tipWidth;
}

/************************************************************************
 *                            getTipHeight                              *
 ************************************************************************/
int Cursor::getTipHeight()
{
   return tipHeight;
}

/************************************************************************
 *                         checkTipExpiration                           *
 ************************************************************************/
void Cursor::checkTipExpiration()
{
   if( (!textualTip.empty()) &&
       (tipTimer.getMilliseconds() >= CURSOR_TIP_EXPIRE_TIME) )
   {
      /* expired, must clean it */
      setTextualTip("");
   }
}

/************************************************************************
 *                            getRenderer                               *
 ************************************************************************/
Farso::WidgetRenderer* Cursor::getRenderer()
{
   return renderer;
}

/************************************************************************
 *                          getTipRenderer                              *
 ************************************************************************/
Farso::WidgetRenderer* Cursor::getTipRenderer()
{
   return tipRenderer;
}

/************************************************************************
 *                            getCursorImage                            *
 ************************************************************************/
Cursor::CursorImage* Cursor::getCursorImage(const Kobold::String& filename)
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
Farso::WidgetRenderer* Cursor::tipRenderer = NULL;
Kobold::String Cursor::textualTip;
int Cursor::tipHeight = 0;
int Cursor::tipWidth = 0;
bool Cursor::definedTipFont = false;
Kobold::String Cursor::tipFont;
int Cursor::tipFontSize = 0;
Kobold::Timer Cursor::tipTimer;
int Cursor::maxSize = 0;
Cursor::CursorImage* Cursor::current = NULL;

/************************************************************************
 *                             CursorImage                              *
 ************************************************************************/
Cursor::CursorImage::CursorImage(const Kobold::String& filename)
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
const Kobold::String& Cursor::CursorImage::getFilename() const
{
   return filename;
}


#endif

