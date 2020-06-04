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

#ifndef _farso_renderer_h
#define _farso_renderer_h

#include "draw.h"
#include "surface.h"
#include "widgetrenderer.h"

namespace Farso
{

/*! Abstract renderer implementation. It's responsible to create the used Draw 
 * and each WidgetRenderer. */
class Renderer
{
   public:
      /*! Constructor.
       * \note: implementations must set Draw pointer on its constructors */
      Renderer();
      virtual ~Renderer();

      /*! \return respective Draw pointer. */
      Draw* getDraw() { return draw; };

      /*! Create a new WidgetRenderer to be used by a root Widget.
       * \param width desired width
       * \param height desired height
       * \return pointer to the created WidgetRenderer */ 
      virtual WidgetRenderer* createWidgetRenderer(int width, int height) = 0;

      /*! Define rendering state to 2d */
      virtual void enter2dMode() = 0;
      /*! Restore the rendering to the state before call to enter2dMode() */
      virtual void restore3dMode() = 0;

      /*! \return if widget's render call should be made by ourselves (true),
       *  or by the original renderer engine (false). */
      virtual const bool shouldManualRender() const = 0;

      /*! Load an image from file to a new surface.
       * \param filename name of the image's file to load
       * \return pointer to the created surface.
       * \note The caller is responsible to delete the surface when no 
       * more needed. */
      virtual Surface* loadImageToSurface(const Kobold::String& filename) = 0;

   protected:
      Draw* draw; /**< Draw to use */
};

}


#endif


