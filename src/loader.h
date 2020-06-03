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

#ifndef _farso_loader_h
#define _farso_loader_h

#include <kobold/kstring.h>

namespace Farso
{

class Font;
class Skin;

/*! Abstract class to control internal file loading on Farso. */
class Loader
{
   public:
      virtual ~Loader() {};
      /*! Load a font.
       * \param f pointer to the font to load.
       * \return loading result (successfull or not). */
      virtual bool loadFont(Font* f) = 0;
      /*! Load a skin.
       * \param s pointer to the Skin to load.
       * \param filename skin file to load.
       * \return loading result (successfull or not). */
      virtual bool loadSkin(Skin* s, const Kobold::String& filename) = 0;
};

/*! Default loader, loading direct from disk, and avoiding any resource 
 * managers. */
class DefaultLoader : public Loader
{
   public:
      virtual ~DefaultLoader();
      bool loadFont(Font* f) override;
      bool loadSkin(Skin* s, const Kobold::String& filename) override;
};

}


#endif

