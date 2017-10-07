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

#include "treeview.h"
#include "container.h"
#include "controller.h"
#include <assert.h>
#include <kobold/log.h>
using namespace Farso;

#define TREE_VIEW_LINE_HEIGHT   21
#define TREE_VIEW_NESTED_WIDTH  20

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                             TreeViewElement                              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
TreeView::TreeViewElement::TreeViewElement(TreeView* owner, 
      TreeView::TreeViewElement* parent, const Kobold::String& caption, 
      void* data)
   :children(Kobold::LIST_TYPE_ADD_AT_END)
{
   this->owner = owner;
   this->parent = parent;
   this->caption = caption;
   this->data = data;
   this->expanded = false;
   this->currentDisplayed = 1; /* Itself */
   if(parent)
   {
      this->level = parent->getLevel() + 1;
   }
   else
   {
      this->level = 0;
   }
}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
TreeView::TreeViewElement::~TreeViewElement()
{
}
 
/***********************************************************************
 *                              addChild                               *
 ***********************************************************************/
TreeView::TreeViewElement* TreeView::TreeViewElement::addChild(
      const Kobold::String& caption, void* data)
{
   TreeViewElement* child = new TreeViewElement(owner, this, caption, data);

   if((!owner->ordered) || (children.getTotal() == 0))
   {
      children.insert(child);
   }
   else
   {
      /* Must find where to insert */
      bool inserted = false;
      TreeViewElement* el = static_cast<TreeViewElement*>(children.getFirst());
      for(int i = 0; i < children.getTotal(); i++)
      {
         if(caption.compare(el->getCaption()) <= 0)
         {
            inserted = true;
            children.insertBefore(child, el);
            break;
         }
         el = static_cast<TreeViewElement*>(el->getNext());
      }
      if(!inserted)
      {
         /* Greater than all current elements, insert as last */
         children.insert(child);
      }
   }

   if(expanded)
   {
      /* We have a new potentially displayed element must increment the 
       * counters and dirtify our owner for a new draw. */
      incCurrentDisplayed(1);
      owner->setDirty();
   }
   else if(children.getTotal() == 1)
   {
      /* Was without children and now have one, must redraw to show 
       * expand button */
      owner->setDirty();
   }
   return child;
}

/***********************************************************************
 *                            removeChild                              *
 ***********************************************************************/
bool TreeView::TreeViewElement::removeChild(TreeViewElement* element)
{
   if(expanded)
   {
      /* Must tell we have lesser items displayed now */
      incCurrentDisplayed(-element->getCurrentDisplayed());
      /* And, as it could be displayed, must dirtify our owner */
      owner->setDirty();
   }
   else if(children.getTotal() == 1)
   {
      /* After remove will be without children, must redraw to hide
       * the expand button. */
      owner->setDirty();
   }

   if(children.remove(element))
   {
      owner->resetCurInitialElement(owner->curInitialElementIndex);
      return true;
   }
   return false;
}

/***********************************************************************
 *                        incCurrentDisplayed                          *
 ***********************************************************************/
void TreeView::TreeViewElement::incCurrentDisplayed(int value)
{
   /* This could be visible at the expanded parent,
    * must increment parent's counter too. */
   currentDisplayed += value;
   incParentCurrentDisplayed(value);
}

/***********************************************************************
 *                     incParentCurrentDisplayed                       *
 ***********************************************************************/
void TreeView::TreeViewElement::incParentCurrentDisplayed(int value)
{
   if((parent) && (parent->isExpanded()))
   {
      parent->incCurrentDisplayed(value);
   }
}

/***********************************************************************
 *                               expand                                *
 ***********************************************************************/
void TreeView::TreeViewElement::expand()
{
   if(!expanded)
   {
      bool wasVisible = owner->isVisible(this);
      expanded = true;
      currentDisplayed += children.getTotal();
      /* Increment parent current displayed, if any and is expanded. */
      incParentCurrentDisplayed(children.getTotal());
      owner->setDirty();
      if(!wasVisible)
      {
          /* As could be before the current initial, we must
           * reset the initial element */
          owner->resetCurInitialElement(owner->scrollBar->getCurrent());
      }
   }
}

/***********************************************************************
 *                               collapse                              *
 ***********************************************************************/
void TreeView::TreeViewElement::collapse()
{
   if(expanded)
   {
      bool wasVisible = owner->isVisible(this);
      expanded = false;
      /* Decrement total displayed, keeping only the node itself */
      int totalDisplayedChildren = currentDisplayed - 1;
      currentDisplayed = 1;
      incParentCurrentDisplayed(-totalDisplayedChildren);
      owner->setDirty();
      if(!wasVisible)
      {
          /* As could be before the current initial, we must
           * reset the initial element */
          owner->resetCurInitialElement(owner->scrollBar->getCurrent());
      }
   }
}

/***********************************************************************
 *                          getFirstDirectChild                        *
 ***********************************************************************/
TreeView::TreeViewElement* TreeView::TreeViewElement::getFirstDirectChild()
{
   assert(children.getTotal() > 0);
   return static_cast<TreeView::TreeViewElement*>(children.getFirst());
}

/***********************************************************************
 *                           getLastDirectChild                        *
 ***********************************************************************/
TreeView::TreeViewElement* TreeView::TreeViewElement::getLastDirectChild()
{
   assert(children.getTotal() > 0);
   return static_cast<TreeView::TreeViewElement*>(children.getLast());
}

/***********************************************************************
 *                               getChild                              *
 ***********************************************************************/
TreeView::TreeViewElement* TreeView::TreeViewElement::getChild(int n)
{
   if(n == 0)
   {
      return this;
   }
   if(n > currentDisplayed - 1)
   {
      return NULL;
   }

   /* Let's search through the tree */
   int curEl = 0;
   TreeViewElement* el = static_cast<TreeViewElement*>(children.getFirst());
   for(int i = 0; i < children.getTotal(); i++)
   {
      if(el->getCurrentDisplayed() + curEl >= n)
      {
         /* Must be within the child */
         return el->getChild(n - curEl - 1);
      }
      /* Let's check next child */
      curEl += el->getCurrentDisplayed();
      el = static_cast<TreeViewElement*>(el->getNext());
   }

   /* Shouldn't happen */
   assert(false);
   return NULL;
}

/***********************************************************************
 *                         getLastExpandedChild                        *
 ***********************************************************************/
TreeView::TreeViewElement* TreeView::TreeViewElement::getLastExpandedChild()
{
   if((isExpanded()) && (hasChild()))
   {
      /* Must get it's last elements last expanded child */
      TreeViewElement* child = getLastDirectChild();
      while((child->isExpanded()) && (child->hasChild()))
      {
         child = child->getLastDirectChild();
      }

      return child;
   }
   else
   {
      /* Not expanded, the last expanded is itself */
      return this;
   }
}

/***********************************************************************
 *                          getChildWithData                           *
 ***********************************************************************/
TreeView::TreeViewElement* TreeView::TreeViewElement::getChildWithData(
      void* data)
{
   TreeViewElement* el = static_cast<TreeViewElement*>(children.getFirst());
   for(int i = 0; i < children.getTotal(); i++)
   {
      /* Check if child has the data */
      if(el->getData() == data)
      {
         return el;
      }
      else
      {
         /* Check if grand child has the data */
         TreeViewElement* child = el->getChildWithData(data);
         if(child)
         {
            return child;
         }
      }

      /* Neither, check next child */
      el = static_cast<TreeViewElement*>(el->getNext());
   }

   return NULL;
}

/***********************************************************************
 *                               hasChild                              *
 ***********************************************************************/
const bool TreeView::TreeViewElement::hasChild(
      TreeView::TreeViewElement* el) 
{
   TreeViewElement* cur = static_cast<TreeViewElement*>(children.getFirst());
   for(int i = 0; i < children.getTotal(); i++)
   {
      /* It's the element? */
      if(cur == el)
      {
         return true;
      }
      /* have it as a child? */
      else if(cur->hasChild(el))
      {
         return true;
      }
      /* none to both, must check next */
      cur = static_cast<TreeViewElement*>(cur->getNext());
   }

   return false;
}

//////////////////////////////////////////////////////////////////////////////
//                                                                          //
//                                TreeView                                  //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
TreeView::TreeView(bool ordered, Widget* parent)
         :Widget(Widget::WIDGET_TYPE_TREE_VIEW, parent),
          elements(Kobold::LIST_TYPE_ADD_AT_END)
{
   assert(parent != NULL);

   this->ordered = ordered;
   curInitialElement = NULL;
   curInitialElementIndex = -1;
   curSelected = NULL;
   
   /* Set dimensions */
   body.set(0, 0, parent->getBody().getWidth() - 1,
         parent->getBody().getHeight() - 1);
   setSize(body.getWidth(), body.getHeight());

   /* Calculate number of available file lines */
   numLines = calculateNumLines();
   if(numLines <= 0)
   {
      Kobold::Log::add("Warning: area for TreeView is too small!");
      numLines = 1;
   }

   /* Create our scroll */
   Container* cont = new Container(Container::TYPE_TOP_RIGHT, this);
   scrollBar = new ScrollBar(ScrollBar::TYPE_VERTICAL, 0, 
         getY(), parent->getBody().getHeight(), cont);

   /* Create the grid, which will contain the labels */
   grid = new Grid(Grid::GRID_TYPE_HIGHLIGHT_FILL, this);

   createLabelsAndGridElements();

}

/***********************************************************************
 *                             Destructor                              *
 ***********************************************************************/
TreeView::~TreeView()
{
}

/***********************************************************************
 *                              setDirty                               *
 ***********************************************************************/
void TreeView::setDirty()
{
   setDirtyWithParent();
}

/***********************************************************************
 *                              getBody                                *
 ***********************************************************************/
const Rect& TreeView::getBody()
{
   return body;
}

/***********************************************************************
 *                           addRootElement                            *
 ***********************************************************************/
TreeView::TreeViewElement* TreeView::addRootElement(
      const Kobold::String& caption, void* data)
{
   TreeViewElement* root = new TreeViewElement(this, NULL, caption, data);

   if((!ordered) || (elements.getTotal() == 0))
   {
      /* Just insert as last */
      elements.insert(root);
   }
   else
   {
      /* Must find where to insert */
      bool inserted = false;
      TreeViewElement* el = static_cast<TreeViewElement*>(elements.getFirst());
      for(int i = 0; i < elements.getTotal(); i++)
      {
         if(caption.compare(el->getCaption()) <= 0)
         {
            inserted = true;
            elements.insertBefore(root, el);
            break;
         }
         el = static_cast<TreeViewElement*>(el->getNext());
      }
      if(!inserted)
      {
         /* Greater than all current elements, insert as last */
         elements.insert(root);
      }
   }
   setDirty();

   if(curInitialElement == NULL)
   {
      curInitialElement = root;
      curInitialElementIndex = 0;
   }

   return root;
}

/***********************************************************************
 *                         removeRootElement                           *
 ***********************************************************************/
bool TreeView::removeRootElement(TreeView::TreeViewElement* element)
{
   setDirty();

   /* Remove the element, reseting our current initial.
    * Note: as we can remove any element, even if not visible,
    * it's necessary (and we should not rely on our current pointer */
   if(elements.remove(element))
   {
      resetCurInitialElement(scrollBar->getCurrent());
      return true;
   }

   return false;
}
 
/***********************************************************************
 *                               remove                                *
 ***********************************************************************/
bool TreeView::remove(TreeViewElement* element)
{
   if(element->getParent())
   {
      return element->getParent()->removeChild(element);
   }

   return removeRootElement(element);
}

/***********************************************************************
 *                             unselect                                *
 ***********************************************************************/
void TreeView::unselect()
{
   if(curSelected)
   {
      curSelected = NULL;
      setDirty();
   }
}

/***********************************************************************
 *                         selectNodeByData                            *
 ***********************************************************************/
void TreeView::selectNodeByData(void* data)
{
   TreeViewElement* el = getNodeByData(data);
   if(el)
   {
      curSelected = el;
      setDirty();
   }
}

/***********************************************************************
 *                           getNodeByData                             *
 ***********************************************************************/
TreeView::TreeViewElement* TreeView::getNodeByData(void* data)
{
   TreeViewElement* el = static_cast<TreeViewElement*>(elements.getFirst());
   for(int i = 0; i < elements.getTotal(); i++)
   {
      /* Check if is itself the owner */
      if(el->getData() == data)
      {
         return el;
      }
      else
      {
         /* Check if any child has the data */
         TreeViewElement* child = el->getChildWithData(data);
         if(child)
         {
            return child;
         }
      }

      /* Neither, must check next root element */
      el = static_cast<TreeViewElement*>(el->getNext());
   }

   return NULL;
}

/***********************************************************************
 *                              doDraw                                 *
 ***********************************************************************/
void TreeView::doDraw(const Rect& pBody)
{
   int totalVisibleElements = getTotalVisibleElements();

   Rect parentBody = getParent()->getBody();
   if((parentBody.getWidth() != body.getWidth()) ||
      (parentBody.getHeight() != body.getHeight()))
   {
      /* Parent's body changed, must redefine our component's size */
      body.set(0, 0, parentBody.getWidth() - 1, parentBody.getHeight() - 1);
      setSize(body.getWidth(), body.getHeight());

      numLines = calculateNumLines();
      /* Must delete and recreate our grid elements and labels */
      grid->clearElements();
      while(labels.size() > 0)
      {
         /* Remove the memory of label and button (remember that they are 
          * children of the grid) */
         grid->remove(labels[labels.size() - 1]);
         grid->remove(buttons[buttons.size() - 1]);
         labels.pop_back();
         buttons.pop_back();
      }
      createLabelsAndGridElements();
      scrollBar->setHeight(getParent()->getBody().getHeight());
   }

   /* Redefine our current labels and scroll bar position */
   setLabels(scrollBar->getCurrent(), totalVisibleElements);
   scrollBar->setTotals(numLines, totalVisibleElements, 
         curInitialElementIndex);
}

/***********************************************************************
 *                             setLabels                               *
 ***********************************************************************/
void TreeView::setLabels(int curInitial, int totalVisible)
{
   /* Define our label colors */
   Farso::Color selLabelColor = Colors::colorSelText;
   Farso::Color normalLabelColor = Colors::colorText;
   Farso::Skin* skin = Farso::Controller::getSkin();
   if(skin)
   {
      selLabelColor = skin->getSkinElement(
            Skin::SKIN_TYPE_TEXT_OPTION_OVER).getFontColor();
      normalLabelColor = skin->getSkinElement(
            Skin::SKIN_TYPE_LABEL).getFontColor();
   }


   /* Check if we need to adjust cur initial */
   if((curInitial > 0) && (totalVisible > 0) && 
      (curInitial + numLines > totalVisible))
   {
      curInitial = totalVisible - numLines;
      if(curInitial < 0)
      {
         curInitial = 0;
      }
   }

   if(curInitialElement)
   {
      /* Check if we need to update the curInitialElement pointer */
      if(curInitial != curInitialElementIndex)
      {
         if(curInitial > curInitialElementIndex)
         {
            /* Must get next */
            while(curInitial != curInitialElementIndex)
            {
               curInitialElement = getNext(curInitialElement);
               curInitialElementIndex++;
            }
         }
         else
         {
            /* Must get previous */
            while(curInitial != curInitialElementIndex)
            {
               curInitialElement = getPrevious(curInitialElement);
               curInitialElementIndex--;
            }
         }
      }
   }

   int curY = getY();
   int width = getFullLineWidth();
   curVisible.clear();

   TreeViewElement* cur = curInitialElement;
   Grid::GridElement* gridElement = grid->getByIndex(0);
   for(int i = 0; i < numLines; i++)
   {
      /* Define text */
      if(i + curInitial < totalVisible)
      {
         /* Add element to current visible vector */
         curVisible.push_back(cur);

         /* Set label caption and color */
         labels[i]->setCaption(cur->getCaption());
         if(cur == curSelected)
         {
            labels[i]->setFontColor(selLabelColor);
         }
         else
         {
            labels[i]->setFontColor(normalLabelColor);
         }

         /* Define its grid and button */
         gridElement->enable();
         if(cur->hasChild())
         {
            buttons[i]->show();
            if(cur->isExpanded())
            {
               buttons[i]->setCaption(BUTTON_SPECIAL_CAPTION_DOWN);
            }
            else
            {
               buttons[i]->setCaption(BUTTON_SPECIAL_CAPTION_RIGHT);
            }
         }
         else
         {
            buttons[i]->hide();
         }

         /* Set positions, based on its level */
         int nestedWidth = (cur->getLevel() + 1) * TREE_VIEW_NESTED_WIDTH;
         labels[i]->setPosition(getX() + nestedWidth + 1, curY);
         labels[i]->setSize(width - (nestedWidth + 1), TREE_VIEW_LINE_HEIGHT);
         buttons[i]->setPosition(getX() + 
               cur->getLevel() * TREE_VIEW_NESTED_WIDTH, curY);
         gridElement->set(getX() + nestedWidth + 1, curY, 
               width - (nestedWidth + 1), TREE_VIEW_LINE_HEIGHT);

         /* Get next element to display */
         cur = getNext(cur);
         curY += TREE_VIEW_LINE_HEIGHT;
      }
      else
      {
         labels[i]->setCaption("");
         gridElement->disable();
         buttons[i]->hide();
      }
      gridElement = static_cast<Grid::GridElement*>(gridElement->getNext());
   }
}

/***********************************************************************
 *                       resetCurInitialElement                        *
 ***********************************************************************/
void TreeView::resetCurInitialElement(int curInitial)
{
   /* Let's reset our selected */
   curSelected = NULL;
   curInitialElement = NULL;

   /* Get to the first displayed element */
   int curEl = 0;
   TreeViewElement* el = static_cast<TreeViewElement*>(elements.getFirst());
   for(int i = 0; i < elements.getTotal(); i++)
   {
      if(el->getCurrentDisplayed() + curEl == curInitial + 1)
      {
         /* Current initial element is the element itself. */
         curInitialElement = el;
         break;
      }
      else if(el->getCurrentDisplayed() + curEl >= curInitial + 1)
      {
         /* The first initially displayed element is a child of the current */
         curInitialElement = el->getChild(curInitial - curEl);
         break;
      }

      /* Not at the current, must count it and check if at next one */
      curEl += el->getCurrentDisplayed(); 
      el = static_cast<TreeViewElement*>(el->getNext());
   }

   if(curInitialElement)
   {
      /* Set index value */
      curInitialElementIndex = curInitial;
   }
   else
   {
      /* not found, must set as first. */
      curInitialElement = static_cast<TreeViewElement*>(elements.getFirst());
      curInitialElementIndex = 0;
   }
   scrollBar->setCurrent(curInitialElementIndex);
}

/***********************************************************************
 *                              getNext                                *
 ***********************************************************************/
TreeView::TreeViewElement* TreeView::getNext(TreeViewElement* el)
{
   if((el->isExpanded()) && (el->hasChild()))
   {
      /* Must get its first child, if any */
      return el->getFirstDirectChild();
   }

   /* Must get its brother element, if any */
   TreeViewElement* parent = el->getParent();
   TreeViewElement* nextBrother = static_cast<TreeViewElement*>(el->getNext());
   while(parent)
   {
      if(nextBrother != parent->getFirstDirectChild())
      {
         return nextBrother;
      }
      /* We are the last one (our brother is the first or ourselves),
       * must try again with our parent's bother. */
      nextBrother = static_cast<TreeViewElement*>(parent->getNext());
      parent = parent->getParent();
   }

   /* Without parents. Our last hope is our root's next */
   if(nextBrother != elements.getFirst())
   {
      return nextBrother;
   }

   /* No next element */
   return NULL;
}

/***********************************************************************
 *                            getPrevious                              *
 ***********************************************************************/
TreeView::TreeViewElement* TreeView::getPrevious(TreeViewElement* el)
{
   TreeViewElement* parent = el->getParent();
   TreeViewElement* brother = static_cast<TreeViewElement*>(el->getPrevious());
   if(parent)
   {
      /* Check if the previous brother isn't the last (cycle) */
      if(brother != parent->getLastDirectChild())
      {
         /* Previous brother is a valid one, let's get its deepiest child */
         return brother->getLastExpandedChild();
      }

      /* We are the first element (thus, no previous), So our previous must be
       * our parent */
      return parent;
   }
   else
   {
      /* Should never be called lesser than minimun elements, assert it. */
      assert(brother != elements.getLast());

      /* No parent, our previous element must be our brother's (or its last 
       * child, if expanded) */
      return brother->getLastExpandedChild();
   }
}

/***********************************************************************
 *                              doTreat                                *
 ***********************************************************************/
bool TreeView::doTreat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY, int mrX, int mrY)
{
   /* No direct treat: all done in child widgets */
   return false;
}

/***********************************************************************
 *                         doAfterChildTreat                           *
 ***********************************************************************/
void TreeView::doAfterChildTreat()
{
   Event event = Controller::getLastEvent();

   /* Check collapse/expand button presses */
   if(event.getType() == EVENT_BUTTON_RELEASE)
   {
      for(unsigned int i = 0; i < buttons.size(); i++)
      {
         if(event.getWidget() == buttons[i])
         {
            /* Should collapse or expand */
            if(curVisible[i]->isExpanded())
            {
               curVisible[i]->collapse();
               Controller::setEvent(this, EVENT_TREEVIEW_COLLAPSED);
            }
            else
            {
               curVisible[i]->expand();
               Controller::setEvent(this, EVENT_TREEVIEW_EXPANDED);
            }
         }
      }
   }
   /* Check element's selection */
   else if(event.getType() == EVENT_GRID_SELECTED_ELEMENT)
   {
      if(event.getWidget() == grid)
      {
         TreeViewElement* sel = curVisible[grid->getCurrent()->getIndex()];
         if(curSelected != sel)
         {
            curSelected = sel;
            setDirty();
            Controller::setEvent(this, EVENT_TREEVIEW_SELECTED);
         }
      }
   }
   else if(event.getType() == EVENT_SCROLLBAR_CHANGED)
   {
      /* Must reset labels content if changed scroll bar current value */
      if(event.getWidget() == scrollBar)
      {
         setDirty();
      }
   }
}

/***********************************************************************
 *                         calculateNumLines                           *
 ***********************************************************************/
int TreeView::calculateNumLines()
{
   return ((getParent()->getBody().getHeight() - 10) / TREE_VIEW_LINE_HEIGHT);
}

/***********************************************************************
 *                         getFullLineWidth                            *
 ***********************************************************************/
int TreeView::getFullLineWidth()
{
   return getParent()->getBody().getWidth() - 24;
}

/***********************************************************************
 *                     getTotalVisibleElements                         *
 ***********************************************************************/
int TreeView::getTotalVisibleElements()
{
   /* Note: it will get the total of 'potential' visible elements,
    * not the actually visible ones (which are in the curVisible vector) */
   int total = 0;
   TreeViewElement* el = static_cast<TreeViewElement*>(elements.getFirst());
   for(int i = 0; i < elements.getTotal(); i++)
   {
      total += el->getCurrentDisplayed();
      el = static_cast<TreeViewElement*>(el->getNext());
   }

   return total;
}

/***********************************************************************
 *                            isVisible                                *
 ***********************************************************************/
bool TreeView::isVisible(TreeView::TreeViewElement* el)
{
   for(unsigned int i = 0; i < curVisible.size(); i++)
   {
      if(curVisible[i] == el)
      {
         return true;
      }
   }

   return false;
}

/***********************************************************************
 *                    createLabelsAndGridElements                      *
 ***********************************************************************/
void TreeView::createLabelsAndGridElements()
{
   /* Create respective labels and grid elements to them */
   int curY = getY();
   int width = getFullLineWidth();
   Label* label;
   Button* button;
   for(int i = 0; i < numLines; i++)
   {
      label = new Label(getX() + TREE_VIEW_NESTED_WIDTH + 1, curY, 
            width - TREE_VIEW_NESTED_WIDTH, TREE_VIEW_LINE_HEIGHT, "", grid);
      labels.push_back(label);
      button = new Button(getX(), curY, TREE_VIEW_NESTED_WIDTH, 
            TREE_VIEW_LINE_HEIGHT, "", grid);
      button->setWithoutDecorations();
      buttons.push_back(button);

      grid->addElement(getX() + TREE_VIEW_NESTED_WIDTH + 1, curY, 
            width - TREE_VIEW_NESTED_WIDTH, TREE_VIEW_LINE_HEIGHT);

      curY += TREE_VIEW_LINE_HEIGHT;
   }
}

