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

#ifndef _farso_cursor_h
#define _farso_cursor_h

#include <kobold/platform.h>

#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID && \
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS

#include <kobold/kstring.h>
#include <kobold/list.h>
#include <kobold/mouse.h>

#include "farsoconfig.h"
#include "widgetrenderer.h"

namespace Farso
{

/*! The mouse cursor implementation for Farso. Built on top of Kobold::Mouse */
class Cursor
{
   public:

      /*! Cursor initialize 
       * \param size maximun cursor size (in pixels).
       * \note each cursor will be of maximun size x size, in pixels. */
      static void init(int size);
      /*! Finish the use of mouse cursor*/
      static void finish();  

      /*! Set the current mouse Cursor to an image
       * \param filename -> name of the image to load */
      static void set(Kobold::String filename);

      /*! Update the Cursor position and state 
       * \note: should be called after SDL_PumEvents or similar. */
      static void update();

      /*! Update current mouse by events. Usually used in conjunction
       * with #update */
      static void updateByEvent(SDL_Event event);

      /*! \return current cursor X coordinate on screen */
      static int getX();
      /*! \return current cursor Y coordinate on screen */
      static int getY();
      /*! \return cursor relative to last X coordinate movement  */
      static int getRelativeX();
      /*! \return cursor relative to last Y coordinate movement  */
      static int getRelativeY();
      /*! \return current cursor wheel relative state from last check */
      static int getRelativeWheel();

      /*! Check if left button is pressed */
      static bool isLeftButtonPressed();
      /*! Check if middle button is pressed */
      static bool isMiddleButtonPressed();
      /*! Check if right button is pressed */
      static bool isRightButtonPressed();
      /*! \return if mouse button released on last check. */
      static bool checkButtonRelease(int buttonNumber);
    
      /*! Unload all unused cursors (usually called to save some memory) */
      static void unloadUnusedCursors();

      /*! Set the text to be displayed with mouse tip
       * \param tip text to display. "" for none. */
      static void setTextualTip(Kobold::String tip);

      /*! \return current textual tip */
      static Kobold::String getTextualTip();

      /*! Set which font to use for writing tips
       * \param fontFilename font's filename 
       * \param size font's size
       * \note This font will be used on next setTextualTip with a
       *       different string than the current one. */
      static void setTipFont(Kobold::String fontFilename, int size);

      /*! \return current height to rendering textual tip */
      static int getTipHeight();

      /*! \return pointer to the used renderer */
      static Farso::WidgetRenderer* getRenderer();
      
      /*! \return pointer to the renderer used for textual tips */
      static Farso::WidgetRenderer* getTipRenderer();

   private:

      /*! The cursor image container */
      class CursorImage : public Kobold::ListElement
      {
         public:
            /*! Constructor 
             * \param filename filename of the image to load */
            CursorImage(Kobold::String filename);
            /*! Destructor */
            ~CursorImage();

            /*! \return image's surface pointer */
            Surface* getImage();

            /*! \return image's filename string */
            Kobold::String getFilename();

         private:
            Surface* image;          /**< The loaded image */
            Kobold::String filename; /**< The image filename */
      };

      /*! \return already loaded cursor image or just loaded 
       * if not yet loaded */
      static CursorImage* getCursorImage(Kobold::String filename);

      static Kobold::List* cursors; /**< Loaded cursors images */
      static Farso::WidgetRenderer* renderer; /**< The renderer used */
      static Farso::WidgetRenderer* tipRenderer; /**< Renderer for text tip */
      static Kobold::String textualTip; /**< Current text tip */
      static int tipHeight; /**< Current text tip height */
      static Kobold::String tipFont; /**< Current tip font name */
      static int tipFontSize; /**< Current tip font size */
      static int maxSize; /**< Maximum cursor size */
      static CursorImage* current; /**< Current cursor image, if any */
      static ControllerRendererJunction* junction; /**< Own junction */
};

}

#endif

#endif

