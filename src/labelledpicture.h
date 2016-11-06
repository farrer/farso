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

#ifndef _farso_labelled_picture_h
#define _farso_labelled_picture_h

#include "widget.h"
#include "picture.h"
#include "label.h"

namespace Farso
{

/*! A labelled picture is an widget where a picture is presented with an bellow
 * label. Sometimes used like a button on touch-oriented programs */
class LabelledPicture : public Widget
{
   public:
      /*! Constructor 
       * \param x coordinate
       * \param y coordinate
       * \param width sufficient to contain the picture
       * \param height sufficient to contain the picture
       * \param caption text caption to be at label.
       * \param imageFile file
       * \param parent pointer to parent, if any. */
      LabelledPicture(int x, int y, int width, int height, 
            Kobold::String caption, Kobold::String imageFile, 
            Farso::Color frontColor, Farso::Color outlineColor,
            int outlineWidth, Widget* parent);
      /*! Destructor */
      ~LabelledPicture();

      /*! \return current body */
      Farso::Rect getBody();

      /*! Reset LabelledPicture caption.
       * \param text new caption to the label of LabelledPicture */
      void setCaption(Kobold::String text);

      /*! Set as dirty */
      void setDirty();

   protected:
      
      void doDraw(Rect pBody);
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY, int mrX, int mrY);
      void doAfterChildTreat();

   private:

      Label* label;      /**< The related label */
      Picture* picture;  /**< The related picture */
      Farso::Rect body;  /**< Its body */
      bool pressStarted; /**< When pressing a label */
};

}


#endif

