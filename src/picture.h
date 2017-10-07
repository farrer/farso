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
      /*! Constructor, loading an image. 
       * \param x left coordinate
       * \param y top coordinate
       * \param filename image's file name. 
       * \param parent pointer to parent's, if any. */
      Picture(int x, int y, const Kobold::String& filename, Widget* parent);
      /*! Constructor, without image (to be changed and populated later 
       * \param x left coordinate
       * \param y top coordinate
       * \param parent pointer to its parent, if any */
      Picture(int x, int y, int width, int height, Widget* parent);
      /*! Destructor */
      ~Picture();

      /*! Change Picture's image to another one.
       * \param picture pointer to a surface to be displayed at this Picture.
       * \note size must be the lesser or equal to the first loaded image or 
       *       the one defined by the constructor.
       * \note the Surface's memory controll is caller's responsability (ie:
       * all Surfaces passed to the Picture via setImage won't be freed at
       * Picture's destructor like the default loaded one). */
      void setImage(Farso::Surface* picture);

      /*! Change picture's image to the one defined by filename.
       * \param filename path to the image's file.
       * \note size must be the lesser or equal to the first loaded image or 
       *       the one defined by the constructor.
       * \note the loaded image will be freed by us. */
      void setImage(const Kobold::String& filename);

      /*! \return picture body - the rectangle defining its area */
      const Rect& getBody();

      /*! Mark the picture as dirty (usually called by one of its children) */
      void setDirty();

   protected:
      /*! Do the picture draw on parent's renderer surface */
      void doDraw(const Rect& pBody);
      /*! Treat picture events */
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed,
                  int mouseX, int mouseY, int mrX, int mrY);
      /*! Not used for pictures. */
      void doAfterChildTreat();
     
   private:
      Surface* image;      /**< Loaded image with the picture */
      Rect body;           /**< same as picture's coordinates */
      bool ownImage;       /**< if image is owned or not */
};

}

#endif

