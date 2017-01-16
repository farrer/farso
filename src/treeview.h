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

#ifndef _farso_tree_view_h
#define _farso_tree_view_h

#include <kobold/kstring.h>
#include <kobold/list.h>

#include "widget.h"
#include "grid.h"
#include "label.h"
#include "button.h"
#include "scrollbar.h"

namespace Farso
{

/*! A widget to interactively display a tree */
class TreeView: public Widget
{
   public:
      /*! A single element on a TreeView */
      class TreeViewElement : public Kobold::ListElement
      {
         public:
            /*! Constructor
             * \param owner TreeView owner of this element
             * \param caption caption to be displayed
             * \param parent pointer to its parent
             * \param data some related data to keep */
            TreeViewElement(TreeView* owner, TreeViewElement* parent, 
                  Kobold::String caption, void* data);
            /*! Destructor */
            ~TreeViewElement();
            
            /*! Add a child element */
            TreeViewElement* addChild(Kobold::String caption, void* data=NULL);

            /*! Remove a direct child element of this one
             * \note will also remove its children.
             * \param element direct child element to be removed 
             * \return if removed or not. */
            bool removeChild(TreeViewElement* element);

            /*! Increment the current displayed elements by a value 
             * \param value value to increment (negative to decrement) */
            void incCurrentDisplayed(int value);

            /*! \return current displayed elements (including itself) by 
             * this element */
            const int getCurrentDisplayed() const { return currentDisplayed; };

            /*! \return if element is expanded or collapsed */
            const bool isExpanded() const { return expanded; };

            /*! \return if element has chidren or not */
            const bool hasChild() const { return children.getTotal() != 0; };

            /*! Check if have an specific child
             * \param el pointer to the element to check if is a child (direct
             *        or indirect) of this one */
            const bool hasChild(TreeViewElement* el);

            /*! \return total children the element has */
            const int getTotalChildren() const { return children.getTotal(); };

            /*! Expand element's children display */
            void expand();
            /*! Collapse element's children display */
            void collapse();

            /*! Get the n-th child of this element, including recursive childs.
             * \note if n == 0, will return the element.
             * \note if n > currentDisplayed-1 will return NULL.
             * \param n child element counter
             * \return n-th child, itself or NULL. */
            TreeViewElement* getChild(int n);

            /*! \return the first direct-child of the element */
            TreeViewElement* getFirstDirectChild();
            /*! \return the last direct-child of the element */
            TreeViewElement* getLastDirectChild();
            /*! \return the last visible child of a element. If no child
             * is visible, return itselef */
            TreeViewElement* getLastExpandedChild();

            /*! \return pointer to its parent, if any */
            TreeViewElement* getParent() { return parent; };

            /*! \return element's caption */
            const Kobold::String getCaption() const { return caption; };

            /*! \return element's level */
            const int getLevel() const { return level; };

         private:

            /*! Increment current displayed at its parent, if any */
            void incParentCurrentDisplayed(int value);

            TreeView* owner;         /**< Owner of this element */
            TreeViewElement* parent; /**< Pointer to its parent */
            Kobold::List children;   /**< Its children list */

            bool expanded;           /**< If expanded or collapsed */

            Kobold::String caption;  /**< Its caption text to display */
            void* data;              /**< Some related data */

            int currentDisplayed;    /**< Current displayed elements */
            int level;               /**< Nested level */
      };

      /*! Constructor 
       * \param parent pointer to its parent. Mandatory.
       * \note the TreeView will take full parent's body to itself. */
      TreeView(Widget* parent);

      /*! Destructor */
      ~TreeView();

      /*! Add a root element to the list 
       * \param caption its text to be displayed
       * \param data some related data of the element
       * \return pointer to the created element. */
      TreeViewElement* addRootElement(Kobold::String caption, void* data=NULL);

      /*! Remove a root element from the TreeView
       * \note will also remove its children.
       * \param element root element to be removed
       * \return if removed or not */
      bool removeRootElement(TreeViewElement* element);

      /*! Remove an element from the tree view (root or child one).
       * \param element pointer to the element to be removed
       * \return if removed or not */
      bool remove(TreeViewElement* element);

      /*! \return current element selected on the view or NULL */
      TreeViewElement* getSelected() { return curSelected; };

      /* From Widget */
      Rect getBody();
      void setDirty();

   protected:

      /* From Widget */
      void doDraw(Rect pBody);
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY, int mrX, int mrY);
      void doAfterChildTreat();

      /*! Calculate number of lines available for tree view */
      int calculateNumLines();
      /*! \return total width avaiable for a line */
      int getFullLineWidth();
      /*! Create our labels and selectable grid elements */
      void createLabelsAndGridElements();
      /*! \return total visible elements on the tree (those expanded) */
      int getTotalVisibleElements();

      /*! Set current displayed labels (and grids)
       * \param curInitial current initial element
       * \param totalVisible total elements potentially visible (could be -
       *        and often is - greater than the current max displayed 
       *        elements)*/
      void setLabels(int curInitial, int totalVisible);

      /*! Totally reset the current initial element, ignoring its
       * current content */
      void resetCurInitialElement(int curInitial);

      /*! Check if an element is visible or not 
       * \param el pointer to the element to check
       * \return if curently visible */
      bool isVisible(TreeView::TreeViewElement* el);

      /*! Get the next visible element of an element. It can be a child,
       * if el is expanded, a 'brother', if there's one next of it, or a
       * uncle ('brother' of its parent). Or none at all! */
      TreeViewElement* getNext(TreeViewElement* el);
      /*! Same as getNext, but to get the previous element. */
      TreeViewElement* getPrevious(TreeViewElement* el);

   private:
      Kobold::List elements; /**< Current root elements */
      
      std::vector<Label*> labels; /**< Labels for each displayed line */
      std::vector<Button*> buttons; /**< Button for each lne expand/collapse */
      ScrollBar* scrollBar;       /**< Scrolling bar */
      Grid* grid;                 /**< Grid for selecting displayed elements */

      Rect body; /**< Current body */

      int numLines; /**< Number of lines available for selectors */
      TreeViewElement* curInitialElement; /**< Current initial displayed */
      int curInitialElementIndex; /**< Index of current initial displayed */
      TreeViewElement* curSelected; /**< Current selected tree element */

      std::vector<TreeViewElement*> curVisible; /**< Current visible elements*/

};

}

#endif

