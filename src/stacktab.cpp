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

#include "stacktab.h"
#include "controller.h"
#include "font.h"
#include <assert.h>

using namespace Farso;

/************************************************************************
 *                            Constructor                               *
 ************************************************************************/
StackTab::StackTab(Widget* parent)
         : Widget(Widget::WIDGET_TYPE_STACK_TAB, 0, 0, 
                  parent->getBody().getWidth(), parent->getBody().getHeight(), 
                  parent),
           tabs(Kobold::LIST_TYPE_ADD_AT_END)
{
   assert(parent != NULL);
   active = NULL;
   Rect pBody = parent->getBody();
   body.set(1, 21, pBody.getWidth() - 1, pBody.getHeight() - 1);
}

/************************************************************************
 *                            Destructor                                *
 ************************************************************************/
StackTab::~StackTab()
{
}

/************************************************************************
 *                           insertTab                                  *
 ************************************************************************/
Container* StackTab::insertTab(Kobold::String name)
{
   /* Create the tab, its container and add it to the list */
   Tab* tab = new Tab();
   tab->name = name;
   tab->container = new Container(Container::TYPE_TOP_LEFT, this);
   tabs.insert(tab);

   /* Make the first one active, if none yet */
   if(active == NULL)
   {
      active = tab;
      tab->container->show();
   }
   else
   {
      /* Not active, must be hidden */
      tab->container->hide();
   }

   /* Done, let's return its container for easier widgets creation. */
   return tab->container;
}

/************************************************************************
 *                             getBody                                  *
 ************************************************************************/
Rect StackTab::getBody()
{
   return body;
}

/************************************************************************
 *                              setDirty                                *
 ************************************************************************/
void StackTab::setDirty()
{
   /* The stack bar don't know how to render its background, so if
    * it's dirty, must set it's parent dirty too. */
   getParent()->setDirty();
   setDirtyValue(true);
}

/************************************************************************
 *                                doDraw                                *
 ************************************************************************/
void StackTab::doDraw(Rect pBody)
{
   Skin* skin = Controller::getSkin();
   Draw* draw = Controller::getDraw();
   Font* font;
   Farso::Surface* surface = getWidgetRenderer()->getSurface();

   /* Define coordinates on parent's surface */
   int x1 = getX() + pBody.getX1();
   int y1 = getY() + pBody.getY1();
   int x2 = x1 + pBody.getWidth() - 1;
   int y2 = y1 + pBody.getHeight() - 1;

   Rect borderSize(0, 0, 0, 0);

   /* Redefine current widget size */
   setSize(pBody.getWidth(), pBody.getHeight());

   /* Get border size (assuming all borders with same size or left width) */
   if(skin)
   {
      Skin::SkinElement el = skin->getSkinElement(Skin::SKIN_TYPE_BORDER_LEFT);
      font = FontManager::getFont(el.getFontName());
      font->setSize(el.getFontSize());
      Rect r = el.getMinSize();
      borderSize.set(0, 0, r.getWidth() - 1, r.getHeight() -1);
   }
   else
   {
      font = FontManager::getDefaultFont();
      font->setSize(10);
   }
  
   /* Define current body */
   Rect parentBody = getParent()->getBody();
   body.set(borderSize.getWidth(), 20 + borderSize.getWidth(), 
            parentBody.getWidth() - borderSize.getWidth(), 
            parentBody.getHeight() - borderSize.getWidth());

   /* Draw body borders */
   if(skin)
   {
      skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_LEFT, 
            x1, y1, x2, y2);
      skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_RIGHT, x1, y1, x2, y2);
      skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_TOP, 
            x1, y1, x2, y2);
      skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_BOTTOM, 
            x1, y1, x2, y2);
   }
   else
   {
      /* Draw Limitators */
      draw->setActiveColor(Colors::colorCont[0]);
      draw->doTwoColorsRectangle(surface, x1, y1, x2, y2,
            Colors::colorCont[1]);
   }

   int incX = getWidth() / tabs.getTotal();
   int curX = x1;

   Tab* tab = (Tab*) tabs.getFirst();
   for(int i = 0; i < tabs.getTotal(); i++)
   {
      if(skin)
      {
         /* Draw with skin */
         if(tab != active)
         {
            skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_LEFT, 
                  curX, y1, curX + incX, y1 + 20);
            skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_RIGHT, 
                  curX + incX, y1, curX + incX, y1 + 20);
            skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_BOTTOM,
                  curX, y1 + 20, 
                  curX + incX, y1 + 20);
         }
      }
      else
      {
         /* No skins, 'clean' farso use */

         /* draw its right separator line */
         draw->setActiveColor(Colors::colorCont[0]);
         draw->doLine(surface, curX + incX, y1 + 1, curX + incX, y1 + 19);
         if(tab != active)
         {
            /* Draw bellow line. Note that active tab must not have it */
            draw->setActiveColor(Colors::colorCont[0]);
            draw->doLine(surface, curX, y1 + 20, curX + incX, y1 + 20);
         }
      }

      /* render the title text */
      draw->setActiveColor(Colors::colorText);
      font->setAlignment(Font::TEXT_CENTERED);
      font->write(surface, Rect(curX, y1, curX + incX, y1 + 20), tab->name);

      tab = (Tab*) tab->getNext();
      curX += incX;
   }
}

/************************************************************************
 *                             doTreat                                  *
 ************************************************************************/
bool StackTab::doTreat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY, int mrX, int mrY)
{
   int incX = getWidth() / tabs.getTotal();
   int curX = 0;

   if((leftButtonPressed) && (mrY >= 0) && (mrY <= 20))
   {
      /* Pressing inside the row of tab selectors, let's see on which */
      Tab* tab = (Tab*) tabs.getFirst();
      for(int i = 0; i < tabs.getTotal(); i++)
      {
         if((mrX >= curX) && (mrX <= curX + incX))
         {
            if(tab != active)
            {
               makeActive(tab);
               Controller::setEvent(this, EVENT_STACKTAB_CHANGED);
               return true;
            }

            Controller::setEvent(this, EVENT_STACKTAB_UNCHANGED);
            return true;
         }

         tab = (Tab*) tab->getNext();
         curX += incX;
      }
   }

   return false;
}

/************************************************************************
 *                          doAfterChildTreat                           *
 ************************************************************************/
void StackTab::doAfterChildTreat()
{
}

/************************************************************************
 *                             makeActive                               *
 ************************************************************************/
void StackTab::makeActive(Tab* tab)
{
   if(active == tab)
   {
      /* already active */
      return;
   }

   if(active)
   {
      /* Remove current visibility of the active one */
      active->container->hide();
   }

   /* Make the tab active */
   active = tab;
   tab->container->show();
   setDirty();
}

