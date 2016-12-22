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

#include "../controller.h"

namespace Farso
{

/*! The junction between OpenGL and the Controller. */
class OpenGLJunction : public ControllerRendererJunction
{
   public:
      /*! Constructor */
      OpenGLJunction();
      /*! Destructor */
      ~OpenGLJunction();

      /*! Set the projection and model view matrix to draw on 2d */
      void enter2dMode();
      /*! Restore the projection and model view matrix to state before
       * call to enter2dMode() */
      void restore3dMode();
      
      const bool shouldManualRender() const {return true;};
};

}

