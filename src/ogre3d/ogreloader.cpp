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

#include "ogreloader.h"
#include "../font.h"
#include "../skin.h"
#include <kobold/ogre3d/ogredefparser.h>
#include <kobold/ogre3d/ogrefilereader.h>

namespace Farso
{

/****************************************************************************
 *                                loadFont                                  *
 ****************************************************************************/
bool OgreLoader::loadFont(Font* f)
{
   Kobold::OgreFileReader fileReader;
   bool res = f->load(fileReader);
   return res;
}

/****************************************************************************
 *                               loadSkin                                   *
 ****************************************************************************/
bool OgreLoader::loadSkin(Skin* s, const Kobold::String& filename)
{
   Kobold::OgreDefParser defParser;
   return s->load(filename, defParser); 
}

}

