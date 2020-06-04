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

#include "sdlrenderer.h"
#include "sdldraw.h"
#include "sdlsurface.h"
#include "sdlwidgetrenderer.h"

namespace Farso
{

/**************************************************************************
 *                              Constructor                               *
 **************************************************************************/
SDLRenderer::SDLRenderer(SDL_Renderer* sdlRenderer)
{
   this->sdlRenderer = sdlRenderer;
   this->draw = new SDLDraw();
}

/**************************************************************************
 *                               Destructor                               *
 **************************************************************************/
SDLRenderer::~SDLRenderer()
{
}

/**************************************************************************
 *                          createWidgetRenderer                          *
 **************************************************************************/
WidgetRenderer* SDLRenderer::createWidgetRenderer(int width, int height)
{
   return new SDLWidgetRenderer(width, height);
}

/**************************************************************************
 *                          loadImageToSurface                            *
 **************************************************************************/
Surface* SDLRenderer::loadImageToSurface(const Kobold::String& filename) 
{
   return new SDLSurface(filename);
}

}

