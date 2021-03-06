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

#include "openglrenderer.h"
#include "opengldraw.h"
#include "openglsurface.h"
#include "openglwidgetrenderer.h"
#include "../controller.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <kobold/platform.h>

namespace Farso
{

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
OpenGLRenderer::OpenGLRenderer()
{
   this->draw = new OpenGLDraw();
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
OpenGLRenderer::~OpenGLRenderer()
{
}

/**************************************************************************
 *                          createWidgetRenderer                          *
 **************************************************************************/
WidgetRenderer* OpenGLRenderer::createWidgetRenderer(int width, int height)
{
   return new OpenGLWidgetRenderer(width, height);
}

/*************************************************************************
 *                              enter2dMode                              *
 *************************************************************************/
void OpenGLRenderer::enter2dMode()
{
   /* Keep ModelView current state */
   glMatrixMode(GL_MODELVIEW);
   glPushMatrix();

   /* At projection */
   glMatrixMode(GL_PROJECTION);
   /* Keep its previous state */
   glPushMatrix();
   
   /* Set to the 2d one */
   glLoadIdentity();
   glOrtho(0.0f, (GLdouble) Controller::getWidth(), 
           0.0f, (GLdouble) Controller::getHeight(),
           -1, 1);

   /* Identity to current model view */
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

/*************************************************************************
 *                             restore3dMode                             *
 *************************************************************************/
void OpenGLRenderer::restore3dMode()
{
   /* Restore Projection matrix */
   glMatrixMode (GL_PROJECTION);
   glPopMatrix();

   /* Restore Modelview matrix */
   glMatrixMode (GL_MODELVIEW);
   glPopMatrix();
}

/**************************************************************************
 *                          loadImageToSurface                            *
 **************************************************************************/
Surface* OpenGLRenderer::loadImageToSurface(const Kobold::String& filename) 
{
   return new OpenGLSurface(filename);
}


}

