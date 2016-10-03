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

#ifndef _farso_picture_h
#define _farso_picture_h

#include "widget.h"
#include "rect.h"
#include "surface.h"

namespace Farso
{

/*! A picture is an widget to render an image on another widgets (or on its
 * own renderer surface). It won't work without parents. If you want it 
 * to be a single image rendered on screen, consider using Goblin::Image 
 * instead. If you still desire it (for example, for events or set another 
 * widgets inside), try using within a container with explicit coordinates. */
class Picture : public Widget
{
   public:
      /*! Constructor 
       * \param x left coordinate
       * \param y top coordinate
       * \param filename image's file name. 
       * \param parent pointer to parent's window, if any. */
      Picture(int x, int y, Kobold::String filename, Widget* parent);
      /*! Destructor */
      ~Picture();

      /*! \return picture body - the rectangle defining its area */
      Rect getBody();


   protected:
      /*! Do the picture draw on parent's renderer surface */
      void doDraw(Rect pBody);
      /*! Treat picture events */
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed,
                  int mouseX, int mouseY, int mrX, int mrY);
      /*! Not used for pictures. */
      void doAfterChildTreat();
     
   private:
      Surface* image;      /**< Loaded image with the picture */
      Rect body;           /**< same as picture's coordinates */
      bool pressStarted;   /**< If press procedure already started */
};

}

#endif

