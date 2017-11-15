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

#include "combobox.h"
#include "eventtype.h"
#include "event.h"
#include "controller.h"
using namespace Farso;

/************************************************************************
 *                             Constructor                              *
 ************************************************************************/
ComboBox::ComboBox(int x, int y, int width, int height, 
      std::vector<Kobold::String> options, Widget* parent)
   :Widget(Widget::WIDGET_TYPE_COMBO_BOX, x, y, width, height, parent)
{
   this->selected = NULL;
   this->body.set(getX(), getY(), getX() + width - 1, getY() + height - 1);

   this->menu = new Menu(width, this);
   this->menu->beginCreate();
   this->menu->insertItem("");
   for(std::vector<Kobold::String>::iterator it = options.begin() ; 
         it != options.end(); ++it)
   {
      this->menu->insertItem(*it);
   }
   this->menu->endCreate();

   this->button = new Button(0, 0, width, height, "", this);
   this->button->setMenu(this->menu);
   Container* cont = new Container(Container::TYPE_CENTERED_LEFT, button);
   downLabel = new Label(width-20, 0, 20, height-4, "", cont); 
   downLabel->setSkinType(Skin::SKIN_TYPE_CAPTION_DOWN);
}

/************************************************************************
 *                            Destructor                                *
 ************************************************************************/
ComboBox::~ComboBox()
{
}

/************************************************************************
 *                              setCaption                              *
 ************************************************************************/
void ComboBox::setCaption(const Kobold::String& text)
{
   selected = getSelection(text);
   if(selected)
   {
      button->setCaption(text);
   }
   else
   {
      button->setCaption("");
   }
}

/************************************************************************
 *                              getSelection                            *
 ************************************************************************/
const Menu::MenuItem* ComboBox::getSelection(const Kobold::String& text)
{
   return menu->getItem(text);
}

/************************************************************************
 *                               setSize                                *
 ************************************************************************/
void ComboBox::setSize(int width, int height)
{
   button->setSize(width, height);
}

/************************************************************************
 *                                doDraw                                *
 ************************************************************************/
void ComboBox::doDraw(const Rect& pBody)
{
   this->body.set(getX(), getY(), 
         getX() + getWidth() - 1, getY() + getHeight() - 1);

   /* Check if we need to change the size of our down label 
    * (probably due to skin changes) */
   Skin* skin = Controller::getSkin();
   if(skin)
   {
      Rect min = skin->getSkinElement(
            Skin::SKIN_TYPE_CAPTION_DOWN).getMinSize();
      if(downLabel->getHeight() != min.getHeight())
      {
         downLabel->setSize(downLabel->getWidth(), min.getHeight());
      }
   }
}

/************************************************************************
 *                                doTreat                               *
 ************************************************************************/
bool ComboBox::doTreat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY, int mrX, int mrY)
{
   /* All actions done at the children */
   return false;
}

/************************************************************************
 *                           doAfterChildTreat                          *
 ************************************************************************/
void ComboBox::doAfterChildTreat()
{
   Event event = Controller::getLastEvent();
   if((event.getType() == EVENT_MENU_SELECTED) &&
      (event.getWidget() == this->menu))
   {
      /* Define the selection */
      selected = this->menu->getCurrentItem();
      if(selected->getCaption().empty())
      {
         /* The default empty selection */
         selected = NULL;
         button->setCaption("");
      }
      else
      {
         button->setCaption(selected->getCaption());
      }
      Controller::setEvent(this, EVENT_COMBOBOX_SELECTED);
   }
}

