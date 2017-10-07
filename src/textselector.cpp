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
#include <assert.h>
using namespace Farso;

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
TextSelector::TextOption::TextOption(int x, int y, int width, int height,
      const Kobold::String& caption, int index, Widget* parent)
{
   this->label = new Label(x, y, width, height, caption, parent);
   this->label->setSkinType(Skin::SKIN_TYPE_TEXT_OPTION);
   this->label->enableBreakLineOnSpace();
   this->index = index;
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
             :Widget(WIDGET_TYPE_TEXT_SELECTOR, parent),
              options(Kobold::LIST_TYPE_ADD_AT_END)
{
   assert(parent != NULL);
   body.set(0, 0, parent->getBody().getWidth() - 1, 
         parent->getBody().getHeight() - 1);
   setSize(body.getWidth(), body.getHeight());
   selected = -1;
   over = -1;
   selectedOption = NULL;
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
void TextSelector::addOption(const Kobold::String& text, int index)
{
   Kobold::String prefix = Kobold::StringUtil::toString(
         options.getTotal() + 1) + "- ";
   if(index < 0)
   {
      index = options.getTotal();
   }
   /* We insert at the whole line, but at doDraw, we'll redefine the 
    * position according to font and skin used (allowing changing the skin
    * on the fly). */
   options.insert(new TextOption(0, 0, getWidth(), getHeight(), 
            prefix + text, index, this));
}

/***********************************************************************
 *                          getSelectedOption                          *
 ***********************************************************************/
int TextSelector::getSelectedOption()
{
   if(selectedOption)
   {
      return selectedOption->index;
   }
   return selected;
}

/***********************************************************************
 *                              unselect                               *
 ***********************************************************************/
void TextSelector::unselect()
{
   selected = -1;
   over = -1;
   if(selectedOption)
   {
      setUnselectedStyle(selectedOption->label);
      selectedOption = NULL;
   }
}

/***********************************************************************
 *                            clearOptions                             *
 ***********************************************************************/
void TextSelector::clearOptions()
{
   /* Remove all labels created for our selection */
   TextOption* opt = static_cast<TextOption*>(options.getFirst());
   for(int i = 0; i < options.getTotal(); i++)
   {
      remove(opt->label);
      opt = static_cast<TextOption*>(opt->getNext());
   }

   /* Finally, remove all elements */
   options.clear();

   /* Nullify things */
   selected = -1;
   selectedOption = NULL;
   over = -1;
}


/***********************************************************************
 *                            forceSelection                           *
 ***********************************************************************/
void TextSelector::forceSelection(int option)
{
   if((option >= 0) && (option < options.getTotal()))
   {
      selected = option;
      over = option;

      /* Define our label style */
      TextOption* opt = static_cast<TextOption*>(options.getFirst());
      for(int i = 0; i < option; i++)
      {
         opt = static_cast<TextOption*>(opt->getNext());
      }
      setSelectedStyle(opt->label);
   }
}

/***********************************************************************
 *                               doDraw                                *
 ***********************************************************************/
void TextSelector::doDraw(const Rect& pBody)
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
      int height = font->getHeight(getWidth(), opt->label->getCaption(), true);

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
   over = -1;
   selected = -1;

   /* Let's check over each label. Note that if we changed the color of
    * the label, we must set as dirty our parent, to clear the background. */
   TextOption* opt = (TextOption*) options.getFirst();
   
   for(int i = 0; i < options.getTotal(); i++)
   {
      if(opt->label->isInner(mrX, mrY))
      {
         if(selectedOption != opt)
         {
            /* Redefine as selected */
            setSelectedStyle(opt->label);
            selectedOption = opt;
         }
         over = i;
      }
      opt = (TextOption*) opt->getNext();
   }

   if((over == -1) && (selectedOption))
   {
      /* Unselect */
      setUnselectedStyle(selectedOption->label);
      selectedOption = NULL;
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
 *                         setUnSelectedStyle                          *
 ***********************************************************************/
void TextSelector::setUnselectedStyle(Label* label)
{
   Skin* skin = Controller::getSkin();
   if(skin == NULL)
   {
      label->setFontColor(Color(255,255,255,255));
   }
   else
   {
      label->setSkinType(Skin::SKIN_TYPE_TEXT_OPTION);
   }
   label->setDirty();
   getParent()->setDirty();
}

/***********************************************************************
 *                          setSelectedStyle                           *
 ***********************************************************************/
void TextSelector::setSelectedStyle(Label* label)
{
   if((selectedOption != NULL) && (selectedOption->label == label))
   {
      /* Already selected */
      return;
   }
   else if(selectedOption)
   {
      /* Unselect current selection */
      setUnselectedStyle(selectedOption->label);
      selectedOption = NULL;
   }

   Skin* skin = Controller::getSkin();
   if(skin == NULL)
   {
      label->setFontColor(Color(10,255,10,255));
   }
   else 
   {
      label->setSkinType(Skin::SKIN_TYPE_TEXT_OPTION_OVER);
   }
   label->setDirty();
   getParent()->setDirty();
}

/***********************************************************************
 *                                getBody                              *
 ***********************************************************************/
const Farso::Rect& TextSelector::getBody()
{
   return body;
}

