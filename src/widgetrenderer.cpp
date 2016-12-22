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

#include "widgetrenderer.h"
#include "controller.h"
#include <math.h>

using namespace Farso;

/***********************************************************************
 *                             WidgetRenderer                          *
 ***********************************************************************/
WidgetRenderer::WidgetRenderer(int width, int height,
      ControllerRendererJunction* junction)
{
   Farso::Draw* draw = Farso::Controller::getDraw();

   counter++;
   this->name = "widgetRenderer" + Kobold::StringUtil::toString(counter);
   this->junction = junction;

   /* Define dimensions and real dimensions */
   this->width = width;
   this->height = height;
   this->realWidth = draw->smallestPowerOfTwo(width);
   this->realHeight = draw->smallestPowerOfTwo(height);

   this->targetX.setCurrent(0);
   this->targetY.setCurrent(0);

   /* Nullify things */
   this->surface = NULL;
   this->updating = false;
   this->visible = true;
}

/***********************************************************************
 *                            ~WidgetRenderer                          *
 ***********************************************************************/
WidgetRenderer::~WidgetRenderer()
{
}

/***********************************************************************
 *                               getSurface                            *
 ***********************************************************************/
Surface* WidgetRenderer::getSurface()
{
   return surface;
}

/***********************************************************************
 *                             getPositionX                            *
 ***********************************************************************/
int WidgetRenderer::getPositionX()
{
   return (int) floorf(targetX.getValue());
}

/***********************************************************************
 *                             getPositionY                            *
 ***********************************************************************/
int WidgetRenderer::getPositionY()
{
   return (int) floorf(targetY.getValue());
}

/***********************************************************************
 *                              setPosition                            *
 ***********************************************************************/
void WidgetRenderer::setPosition(float x, float y)
{
   targetX.setCurrent(x);
   targetY.setCurrent(y);
   doSetPosition(x, y);
}
 
/***********************************************************************
 *                          setTargetPosition                          *
 ***********************************************************************/ 
void WidgetRenderer::setTargetPosition(float x, float y,  int steps)
{
   updating = true;
   targetX.setTarget(x, steps);
   targetY.setTarget(y, steps);
}

/***********************************************************************
 *                             needUpdate                             *
 ***********************************************************************/
bool WidgetRenderer::needUpdate()
{
   return updating;
}

/***********************************************************************
 *                                update                              *
 ***********************************************************************/
void WidgetRenderer::update()
{
   if(updating)
   {
      targetX.update();
      targetY.update();

      doSetPosition(targetX.getValue(), targetY.getValue());

      updating = targetX.needUpdate() || targetY.needUpdate();
   }
}

/***********************************************************************
 *                                 show                                *
 ***********************************************************************/
void WidgetRenderer::show()
{
   visible = true;
   doShow();
}

/***********************************************************************
 *                                 hide                                *
 ***********************************************************************/
void WidgetRenderer::hide()
{
   visible = false;
   doHide();
}

/***********************************************************************
 *                              isVisible                              *
 ***********************************************************************/
bool WidgetRenderer::isVisible()
{
   return visible;
}

/***********************************************************************
 *                               render                                *
 ***********************************************************************/
void WidgetRenderer::render()
{
   if(visible)
   {
      doRender();
   }
}


int WidgetRenderer::counter = 0;

