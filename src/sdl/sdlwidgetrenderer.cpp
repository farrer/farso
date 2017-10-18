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

#include "sdlwidgetrenderer.h"
#include "sdlsurface.h"
using namespace Farso;

/***********************************************************************
 *                          SDLWidgetRenderer                          *
 ***********************************************************************/
SDLWidgetRenderer::SDLWidgetRenderer(int width, int height, 
      ControllerRendererJunction* junction)
       : WidgetRenderer(width, height, junction)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
   Uint32 pixelFormat = SDL_PIXELFORMAT_RGBA8888;
#else
   Uint32 pixelFormat = SDL_PIXELFORMAT_ABGR8888;
#endif

   this->sdlJunction = static_cast<SDLJunction*>(junction);
   this->texture = SDL_CreateTexture(sdlJunction->getRenderer(), 
         pixelFormat, SDL_TEXTUREACCESS_STREAMING, width, height);
   this->posX = 0;
   this->posY = 0;

   SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);
}

/***********************************************************************
 *                             createSurface                           *
 ***********************************************************************/
void SDLWidgetRenderer::createSurface()
{
   this->surface = new SDLSurface(name, width, height);
}

/***********************************************************************
 *                         ~SDLWidgetRenderer                          *
 ***********************************************************************/
SDLWidgetRenderer::~SDLWidgetRenderer()
{
   SDL_DestroyTexture(this->texture);
}

/***********************************************************************
 *                             uploadSurface                           *
 ***********************************************************************/
void SDLWidgetRenderer::uploadSurface()
{
   /* From the SDL mail list, by one of the SDL developers:
    * 'if you're keeping a copy in memory anyhow, and/or updating 
    * every pixel for each upload, SDL_UpdateTexture() is the better choice.'
    * As we don't need to read back the texture contents (we are only flushing
    * our surface to it), let's use SDL_UpdateSurface instead of Lock / copy /
    * Unlock. */
   SDLSurface* sdlSurface = static_cast<SDLSurface*>(surface);
   SDL_UpdateTexture(texture, NULL, sdlSurface->getSurface()->pixels, 
         sdlSurface->getSurface()->pitch);
}

/***********************************************************************
 *                            doSetPosition                            *
 ***********************************************************************/
void SDLWidgetRenderer::doSetPosition(float x, float y)
{
   this->posX = x;
   this->posY = y;
}

/***********************************************************************
 *                                doHide                               *
 ***********************************************************************/
void SDLWidgetRenderer::doHide()
{
}

/***********************************************************************
 *                                doShow                               *
 ***********************************************************************/
void SDLWidgetRenderer::doShow()
{
}

/***********************************************************************
 *                               doRender                              *
 ***********************************************************************/
void SDLWidgetRenderer::doRender()
{
   SDL_Rect dest;
   dest.x = posX;
   dest.y = posY;
   dest.w = width;
   dest.h = height;

   SDL_RenderCopy(sdlJunction->getRenderer(), this->texture, NULL, &dest);
}

