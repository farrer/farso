
#include "widgetrenderer.h"
#include "controller.h"
#include <math.h>

using namespace Farso;

/***********************************************************************
 *                             WidgetRenderer                          *
 ***********************************************************************/
WidgetRenderer::WidgetRenderer(int width, int height)
{
   Farso::Draw* draw = Farso::Controller::getDraw();

   counter++;
   name = "widgetRenderer" + Kobold::StringUtil::toString(counter);

   /* Define dimensions and real dimensions */
   this->width = width;
   this->height = height;
   this->realWidth = draw->smallestPowerOfTwo(width);
   this->realHeight = draw->smallestPowerOfTwo(height);

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

int WidgetRenderer::counter = 0;

