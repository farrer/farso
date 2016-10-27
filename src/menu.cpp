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

#include "menu.h"
#include "controller.h"
#include "font.h"

#include <kobold/log.h>

using namespace Farso;

/***********************************************************************
 *                                MenuItem                             *
 ***********************************************************************/
Menu::MenuItem::MenuItem(Kobold::String caption, Widget* owner)
{
   this->enabled = true;
   this->visible = true;
   this->owner = owner;
   this->icon = NULL;

   calculateNeededSize(caption, NULL);

   /* Create the label */
   this->label = new Label(0, 0, width, height, caption, owner);
}

/***********************************************************************
 *                                MenuItem                             *
 ***********************************************************************/
Menu::MenuItem::MenuItem(Kobold::String caption, Kobold::String icon, 
                         Widget* owner)
{
   this->enabled = true;
   this->visible = true;
   this->owner = owner;
   this->icon = new Picture(0, 0, icon, owner);
   calculateNeededSize(caption, this->icon);
   this->label = new Label(0, 0, width, height, caption, owner);
}

/***********************************************************************
 *                               ~MenuItem                             *
 ***********************************************************************/
Menu::MenuItem::~MenuItem()
{
}

/***********************************************************************
 *                       calculateNeededSize                           *
 ***********************************************************************/
void Menu::MenuItem::calculateNeededSize(Kobold::String str, 
      Farso::Picture* pic)
{
   Skin* skin = Controller::getSkin();
   Font* font;

   /* Define dimensions from font used for caption */
   if(skin)
   {
      Skin::SkinElement el = skin->getSkinElement(Skin::SKIN_TYPE_LABEL);
      font = FontManager::getFont(el.getFontName());
      font->setSize(el.getFontSize());
   }
   else
   {
      font = FontManager::getDefaultFont();
      font->setSize(10);
   }
   font->setAlignment(Font::TEXT_LEFT);
   width = font->getWidth(str) + 10;
   height = font->getDefaultHeight() + 4;

   /* Apply picture dimensions, if defined */
   if(pic != NULL)
   {
      width += pic->getWidth() + 5;
      if(height < (pic->getHeight() + 2))
      {
         height = pic->getHeight() + 2;
      }
   }
}

/***********************************************************************
 *                             setPosition                             *
 ***********************************************************************/
void Menu::MenuItem::setPosition(int x, int y)
{
   this->label->setPosition(x, y);
}

/***********************************************************************
 *                         getNeededHeight                             *
 ***********************************************************************/
int Menu::MenuItem::getNeededHeight()
{
   return height;
}

/***********************************************************************
 *                         getNeededWidth                              *
 ***********************************************************************/
int Menu::MenuItem::getNeededWidth()
{
   return width;
}

/***********************************************************************
 *                              disable                                *
 ***********************************************************************/
void Menu::MenuItem::disable()
{
   if(enabled)
   {
      if(label)
      {
         label->disable();
      }
      if(icon)
      {
         icon->disable();
      }

      enabled = false;
   }
}

/***********************************************************************
 *                               enable                                *
 ***********************************************************************/
void Menu::MenuItem::enable()
{
   if(!enabled)
   {
      if(label)
      {
         label->enable();
      }
      if(icon)
      {
         icon->enable();
      }

      enabled = true;
   }
}

/***********************************************************************
 *                            isEnabled                                *
 ***********************************************************************/
bool Menu::MenuItem::isEnabled()
{
   return enabled;
}

/***********************************************************************
 *                               hide                                  *
 ***********************************************************************/
void Menu::MenuItem::hide()
{
   if(label)
   {
      label->hide();
   }
   if(icon)
   {
      icon->hide();
   }
   owner->setDirty();

   visible = false;
}

/***********************************************************************
 *                               show                                  *
 ***********************************************************************/
void Menu::MenuItem::show()
{
   if(label)
   {
      label->show();
   }
   if(icon)
   {
      icon->show();
   }
   owner->setDirty();

   visible = true;
}

/***********************************************************************
 *                             isVisible                               *
 ***********************************************************************/
bool Menu::MenuItem::isVisible()
{
   return visible;
}

/***********************************************************************
 *                                Menu                                 *
 ***********************************************************************/
Menu::Menu()
     : Widget(Widget::WIDGET_TYPE_MENU)
{
   this->current = NULL;
   this->creating = false;
   this->pressStarted = false;
   this->grid = new Grid(Grid::GRID_TYPE_HIGHLIGHT_FILL, this);
   this->curWidth = 0;
   this->curHeight = 0;
}

/***********************************************************************
 *                               ~Menu                                 *
 ***********************************************************************/
Menu::~Menu()
{
}

/***********************************************************************
 *                           beginCreate                               *
 ***********************************************************************/
void Menu::beginCreate()
{
   if(getWidgetRenderer() != NULL)
   {
      Kobold::Log::add(
            "Warning: calling beginCreate to an already created menu!");
      return;
   }
   creating = true;
}

/***********************************************************************
 *                           insertItem                                *
 ***********************************************************************/
Menu::MenuItem* Menu::insertItem(Kobold::String text)
{
   if(!creating)
   {
      Kobold::Log::add(
            "Warning: won't insert menu items while not creating it!");
      return NULL;
   }

   /* Create and insert item */
   MenuItem* item = new MenuItem(text, grid);
   items.insertAtEnd(item);

   /* Check current menu width */
   if(item->getNeededWidth() > curWidth)
   {
      curWidth = item->getNeededWidth();
   }


   return item;
}

/***********************************************************************
 *                           insertItem                                *
 ***********************************************************************/
Menu::MenuItem* Menu::insertItem(Kobold::String text, Kobold::String icon)
{
   if(!creating)
   {
      Kobold::Log::add(
            "Warning: won't insert menu items while not creating it!");
      return NULL;
   }
   
   /* Create and insert item */
   MenuItem* item = new MenuItem(text, icon, grid);
   items.insertAtEnd(item);

   /* Check current menu width */
   if(item->getNeededWidth() > curWidth)
   {
      curWidth = item->getNeededWidth();
   }

   return item;
}
 
/***********************************************************************
 *                             endCreate                               *
 ***********************************************************************/     
void Menu::endCreate()
{
   if(!creating)
   {
      Kobold::Log::add("Warning: called endCreate while not creating a menu!");
      return;
   }
   creating = false;

   /* Calculate total needed dimensions */
   int width = 1;
   int height = 1;

   MenuItem* item = (MenuItem*) items.getFirst();
   for(int i = 0; i < items.getTotal(); i++)
   {
      if(item->getNeededWidth() > width)
      {
         width = item->getNeededWidth();
      }
      height += item->getNeededHeight();

      item = (MenuItem*) item->getNext();
   }

   /* define menu size (creating its renderer) */
   Widget::setSize(width, height);

   /* Set body */
   body.set(0, 0, width - 1, height - 1);

   hide();
}

/***********************************************************************
 *                               open                                  *
 ***********************************************************************/
void Menu::open(int x, int y)
{
   setPosition(x, y);
   show();

   /* Let's define each item position, and its grid element */
   int pX = 0, 
       pY = 0;
   grid->clearElements();

   MenuItem* item = (MenuItem*) items.getFirst();
   for(int i = 0; i < items.getTotal(); i++)
   {
      if(item->isVisible())
      {
         /* Define item position and its grid */
         item->setPosition(pX, pY);

         grid->addElement(pX + 2, pY + 2, curWidth - 4,
               item->getNeededHeight() - 2);

         pY += item->getNeededHeight();
      }

      item = (MenuItem*) item->getNext();
   }

   /* define current height */
   curHeight = pY + 1;

   Controller::setActiveMenu(this);
}

/***********************************************************************
 *                              close                                  *
 ***********************************************************************/
void Menu::close()
{
   hide();
   Controller::setActiveMenu(NULL);
}

/***********************************************************************
 *                              getItem                                *
 ***********************************************************************/
Menu::MenuItem* Menu::getItem(int index)
{
   if(index >= getTotal())
   {
      Kobold::Log::add("Warning: getItem with index after menu total!");
      return NULL;
   }

   MenuItem* item = (MenuItem*) items.getFirst();
   for(int i = 0; i < index - 1; i++)
   {
      item = (MenuItem*) item->getNext();
   }

   return item;
}

/***********************************************************************
 *                              getItem                                *
 ***********************************************************************/
Menu::MenuItem* Menu::getItem(int rX, int rY)
{
   int height = 1;
   
   MenuItem* item = (MenuItem*) items.getFirst();
   for(int i = 0; i < items.getTotal(); i++)
   {
      if(item->isVisible())
      {
         if((item->isEnabled()) && (rY >= height) && 
            (rY <= height + item->getNeededHeight()))
         {
            /* Got the item */
            return item;
         }
         height += item->getNeededHeight();
      }

      item = (MenuItem*) item->getNext();
   }

   return NULL;
}

/***********************************************************************
 *                          getCurrentItem                             *
 ***********************************************************************/
Menu::MenuItem* Menu::getCurrentItem()
{
   return current;
}

/***********************************************************************
 *                             setSize                                 *
 ***********************************************************************/
void Menu::setSize(int width, int height)
{
   Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, 
         "Error: Menu::setSize should never be called!");
}

/***********************************************************************
 *                             getBody                                 *
 ***********************************************************************/
Rect Menu::getBody()
{
   return body;
}

/***********************************************************************
 *                             doDraw                                  *
 ***********************************************************************/
void Menu::doDraw(Rect pBody)
{
   /* Note: We could ignore pBody, as Menu will never have a parent. */
   Skin* skin = Controller::getSkin();
   Draw* draw = Controller::getDraw();
   Surface* surface = getWidgetRenderer()->getSurface();

   /* Finally, let's draw it */
   if(skin)
   {
      /* Draw the menu with its skin */
      skin->drawElement(surface, Skin::SKIN_TYPE_MENU, 0, 0, 
            curWidth -1, curHeight -1);
   }
   else
   {
      /* Draw its brackground */
      draw->setActiveColor(Colors::colorMenu);
      draw->doFilledRectangle(surface, 1, 1, curWidth - 2, curHeight - 2);

      /* And menu border */
      draw->setActiveColor(Colors::colorCont[2]);
      draw->doRoundedRectangle(surface, 0, 0, curWidth - 1, curHeight - 1, 
            Colors::colorCont[1]);

   }
}

/***********************************************************************
 *                             doTreat                                 *
 ***********************************************************************/
bool Menu::doTreat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY, int mrX, int mrY)
{
   if(leftButtonPressed)
   {
      if(!pressStarted)
      { 
         /* Must start the press and select current item, if any
          * available and visible. */
         pressStarted = true;
         if(isInner(mrX, mrY))
         {
            /* Try to get the item under position */
            current = getItem(mrX, mrY);
         }
         else
         {
            /* outside: no item under */
            current = NULL;
         }
      }
      Controller::setEvent(this, EVENT_MENU_PRESSING);
      return true;
   }
   else if(pressStarted)
   {
      pressStarted = false;
      if(current)
      {
         Controller::setEvent(this, EVENT_MENU_SELECTED);
      }
      else
      {
         Controller::setEvent(this, EVENT_MENU_CLOSED);
      }
      close();
      return true;
   }

   return false;
}

/***********************************************************************
 *                        doAfterChildTreat                            *
 ***********************************************************************/
void Menu::doAfterChildTreat()
{
}
