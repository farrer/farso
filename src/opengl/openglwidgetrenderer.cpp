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

#include "openglwidgetrenderer.h"
#include "openglsurface.h"
#include "opengldraw.h"
#include "../controller.h"

#include <kobold/platform.h>
#if KOBOLD_PLATFORM == KOBOLD_PLATFORM_MACOS
   #include <OpenGL/glu.h>
#else
   #include <GL/glu.h>
#endif
using namespace Farso;

/************************************************************************
 *                        OpenGLWidgetRenderer                          *
 ************************************************************************/
OpenGLWidgetRenderer::OpenGLWidgetRenderer(int width, int height,
      ControllerRendererJunction* junction)
                     :WidgetRenderer(width, height, junction)
{
   Draw* draw = Controller::getDraw();
   posX = 0;
   posY = 0;

   /* Define real dimensions */
   this->realWidth = draw->smallestPowerOfTwo(width);
   this->realHeight = draw->smallestPowerOfTwo(height);

   /* Create our surface */
   this->surface = new OpenGLSurface(name, realWidth, realHeight);

   /* Generate our texture */
   glGenTextures(1, &texture);

   /* Calculate its max coordinate, as size may not equal (as powerOfTwo) */
   propX = (float) (width) / (float) this->realWidth;
   propY = (float) (height) / (float) this->realHeight;
}

/************************************************************************
 *                       ~OpenGLWidgetRenderer                          *
 ************************************************************************/
OpenGLWidgetRenderer::~OpenGLWidgetRenderer()
{
   /* Delete our generated texture */
   glDeleteTextures(1, &texture);

   if(surface != NULL)
   {
      delete surface;
   }
}

/************************************************************************
 *                           uploadSurface                              *
 ************************************************************************/
void OpenGLWidgetRenderer::uploadSurface()
{
   /* Retrieve SDL_Surface from our surface */
   SDL_Surface* sdlSurf = ((OpenGLSurface*) getSurface())->getSurface();

   /* Set the OpenGL texture */
   glBindTexture(GL_TEXTURE_2D, texture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sdlSurf->w, sdlSurf->h,
         0, GL_RGBA, GL_UNSIGNED_BYTE, sdlSurf->pixels);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

/************************************************************************
 *                             setSurface                               *
 ************************************************************************/
void OpenGLWidgetRenderer::doSetPosition(float x, float y)
{
   posX = (int) x;
   posY = (int) Controller::getHeight() - height - y - 2;
}

/************************************************************************
 *                               doHide                                 *
 ************************************************************************/
void OpenGLWidgetRenderer::doHide()
{
}

/************************************************************************
 *                               doShow                                 *
 ************************************************************************/
void OpenGLWidgetRenderer::doShow()
{
}

/************************************************************************
 *                              doRender                                *
 ************************************************************************/
void OpenGLWidgetRenderer::doRender(float depth)
{
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_DEPTH_TEST);

   glEnable(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, texture);

   glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

   glPushMatrix();
      glTranslatef(posX, posY, 0.0f);
      glBegin(GL_QUADS);
         glTexCoord2f(0.0f, propY);
         glVertex3f(0.0f, 0.0f, depth);
         glTexCoord2f(0.0f, 0.0f);
         glVertex3f(0.0f, height, depth);
         glTexCoord2f(propX, 0.0f);
         glVertex3f(width, height, depth);
         glTexCoord2f(propX, propY);
         glVertex3f(width, 0.0f, depth);
      glEnd();
   glPopMatrix();

   glDisable(GL_TEXTURE_2D);

   glEnable(GL_DEPTH_TEST);
   glDisable(GL_BLEND);
}


