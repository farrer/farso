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

#ifndef _farso_window_h
#define _farso_window_h

#include <kobold/kstring.h>

#include "widget.h"
#include "rect.h"
#include "button.h"

namespace Farso
{

/*! Window Widget. A window is an widget container that can be moved (with
 * a click on its title bar) and closed. */
class Window : public Widget
{
   public:
      /*! Constructor of a window.
       * \param width desired window width
       * \param height desired window height
       * \param title -> window title. */ 
      Window(int width, int height, Kobold::String title);

      /*! Destructor */
      ~Window();

      /*! Make the window the one with current focus (if not already has) */
      void activate();
      
      /*! Make the window inactive (ie: not with focus). */
      void inactivate();

      /*! \return if this is the current active (with focus) window */
      bool isActive();

      /*! Actually open the window. Remember that it will be created
       * closed and should be explicitly opened with this function */
      void open();

      /*! Close the window and remove it from Farso::Controller.
       * \note the window will be mark to be closed on next 
       *       Farso::Controller iteration*/
      void close();

      /*! \return if this window could be manually closed or not */
      bool isCloseable();

      /*! \return if this window could be manually moved or not */
      bool isMoveable();

      /*! \return if this window is modal or not */
      bool isModal();

      /*! Set this window as modal (a modal window will
       *  always has focus while opened) */
      void setModal();

      /*! Set the main external pointer to this window.
       * \note: this pointer will be nullyfied when the window is
       *        closed (and destroyed). Thus a normal check to see
       *        if the window was closed by the user is just check 
       *        if the main pointer is NULL or not.
       * \param extr pointer to the external window pointer. */
      void setExternPointer(Window** extr);
      
      Rect getBody();

   protected:

      /*! Draw the window to target renderer */
      void doDraw(Rect pBody);
      /*! Treat window events. */
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY, int mrX, int mrY);
      void doAfterChildTreat();

   private:
      /*! Define current body, based on skins and window sizes */
      void defineBody();

      bool moveable;  /**< Flag if the window can be manually moved */
      bool closeable; /**< Flag if the window can be manually closed */

      bool modal; /**< Flag if the window is modal (a modal window will
                       always has focus while opened). */

      bool active; /** If current with current window focus or not */

      Window** externPointer; /**< Main exter pointer of this window. */

      int clickDiff[2];/**< Mouse diff when first clicked on the window
                            to the window position (usefull for correct
                            window moves with mouse). */
      bool moveStarted; /**< If a window move started or not */
      bool willActivate; /**< If clicked on this inactive window */
      bool clickedInnerWindow; /**< If clicked inner window on last treat */

      Rect titleBar; /**< Rectangle who defines the title bar position */
      Rect body; /**< Rectangle defining the window's body. */
      
      Button* closeButton; /**< Button to close the window. */
};

}

#endif

