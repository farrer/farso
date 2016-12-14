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
   this->separator = false;
   this->owner = owner;
   this->icon = NULL;
   this->pX = 0;
   this->pY = 0;

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
   this->separator = false;
   this->owner = owner;
   this->pX = 0;
   this->pY = 0;
   this->icon = new Picture(0, 0, icon, owner);
   calculateNeededSize(caption, this->icon);
   this->label = new Label(0, 0, width, height, caption, owner);
}

/***********************************************************************
 *                                MenuItem                             *
 ***********************************************************************/
Menu::MenuItem::MenuItem()
{
   this->pX = 0;
   this->pY = 0;
   this->enabled = false;
   this->visible = true;
   this->separator = true;
   this->owner = NULL;
   this->icon = NULL;
   this->label = NULL;
   calculateNeededSize("", NULL);
}

/***********************************************************************
 *                                setText                              *
 ***********************************************************************/
void Menu::MenuItem::setCaption(Kobold::String str)
{
   if(this->label)
   {
      calculateNeededSize(str, this->icon);
      this->label->setCaption(str);
   }
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
   /* Define internal */
   pX = x;
   pY = y;

   /* define icon's */
   if(this->icon)
   {
     this->icon->setPosition(x, y);
      x += this->icon->getWidth() + 4;
   }

   /* define label's */
   if(this->label)
   {
      this->label->setPosition(x, y);
   }
}

/***********************************************************************
 *                         getNeededHeight                             *
 ***********************************************************************/
int Menu::MenuItem::getNeededHeight()
{
   return height;
}

/***********************************************************************
 *                                 getX                                *
 ***********************************************************************/
int Menu::MenuItem::getX()
{
   return pX;
}

/***********************************************************************
 *                                 getX                                *
 ***********************************************************************/
int Menu::MenuItem::getY()
{
   return pY;
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
   if((!enabled) && (!separator))
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
 *                           isSeparator                               *
 ***********************************************************************/
bool Menu::MenuItem::isSeparator()
{
   return separator;
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
Menu::Menu(int minWidth)
     :Widget(Widget::WIDGET_TYPE_MENU, NULL)
{
   this->current = NULL;
   this->creating = false;
   this->pressStarted = false;
   this->hasSeparator = false;
   this->grid = new Grid(Grid::GRID_TYPE_HIGHLIGHT_FILL, this);
   this->curWidth = 0;
   this->curHeight = 0;
   this->minWidth = minWidth;
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
 *                         insertSeparator                             *
 ***********************************************************************/
Menu::MenuItem* Menu::insertSeparator()
{
   if(!creating)
   {
      Kobold::Log::add(
            "Warning: couldn't insert separator while not creating the menu.");
      return NULL;
   }
 
   /* Create and insert the separator */
   MenuItem* item = new MenuItem();
   items.insertAtEnd(item);

   this->hasSeparator = true;

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

   /* Check minimum size */
   if(width < minWidth)
   {
      curWidth = minWidth;
      width = minWidth;
   }

   /* define menu size (creating its renderer) */
   Widget::setSize(width, height);

   /* Set body */
   body.set(0, 0, width - 1, height - 1);

   hide();
   setDirty();
}

/***********************************************************************
 *                               open                                  *
 ***********************************************************************/
void Menu::open(int x, int y, Widget* caller)
{
   /* Define its caller */
   this->caller = caller;

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

         if(item->isEnabled())
         {
            grid->addElement(pX + 2, pY + 2, curWidth - 4,
                  item->getNeededHeight() - 2);
         }

         pY += item->getNeededHeight();
      }

      item = (MenuItem*) item->getNext();
   }

   /* define current height */
   curHeight = pY + 1;

   /* Check for menu inside Farso available window */
   if(x + curWidth > Controller::getWidth())
   {
      x = Controller::getWidth() - curWidth + 1;
   }
   if(y + curHeight > Controller::getHeight())
   {
      y = Controller::getHeight() - curHeight + 1;
   }

   /* Finally, set menu position and display it. */
   setPosition(x, y);
   show();

   Controller::setActiveMenu(this);
}

/***********************************************************************
 *                              close                                  *
 ***********************************************************************/
void Menu::close()
{
   hide();
   Controller::setActiveMenu(NULL);
   if((caller != NULL) && (caller->getType() == Widget::WIDGET_TYPE_BUTTON))
   {
      Button* callerButton = static_cast<Button*>(caller);
      callerButton->release();
   }
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
Farso::Rect Menu::getBody()
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

   if(hasSeparator)
   {
      /* Must draw our separators */
      MenuItem* item = (MenuItem*) items.getFirst();
      for(int i = 0; i < items.getTotal(); i++)
      {
         if(item->isSeparator())
         {
            if(skin)
            {
               skin->drawElement(surface, Skin::SKIN_TYPE_MENU_SEPARATOR, 
                     0, item->getY(), curWidth -1, 
                     item->getY() + item->getNeededHeight() - 1);
            }
            else
            {
               /* Draw at the middle of the item */
               int midY = (item->getNeededHeight() / 2) + item->getY();
               draw->setActiveColor(Colors::colorCont[1]);
               draw->doTwoColorsRectangle(surface, item->getX() + 4, midY - 1, 
                      curWidth - 5, midY, Colors::colorCont[0]);
            }
         }

         item = (MenuItem*) item->getNext();
      }

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

