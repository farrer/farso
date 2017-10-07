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

#ifndef _farso_file_selector_h
#define _farso_file_selector_h

#include <kobold/platform.h>

#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID && \
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS

#include <kobold/kstring.h>
#include "widget.h"

#include "button.h"
#include "grid.h"
#include "label.h"
#include "scrollbar.h"
#include "textentry.h"

#include <vector>

//TODO: check this Widget on Android and iOS. Probably, will need to force,
//      on these systems, to be not navigatable (and also some adjust to
//      get the file list on android APK).

namespace Farso
{

/*! Implementation of a file selector / navigator widget (very usefull for
 *  load / save dialogs). */
class FileSelector: public Widget
{
   public:
      /*! Constructor
       * \param load true if the selector is for load a file, false if it
       *             is for saving a file. 
       * \param dir directory to display
       * \param nav if will show directories to navigate or not 
       * \param parent pointer to parent. Mandatory. The File Selector will
       *        use full parent's body coordinates. */
      FileSelector(bool load, const Kobold::String& dir, bool nav, 
                   Widget* parent);

      /*! Destructor */
      ~FileSelector();

      /*! Get the current selected filename */
      Kobold::String getFilename() const;
 
      /*! Set the current file and dir to the one defined by filename
       * \param filename -> full filename to set new file and dir. */
      void setFilename(const Kobold::String& filename);

      /*! Set the current filter
       * \param newFilter -> new filter to set */
      void setFilter(const Kobold::String& newFilter);
      
      /* From Widget */
      const Rect& getBody();
      void setDirty();

      /*! Check if selector if of 'load' type 
       * \return true if load type, false if save */
      const bool isLoadType() const;

   protected:

      /* From Widget */
      void doDraw(const Rect& pBody);
      bool doTreat(bool leftButtonPressed, bool rightButtonPressed, 
            int mouseX, int mouseY, int mrX, int mrY);
      void doAfterChildTreat();

   private:
      /*! Change the current displayed dir
       * \param newDir -> dir to change the display to */
      void changeCurDir(const Kobold::String& newDir);

      /*! Set labels to current visible files */
      void setLabels();

      /*! Verify if the string pass the current filter
       * \param s -> string to verify
       * \return -> true if passed*/
      const bool passFilter(const Kobold::String& s) const;
      
      /*! \return number of lines available to display files and dirs */
      int calculateNumLines();
      /*! \return width for each selector */
      int getSelectorWidth();

      /*! Create needed labels and grid elements */
      void createLabelsAndGridElements();

      Button* cancelButton;    /**< The cancel Button */
      Button* acceptButton;    /**< The accept Button */
      Label* textCurFile;      /**< The current file display (for load) */
      TextEntry* editCurFile;  /**< The current file edit (for save) */
      Label* textFilter;       /**< The current filter display */

      std::vector<Label*> labels; /**< Labels for each file line */
      std::vector<Kobold::String> files; /**< Current files to display */
      ScrollBar* scrollBar;       /**< Scrolling bar */
      Grid* grid;                 /**< Grid for selecting displayed files */

      bool navDirs;            /**< True if will diplay (and navigate) dirs */
      bool loading;            /**< If selector is a loading one, 
                                    or a saving one */
      
      Kobold::String curDir;   /**< Current Opened directory */

      Kobold::String filter;   /**< Files Filter */

      Rect body; /**< Current body */

      int lastDir;             /**< Last element on selector that is a 
                                    directory (this used to separate 
                                    strings directories of  strings files)*/
      int numLines; /**< Number of lines available for selectors */

};

}

#endif

#endif

