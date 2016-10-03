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

#include "textselector.h"
#include "controller.h"
#include "event.h"
using namespace Farso;

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
TextSelector::TextOption::TextOption(int x, int y, int width, int height,
      Kobold::String caption, Widget* parent)
{
   label = new Label(x, y, width, height, caption, parent);
   label->setSkinType(Skin::SKIN_TYPE_TEXT_OPTION);
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
TextSelector::TextOption::~TextOption()
{
   /* No need to delete label, as it will be deleted when removed
    * from Widget's list destructor. */
}

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
TextSelector::TextSelector(Widget* parent)
             :Widget(WIDGET_TYPE_TEXT_SELECTOR, 0, 0, 0, 0, parent),
              options(Kobold::LIST_TYPE_ADD_AT_END)
{
   assert(parent != NULL);
   body.set(0, 0, parent->getBody().getWidth() - 1, 
         parent->getBody().getHeight() - 1);
   setSize(body.getWidth(), body.getHeight());
   selected = -1;
   over = -1;
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
TextSelector::~TextSelector()
{
}

/***********************************************************************
 *                              addOption                              *
 ***********************************************************************/
void TextSelector::addOption(Kobold::String text)
{
   Kobold::String prefix = Kobold::StringUtil::toString(
         options.getTotal() + 1) + "- ";
   /* We insert at the whole line, but at doDraw, we'll redefine the 
    * position according to font and skin used (allowing changing the skin
    * on the fly). */
   options.insert(new TextOption(getX(), getY(), getWidth(), getHeight(), 
            prefix + text, this));
}

/***********************************************************************
 *                          getSelectedOption                          *
 ***********************************************************************/
int TextSelector::getSelectedOption()
{
   return selected;
}

/***********************************************************************
 *                               doDraw                                *
 ***********************************************************************/
void TextSelector::doDraw(Rect pBody)
{
   body.set(0, 0, getParent()->getBody().getWidth() - 1, 
         getParent()->getBody().getHeight() - 1);
   setSize(body.getWidth(), body.getHeight());

   if(options.getTotal() == 0)
   {
      return;
   }
   
   /* Redefine each option coordinates */
   TextOption* opt = (TextOption*) options.getFirst();
   
   Font* font = opt->label->getFont();
   font->setSize(opt->label->getFontSize());

   int curY = 0;
   for(int i = 0; i < options.getTotal(); i++)
   {
      /* Calculate needed height to render the entire string */
      int height = font->getHeight(getWidth(), opt->label->getCaption());

      /* Define label position and size */
      opt->label->setPosition(0, curY);
      opt->label->setSize(getWidth(), height);

      /* Let's go to next. */
      opt = (TextOption*) opt->getNext();
      curY += height;
   }
}

/***********************************************************************
 *                               doTreat                               *
 ***********************************************************************/
bool TextSelector::doTreat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY, int mrX, int mrY)
{
   Skin* skin = Controller::getSkin();
   int wasOverAt = over;
   over = -1;
   selected = -1;

   /* Let's check over each label. Note that if we changed the color of
    * the label, we must set as dirty our parent, to clear the background. */
   TextOption* opt = (TextOption*) options.getFirst();
   
   for(int i = 0; i < options.getTotal(); i++)
   {
      if(opt->label->isInner(mrX, mrY))
      {
         if(wasOverAt != i)
         {
            if(skin == NULL)
            {
               opt->label->setFontColor(Color(10,255,10,255));
            }
            else 
            {
               opt->label->setSkinType(Skin::SKIN_TYPE_TEXT_OPTION_OVER);
            }
            opt->label->setDirty();
            getParent()->setDirty();
         }
         over = i;
      }
      else
      {
         if(wasOverAt == i)
         {
            if(skin == NULL)
            {
               opt->label->setFontColor(Color(255,255,255,255));
            }
            else
            {
               opt->label->setSkinType(Skin::SKIN_TYPE_TEXT_OPTION);
            }
            opt->label->setDirty();
            getParent()->setDirty();
         }
      }
      opt = (TextOption*) opt->getNext();
   }

   return false;
}

/***********************************************************************
 *                           doAfterChildTreat                         *
 ***********************************************************************/
void TextSelector::doAfterChildTreat()
{
   Event ev = Controller::getLastEvent();
   if(ev.getType() == EVENT_LABEL_CLICKED)
   {
      /* Let's check if was a label from our options */
      TextOption* opt = (TextOption*) options.getFirst();
      for(int i = 0; i < options.getTotal(); i++)
      {
         if(ev.getWidget() == opt->label)
         {
            Controller::setEvent(this, EVENT_TEXTSELECTOR_OPTION_SELECTED);
            selected = i;
            return;
         }
         opt = (TextOption*) opt->getNext();
      }
   }
}

/***********************************************************************
 *                                getBody                              *
 ***********************************************************************/
Rect TextSelector::getBody()
{
   return body;
}

