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

#ifndef _farso_widget_json_h
#define _farso_widget_json_h

#include "farsoconfig.h"

#if FARSO_HAS_RAPIDJSON == 1

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <kobold/kstring.h>
#include <OGRE/OgreVector2.h>
#include "font.h"
#include "widget.h"
#include "grid.h"
#include "treeview.h"

namespace Farso
{
   class WidgetJsonParser
   {
      public:
         /*! Constructor */
         WidgetJsonParser();
         /*! Destructor */
         virtual ~WidgetJsonParser();

         /*! Load widgets from a JSON string.
          * \param jsonStr string with JSON widgets
          * \param listener pointer to the event listener to use for the loaded
          *        widgets defined with json "listener=true", or NULL for no 
          *        listener.
          * \param openWindows if open all windows and set their positions.
          *        If you plan to reset the position to one not defined by the
          *        JSON, you should pass false here, and call to setPosition
          *        and open the window latter.
          * \return true if all widgets could be loaded */
         bool loadFromJson(const Kobold::String& jsonStr, 
               WidgetEventListener* listener, bool openWindows);

      protected:

         /*! Function to parse an extended widget. Should be overriden on
          * applications that need to parse its own implemented widgets.
          * \param name widget 'type' name.
          * \param value with the widget to create.
          * \param parent pointer to the potential parent widget.
          * \param listener pointer to the event listener to use for the loaded
          *        widgets, or NULL for no listener.
          * \return pointer to the widget created or NULL, if none. */
         virtual Widget* parseExtendedWidget(const Kobold::String& type, 
               const rapidjson::Value& value, Widget* parent, 
               WidgetEventListener* listener, bool openWindows);

      private:
         /*! Internal font info for parse */
         class FontInfo
         {
            public:
               Kobold::String filename;
               int size;
               Font::Alignment alignment;
         };

         /*! Parse font information of defined 'name' from rapidjson value.
          * \return parsed font information. */
         FontInfo parseFontInfo(const rapidjson::Value& value, 
               const Kobold::String& name, Font::Alignment defaultAlign);

         /*! Parse an integer of defined 'name' from rapidjson value.
          * \return parsed integer or default value if undefined. */
         int parseInt(const rapidjson::Value& value, const Kobold::String& name,
               int defaultValue);

         /*! Parse a float of defined 'name' from rapidjson value.
          * \return parsed float or default value if undefined. */
         float parseFloat(const rapidjson::Value& value, 
               const Kobold::String& name, float defaultValue);

         /*! Parse a boolean of defined 'name' from rapidjson value.
          * \return parsed boolean or default value if undefined. */
         bool parseBoolean(const rapidjson::Value& value, 
               const Kobold::String& name, bool defaultValue);

         /*! Parse a string of the item defined by 'name' from 
          * rapidjson value */
         Kobold::String parseString(const rapidjson::Value& value,
               const Kobold::String& name);

         /*! Parse a Vector2 of the item defined by 'name' from 
          * rapidjson value.
          * \return parsed vector or default [0,0] */
         Ogre::Vector2 parseVector2(const rapidjson::Value& value,
               const Kobold::String& name);

         /*! Parse a rectangle for the item defined by 'name' from rapidjson
          * value.
          * \return a defined rect if defined */
         Rect parseRect(const rapidjson::Value& value,
               const Kobold::String& name);

         /*! Parse a color for the item defined by 'name' from rapidjson value.
          * \return parsed color, when defined */
         Color parseColor(const rapidjson::Value& value,
               const Kobold::String& name, bool& defined);

         /*! Parse a widget from a json value.
          * \param value of the defined 'widget'
          * \param parent pointer to a parent widget, if any.
          * \param listener pointer to listener, if any. */
         bool parseJsonWidget(const rapidjson::Value& value, 
               Widget* parent, WidgetEventListener* listener, bool openWindows);

         Widget* parseWindow(const rapidjson::Value& value, Widget* parent);
         Widget* parseButton(const rapidjson::Value& value, Widget* parent);
         Widget* parseCheckBox(const rapidjson::Value& value, Widget* parent);
         Widget* parseClickablePicture(const rapidjson::Value& value,
               Widget* parent);
         Widget* parseContainer(const rapidjson::Value& value, Widget* parent);
         Widget* parseFileSelector(const rapidjson::Value& value,
               Widget* parent);
         Widget* parseGrid(const rapidjson::Value& value, Widget* parent);
         bool parseGridItem(const rapidjson::Value& value, Grid* grid);
         Widget* parseLabel(const rapidjson::Value& value, Widget* parent);
         Widget* parseLabelledPicture(const rapidjson::Value& value,
               Widget* parent);
         Widget* parseMenu(const rapidjson::Value& value, Widget* parent);
         Widget* parsePicture(const rapidjson::Value& value, Widget* parent);
         Widget* parseProgressBar(const rapidjson::Value& value,
               Widget* parent);
         Widget* parseScrollBar(const rapidjson::Value& value, Widget* parent);
         Widget* parseScrollText(const rapidjson::Value& value,
               Widget* parent);
         Widget* parseSpin(const rapidjson::Value& value, Widget* parent);
         Widget* parseStackTab(const rapidjson::Value& value, Widget* parent,
               WidgetEventListener* listener);
         Widget* parseTextEntry(const rapidjson::Value& value, Widget* parent);
         Widget* parseTextSelector(const rapidjson::Value& value,
               Widget* parent);
         Widget* parseTreeView(const rapidjson::Value& value, Widget* parent);
         bool parseTreeElement(const rapidjson::Value& value, 
               TreeView::TreeViewElement* parent);
         bool parseChildren(const rapidjson::Value& value, Widget* parent,
               WidgetEventListener* listener);

         /* Common Widget info */
         Kobold::String id;
         Kobold::String caption;
         Ogre::Vector2 pos;
         Ogre::Vector2 size;

   };

}


#endif

#endif

