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

#include <kobold/platform.h>

#if KOBOLD_PLATFORM == KOBOLD_PLATFORM_WINDOWS
   #include <windows.h>
#else
   #include <dirent.h>
   #include <sys/types.h>
#endif

#include <kobold/log.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>

#include "fileselector.h"
#include "container.h"
#include "controller.h"

#include <assert.h>

using namespace Farso;

#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_WINDOWS
   #define FILE_SEL_SLASH '/'
#else
   #define FILE_SEL_SLASH '\\'
#endif

#define FILE_SEL_LINE_HEIGHT 21

/***********************************************************************
 *                             Constructor                             *
 ***********************************************************************/
FileSelector::FileSelector(bool load, Kobold::String dir, bool nav, 
      Widget* parent)
             :Widget(Widget::WIDGET_TYPE_FILE_SELECTOR, 0,0,0,0, parent)
{
   curDir = dir;
   filter = "";
   loading = load;
   navDirs = nav;

   /* Set dimensions */
   assert(parent != NULL);
   body.set(0, 0, parent->getBody().getWidth() - 1,
         parent->getBody().getHeight() - 1);
   setSize(body.getWidth(), body.getHeight());

   /* Create the buttons, centralized at bottom */
   Container* cont = new Container(Container::TYPE_BOTTOM_CENTERED, this);
   cancelButton = new Button(-80, 0, 70, FILE_SEL_LINE_HEIGHT, "Cancel", cont);
   acceptButton = new Button(80, 0, 70, FILE_SEL_LINE_HEIGHT, "Confirm", cont); 

   /* Calculate number of available file lines */
   numLines = calculateNumLines();
   if(numLines <= 0)
   {
      Kobold::Log::add("Warning: area for file selector is too small!");
      numLines = 1;
   }

   /* Create our scroll */
   cont = new Container(Container::TYPE_TOP_RIGHT, this);
   scrollBar = new ScrollBar(ScrollBar::TYPE_VERTICAL, 0, 
         getY() + FILE_SEL_LINE_HEIGHT, 
         parent->getBody().getHeight() - 10 - 2 * FILE_SEL_LINE_HEIGHT,
         cont);

   /* Create the grid, which will contain the labels */
   grid = new Grid(Grid::GRID_TYPE_HIGHLIGHT_FILL, this);

   createLabelsAndGridElements();

   /* Create the current file text */
   int width = getSelectorWidth();
   if(loading)
   {
      textCurFile = new Label(getX(), getY(), width - 40,  
            FILE_SEL_LINE_HEIGHT, "", this);
      textCurFile->enableBorder();
      editCurFile = NULL;
   }
   else
   {
      textCurFile = NULL;
      editCurFile = new TextEntry(getX(), getY(), width - 40,
            FILE_SEL_LINE_HEIGHT, this);
   }

   /* Create the current filter text */
   cont = new Container(Container::TYPE_TOP_RIGHT, this);
   textFilter = new Label(0, 0, 40, FILE_SEL_LINE_HEIGHT, "*", cont);

   /* Put it at initial dir */
   changeCurDir(dir);
}

/***********************************************************************
 *                              Destructor                             *
 ***********************************************************************/
FileSelector::~FileSelector()
{
}

/***********************************************************************
 *                         calculateNumLines                           *
 ***********************************************************************/
int FileSelector::calculateNumLines()
{
   return ((getParent()->getBody().getHeight() - 10) / 
         FILE_SEL_LINE_HEIGHT) - 2;
}

/***********************************************************************
 *                         getSelectorWidth                            *
 ***********************************************************************/
int FileSelector::getSelectorWidth()
{
   return getParent()->getBody().getWidth() - 24;
}

/***********************************************************************
 *                    createLabelsAndGridElements                      *
 ***********************************************************************/
void FileSelector::createLabelsAndGridElements()
{
   /* Create respective labels and grid elements to them */
   int curY = getY() + FILE_SEL_LINE_HEIGHT + 2;
   int width = getSelectorWidth();
   Label* label;
   for(int i = 0; i < numLines; i++)
   {
      label = new Label(getX(), curY, width, FILE_SEL_LINE_HEIGHT, "", grid);
      labels.push_back(label);

      grid->addElement(getX(), curY, width, FILE_SEL_LINE_HEIGHT);

      curY += FILE_SEL_LINE_HEIGHT;
   }
}

/***********************************************************************
 *                               getBody                               *
 ***********************************************************************/
Rect FileSelector::getBody()
{
   return body;
}

/***********************************************************************
 *                              setDirty                               *
 ***********************************************************************/
void FileSelector::setDirty()
{
   getParent()->setDirty();
   Widget::setDirty();
}

/***********************************************************************
 *                              setFilter                              *
 ***********************************************************************/
void FileSelector::setFilter(Kobold::String newFilter)
{
   filter = newFilter;
   textFilter->setCaption(filter);
   /* Update the current dir with the new filter */
   changeCurDir(curDir);
}

/***********************************************************************
 *                             changeCurDir                            *
 ***********************************************************************/
void FileSelector::changeCurDir(Kobold::String newDir)
{
   int j, total;

   Kobold::String aux = "";
   std::vector<Kobold::String> strings;

#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_WINDOWS
   DIR* dir = NULL;
   struct dirent* dirEnt = NULL;
#else
   WIN32_FIND_DATA dir;
   HANDLE hSearch;
   Kobold::String fullDir;
#endif

   /* Clear current displayed */
   files.clear();

   /* Change to dir */
   curDir = newDir;
   if(curDir[curDir.length()-1] != FILE_SEL_SLASH)
   {
      curDir += FILE_SEL_SLASH;
   }

#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_WINDOWS
   dir = opendir(newDir.c_str());
#else
   fullDir = curDir+"*.*";
   hSearch = FindFirstFile((const char*)fullDir.c_str(), &dir);
#endif

   total = 0;

#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_WINDOWS
   if(dir)
#else
   if(hSearch != INVALID_HANDLE_VALUE)
#endif
   {
      /* first pass to define the size */
#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_WINDOWS
   if(dir)
      for(dirEnt = readdir(dir); dirEnt != NULL; dirEnt = readdir(dir) )
      {
         total++;
      }
#else
      for(total=0; (FindNextFile(hSearch,&dir)); total++);
#endif

      /* rewind for next pass */
#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_WINDOWS
      rewinddir(dir);
#else
      hSearch = FindFirstFile((const char*)fullDir.c_str(), &dir);
#endif

      /* now save each directory/file name */
#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_WINDOWS
      for(dirEnt = readdir(dir); dirEnt != NULL; dirEnt = readdir(dir) )
      {
         Kobold::String s = dirEnt->d_name;

         /* Put a first character to group directories and files */
         if(dirEnt->d_type == DT_DIR)
         {
            s = "d" + s;
         }
         else
         {
            s = "f" + s;
         }
         strings.insert(strings.end(), s); 
      }

      /* finally, close it */
      closedir(dir);
#else
      for(; (FindNextFile(hSearch,&dir)); )
      {
         Kobold::String s = dir.cFileName;
         if(dir.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
         {
            s = "d" + s;
         }
         else
         {
            s = "f" + s;
         }
         strings.insert(strings.end(), s); 
      }
#endif
   }
   else
   {
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_ERROR, "Can't open dir '%s'",
                       newDir.c_str());
   }

   /* Sort the result */
   if(total > 0)
   {
      std::sort(strings.begin(), strings.end(), std::less<Kobold::String>());
   }

   /* Put at selectors */
   j = 0;
   for (std::vector<Kobold::String>::iterator it = strings.begin(); 
        it != strings.end(); it++)
   {
      Kobold::String str = *it;
      /* Directories */
      if(str[0] == 'd')
      {
         /* Remove the "group" char */
         str.erase(0,1);

         /* Insert at list, if directory navigations is defined */
         if(navDirs)
         {
            lastDir = j;
            files.push_back(str);
         }
      }

      /* Files */
      else
      {
         /* Remove the "group" char */
         str.erase(0,1);

         /* Verify if pass */
         if(passFilter(str))
         {
            /* Passed filter, so Insert at list */
            files.push_back(str);
         }
      }
      
      j++;
   }

   strings.clear();

   /* Set scroll bar size */
   scrollBar->setTotals(numLines, j);

   /* Set labels */
   setLabels();
   setDirty();
}

/***********************************************************************
 *                             setLabels                               *
 ***********************************************************************/
void FileSelector::setLabels()
{
   int totalFiles = files.size();
   int first = scrollBar->getCurrent();

   for(int i = 0; i < numLines; i++)
   {
      /* Define text */
      if(i + first < totalFiles)
      {
         labels[i]->setCaption(files[i + first]);
      }
      else
      {
         labels[i]->setCaption("");
      }
      /* Define color */
      if(i + first <= lastDir)
      {
         labels[i]->setFontColor(Colors::colorDirectory);
         labels[i]->setSkinType(Skin::SKIN_TYPE_DIRECTORY_LABEL);
      }
      else
      {
         labels[i]->setFontColor(Colors::colorFile);
         labels[i]->setSkinType(Skin::SKIN_TYPE_FILE_LABEL);
      }
   }
}

/***********************************************************************
 *                             passFilter                              *
 ***********************************************************************/
bool FileSelector::passFilter(Kobold::String s)
{
   Kobold::String aux = s;

   if(filter.empty())
   {
      /* No filter defined! */
      return true;
   }

   /* Get the remaining Kobold::String (with the same size of filter) */
   aux.erase(0,aux.length()-filter.length());

   /* If it is equal to filter, passed */
   return aux == filter;
}

/***********************************************************************
 *                             getFileName                             *
 ***********************************************************************/
Kobold::String FileSelector::getFilename()
{
   /* Get the current directory */
   Kobold::String fileName = curDir;

   /* Get the current file */
   if(loading)
   {
      if(textCurFile->getCaption().empty())
      {
         return "";
      }
      fileName += textCurFile->getCaption();
   }
   else
   {
      if(editCurFile->getCaption().empty())
      {
         return "";
      } 
      fileName += editCurFile->getCaption();
   }

   /* Add the filter if the file hasn't it */
   if(!passFilter(fileName))
   {
      fileName += filter;
   }

   return fileName;
}

/***********************************************************************
 *                             setFileName                             *
 ***********************************************************************/
void FileSelector::setFilename(Kobold::String fileName)
{
   Kobold::String newDir, file;

   /* get directory from it */
   newDir = fileName;
   newDir.erase(newDir.find_last_of(FILE_SEL_SLASH));

   /* change the current displayed directory */
   changeCurDir(newDir);

   /* set the file from it */
   file = fileName;
   file.erase(0, newDir.length()+1); //+1 to remove the "/"
   if(loading)
   {
      textCurFile->setCaption(file);
   }
   else
   {
      editCurFile->setCaption(file);
   }
   setDirty();
}

/***********************************************************************
 *                                draw                                 *
 ***********************************************************************/
void FileSelector::doDraw(Rect pBody)
{
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
         grid->remove(labels[labels.size() - 1]);
         labels.pop_back();
      }
      createLabelsAndGridElements();
      scrollBar->setHeight(getParent()->getBody().getHeight() - 10 - 
                           2 * FILE_SEL_LINE_HEIGHT);
      scrollBar->setTotals(numLines, files.size());
      
      /* Adjusts width of selected and pattern */
      int width = getSelectorWidth();
      if(loading)
      {
         textCurFile->setWidth(width - 40);
      }
      else
      {
         editCurFile->setWidth(width - 40);
         
      }
      /* Redefine Labels */
      setLabels();
   }

   /* Calculate selector position */
   int rx1 = pBody.getX1() + getX();
   int ry1 = pBody.getY1() + getY() + FILE_SEL_LINE_HEIGHT + 2;
   int rx2 = rx1 + getSelectorWidth();
   int ry2 = ry1 + getHeight() - (2 * FILE_SEL_LINE_HEIGHT) - 5;

   /* Draw selector area border */
   Skin* skin = Controller::getSkin();
   Draw* draw = Controller::getDraw();
   Surface* surface = getWidgetRenderer()->getSurface();

   if(skin != NULL)
   {
      skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_LEFT,
            rx1, ry1, rx2, ry2);
      skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_RIGHT,
            rx1, ry1, rx2, ry2);
      skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_TOP,
            rx1, ry1, rx2, ry2);
      skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_BOTTOM,
            rx1, ry1, rx2, ry2);
   }
   else
   {
      draw->setActiveColor(Colors::colorCont[0]);
      draw->doRectangle(surface, rx1, ry1, rx2, ry2);
   }
}

/***********************************************************************
 *                              doTreat                                *
 ***********************************************************************/
bool FileSelector::doTreat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY, int mrX, int mrY)
{
   return false;
}

/***********************************************************************
 *                       doAfterChildTreat                             *
 ***********************************************************************/
void FileSelector::doAfterChildTreat()
{
   Event event = Controller::getLastEvent();

   if(event.getType() == EVENT_BUTTON_RELEASE)
   {
      if( (event.getWidget() == acceptButton) &&
          ( ((loading) && (textCurFile->getCaption() != "")) ||
          ( ((!loading) && (editCurFile->getCaption() != "")) ) ) )
      {
         /* Accepted a selected (or edited) filename */
         Controller::setEvent(this, EVENT_FILESELECTOR_ACCEPT);
      }
      else if(event.getWidget() == cancelButton)
      {
         /* Called to cancel file selector */
         Controller::setEvent(this, EVENT_FILESELECTOR_CANCEL);
      }
   }
   else if(event.getType() == EVENT_GRID_SELECTED_ELEMENT)
   {
      if(event.getWidget() == grid)
      {
         /* Selected an element on the grid, get its index (as grid index plus
          * initial scroling bar's */
         int curIndex = grid->getCurrent()->getIndex() + 
                        scrollBar->getCurrent();

         if(curIndex >= (int) files.size())
         {
            /* Selected an inactive selector. */
            return;
         }

         Kobold::String cur = files[curIndex];

         if(curIndex <= lastDir)
         {
            /* It's a dir, change the directory to the new one */
            Kobold::String newDir = "";
            if(cur == ".")
            {
               /* The new dir is the same */
               newDir = curDir;
            }
            else if(cur == "..")
            {
               /* The new dir is without the last one, if not ../  */
               if( (curDir.length() >= 3) && 
                   (curDir[curDir.length()-1] == FILE_SEL_SLASH) &&
                   (curDir[curDir.length()-2] == '.') &&
                   (curDir[curDir.length()-3] == '.') )
               {
                  /* Continue to adding ../ */
                  newDir = curDir + Kobold::String("..") + FILE_SEL_SLASH;
               }
               else
               {
                  /* Try to remove the last directory, if any */
                  size_t pos = newDir.find_last_of(FILE_SEL_SLASH);
                  if(pos != Kobold::String::npos)
                  {
                     newDir = curDir;
                     newDir.erase(newDir.length()-1, 1); //remove the last"/"
                     newDir.erase(pos);
                  } 
                  else
                  {
                     newDir = curDir + Kobold::String("..") + FILE_SEL_SLASH;
                  }
               }
            }
            else
            {
               /* The new one is appended  */
               newDir = curDir + cur; 
            }
            changeCurDir(newDir);
            /* Change the file text if is loading (since at saving 
             * we hope to keep the one got from user) */
            if(loading)
            {
               textCurFile->setCaption("");
            }

            Controller::setEvent(this, EVENT_FILESELECTOR_SELECTED_DIRECTORY);
         }
         else
         {
            /* It's a file, change the selected file */
            if(loading)
            {
               textCurFile->setCaption(cur);
            }
            else
            {
               editCurFile->setCaption(cur);
            }
            setDirty();
            Controller::setEvent(this, EVENT_FILESELECTOR_SELECTED_FILE);
         }
      }
   }
   else if(event.getType() == EVENT_SCROLLBAR_CHANGED)
   {
      /* Must reset labels content if changed scroll bar current value */
      if(event.getWidget() == scrollBar)
      {
         setLabels();
      }
   }
}


