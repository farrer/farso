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

#include "scrolltext.h"
#include "controller.h"
#include "container.h"
#include <kobold/log.h>
using namespace Farso;

/***********************************************************************
 *                               ScrollText                            *
 ***********************************************************************/
ScrollText::ScrollText(int x, int y, int width, int height,
                       Kobold::String defaultFont, int defaultFontSize,
                       Color defaultColor, Widget* parent)
          :Widget(Widget::WIDGET_TYPE_SCROLL_TEXT, x, y, width, height, parent),
           lines(Kobold::LIST_TYPE_ADD_AT_END)
{
   body.set(getX(), getY(), getX() + width - 1, getY() + height - 1);

   /* Create children */
   this->scrollBar = new ScrollBar(ScrollBar::TYPE_VERTICAL, 
         width - 20, 0, height, this); 

   /* Set defaults */
   this->defaultFont = defaultFont;
   this->defaultFontSize = defaultFontSize;
   this->defaultColor = defaultColor;
   this->firstDisplayedLine = NULL;
   this->firstDisplayed = 0;

   /* Define define font default height (for empy lines) */
   Font* font = FontManager::getFont(defaultFont);
   font->setSize(defaultFontSize);
   this->defaultFontHeight = font->getDefaultHeight();
}

/***********************************************************************
 *                              ~ScrollText                            *
 ***********************************************************************/
ScrollText::~ScrollText()
{
}

/***********************************************************************
 *                                 clear                               *
 ***********************************************************************/
void ScrollText::clear()
{
   lines.clearList();
   firstDisplayedLine = NULL;
   firstDisplayed = 0;
}

/***********************************************************************
 *                            createLine                               *
 ***********************************************************************/
ScrollText::TextLine* ScrollText::createLine(Kobold::String fontName,
      int fontSize, Font::Alignment alignment, Color color)
{
   TextLine* line = new TextLine(this->defaultFontHeight);
   line->setAlignment(alignment);
   lines.insert(line);
   TextSentence* sentence = new TextSentence(fontName, fontSize,
         color);
   line->insert(sentence);

   if(firstDisplayedLine == NULL)
   {
      firstDisplayedLine = line;
      firstDisplayed = 0;
   }

   return line;
}

/***********************************************************************
 *                            getSentence                              *
 ***********************************************************************/
ScrollText::TextSentence* ScrollText::getSentence(Kobold::String fontName, 
      int fontSize, Font::Alignment alignment, Color color)
{
   if(lines.getTotal() == 0)
   {
      /* Create a new line with a new sentence to write into. */
      TextLine* line = createLine(fontName, fontSize, alignment, color);
      return (TextSentence*) line->getLast();
   }

   /* Get current line and see if its last sentence is compatible */
   TextLine* line = (TextLine*) lines.getLast();
   TextSentence* sentence = (TextSentence*) line->getLast();
   if((line->getTotal() == 0) || 
      (!sentence->isCompatible(fontName, fontSize, color)))
   {
      /* No sentence or last one not compatible: must create a new, but 
       * before let's check if the alignment is compatible with the line */
      if(line->isCompatible(alignment))
      {
         /* We could use the current line */
         sentence = new TextSentence(fontName, fontSize, color);
         line->insert(sentence);
         /* Let's sure to se the alignment, as the current line
          * could be compatible is empty. */
         line->setAlignment(alignment);
      }
      else
      {
         /* Line isn't compatible, must create a new one */
         line = createLine(fontName, fontSize, alignment, color);
         sentence = (TextSentence*) line->getLast();
      }

      return sentence;
   }

   /* Last sentence from last line is compatible, must use it. */
   return sentence;
}

/***********************************************************************
 *                                 setText                             *
 ***********************************************************************/
void ScrollText::setText(Kobold::String text)
{
   clear();
   addText(text);
}

/***********************************************************************
 *                                 addText                             *
 ***********************************************************************/
void ScrollText::addText(Kobold::String text)
{
   addText(text, defaultFont, defaultFontSize, Font::TEXT_LEFT, defaultColor);
}

/***********************************************************************
 *                                 addText                             *
 ***********************************************************************/
void ScrollText::addText(Kobold::String text, Kobold::String font, int size, 
      Font::Alignment align)
{
   addText(text, font, size, align, defaultColor);
}

/***********************************************************************
 *                                 addText                             *
 ***********************************************************************/
void ScrollText::addText(Kobold::String text, Kobold::String font, int size, 
      Font::Alignment align, Color color)
{
   /* Get sentence and line */
   TextSentence* sentence = getSentence(font, size, align, color);
   TextLine* line = (TextLine*) lines.getLast();

   /* Get font to use */
   Font* f = FontManager::getFont(font);
   f->setSize(size);

   /* Add characters to the sentence until line is full */
   Kobold::String fit;
   Kobold::String wontFit = text;

   int baseWidth = getWidth() - 2 - scrollBar->getWidth() - 
      (2 * FONT_HORIZONTAL_DELTA);
   int availableWidth = baseWidth - line->getWidth();

   int fitWidth = 0;
   bool createdLine = false;
   while(!wontFit.empty())
   {
      text = wontFit;
      fitWidth = f->getWhileFits(text, fit, wontFit, availableWidth);

      availableWidth -= fitWidth;

      if(!fit.empty())
      {
         /* Add string to current sentence */
         sentence->addText(fit, fitWidth, f->getDefaultHeight());
         line->add(fitWidth, f->getDefaultHeight());
      }
      else if(createdLine)
      {
         /* Line was created but nothing fits yet: will never fits! */
         Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL, 
               "WARN: text '%s' will never fit ScrollText area at size '%d'",
               text.c_str(), size);
         return;
      }

      if(!wontFit.empty())
      {
         /* End of line reached, must create a new one. */
         line = createLine(font, size, align, color);
         sentence = (TextSentence*) line->getLast();
         createdLine = true;
         availableWidth = baseWidth;
      }
   }

   setDirty();
}

/***********************************************************************
 *                                 addText                             *
 ***********************************************************************/
void ScrollText::addText(Kobold::String text, Color color)
{
   addText(text, defaultFont, defaultFontSize, Font::TEXT_LEFT, color);
}

/***********************************************************************
 *                              addLineBreak                           *
 ***********************************************************************/
void ScrollText::addLineBreak()
{
   /* Create a new line */
   TextLine* line = new TextLine(this->defaultFontHeight);
   lines.insert(line);
   
   if(firstDisplayedLine == NULL)
   {
      firstDisplayedLine = line;
      firstDisplayed = 0;
   }
}

/***********************************************************************
 *                                getBody                              *
 ***********************************************************************/
Farso::Rect ScrollText::getBody()
{
   return body;
}

/***********************************************************************
 *                               doDraw                                *
 ***********************************************************************/
void ScrollText::doDraw(Rect pBody)
{
   Skin* skin = Controller::getSkin();
   Farso::Surface* surface = getWidgetRenderer()->getSurface();
   
   body.set(getX(), getY(), getX() + getWidth() - 1, getY() + getHeight() - 1);

   /* Define coordinates (with parent's applied) */
   int pX = getX() + pBody.getX1();
   int pY = getY() + pBody.getY1();
   int pX2 = pX + getWidth() - 1 - scrollBar->getWidth();
   int pY2 = pY + getHeight() - 1;
   Rect drawArea(pX, pY, 
                 pX + getWidth() - 1 - scrollBar->getWidth(),
                 pY + getHeight() - 1);

   if(skin != NULL)
   {
      skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_LEFT, 
            pX, pY, pX2, pY2);
      skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_RIGHT, 
            pX, pY , pX2, pY2);
      skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_TOP,
            pX, pY, pX2, pY2);
      skin->drawElement(surface, Skin::SKIN_TYPE_BORDER_BOTTOM,
            pX, pY, pX2, pY2);
   }
   else
   {
      Farso::Draw* draw = Controller::getDraw();
      /* Draw text area border */
      draw->setActiveColor(Colors::colorCont[0]);
      draw->doRectangle(surface, pX, pY, pX2, pY2);
   }

   if(firstDisplayedLine == NULL)
   {
      /* nothing to render */
      scrollBar->setTotals(0, 0);
      return;
   }

   /* Render each displayed line, starting at first displayed one */
   TextLine* line = firstDisplayedLine;

   /* Apply current height to draw position */
   pY += line->getHeight();
   pX += FONT_HORIZONTAL_DELTA;
   
   int curHeight = 0;
   int disp = 0;
   do
   {
      disp++;
      
      /* No need to call draw if empty line, just use its height increment. */
      if(!line->isEmptyLine())
      {
         line->draw(pX, pY, drawArea, surface);
      }

      /* Apply line increment to height */
      curHeight += line->getHeight();
      pY += line->getHeight();

      /* Let's draw next line */
      line = (TextLine*) line->getNext();
   } while( (line != lines.getFirst()) && 
            (curHeight + line->getHeight() <= getHeight()) );

   /* Define totals */
   scrollBar->setTotals(disp, lines.getTotal());
}

/***********************************************************************
 *                               doTreat                               *
 ***********************************************************************/
bool ScrollText::doTreat(bool leftButtonPressed, bool rightButtonPressed, 
      int mouseX, int mouseY, int mrX, int mrY)
{
   /* No events happens inside text scroller (only on its children) */
   return false;
}

/***********************************************************************
 *                              setDirty                               *
 ***********************************************************************/
void ScrollText::setDirty()
{
   if(getParent() != NULL)
   {
      getParent()->setDirty();
   }
   Widget::setDirty();
}

/***********************************************************************
 *                          doAfterChlidTreat                          *
 ***********************************************************************/
void ScrollText::doAfterChildTreat()
{
   Event event = Controller::getLastEvent();
   if((event.getType() == EVENT_SCROLLBAR_CHANGED) &&
      (event.getWidget() == scrollBar))
   {
      /* Define new initial */
      int curScroll = scrollBar->getCurrent();
      firstDisplayedLine = (TextLine*) lines.getFirst();
      for(firstDisplayed = 0; firstDisplayed < curScroll; firstDisplayed++)
      {
         firstDisplayedLine = (TextLine*) firstDisplayedLine->getNext();
      }

      setDirty();
   }
}


/***********************************************************************
 *                             TextSentence                            *
 ***********************************************************************/
ScrollText::TextSentence::TextSentence(Kobold::String fontName, int fontSize, 
      Color color)
{
   this->fontName = fontName;
   this->fontSize = fontSize;
   
   /* Is initially empty, let's define height as default's one */
   Font* font = FontManager::getFont(fontName);
   font->setSize(fontSize);
   this->height = font->getDefaultHeight();

   this->width = 0;
   this->color = color;
}

/***********************************************************************
 *                            ~TextSentence                            *
 ***********************************************************************/
ScrollText::TextSentence::~TextSentence()
{
}

/***********************************************************************
 *                             isCompatible                            *
 ***********************************************************************/
bool ScrollText::TextSentence::isCompatible(Kobold::String fontName, 
      int fontSize, Color color)
{
   return (this->fontName == fontName) && (this->fontSize == fontSize) &&
          (this->color == color);

}

/***********************************************************************
 *                               addText                               *
 ***********************************************************************/
void ScrollText::TextSentence::addText(Kobold::String text, int width, 
      int height)
{
   this->text += text;
   this->width += width;
   if(height > this->height)
   {
      this->height = height;
   }
}

/***********************************************************************
 *                              getWidth                               *
 ***********************************************************************/
int ScrollText::TextSentence::getWidth()
{
   return width;
}

/***********************************************************************
 *                               getHeight                             *
 ***********************************************************************/
int ScrollText::TextSentence::getHeight()
{
   return height;
}

/***********************************************************************
 *                                  draw                               *
 ***********************************************************************/
void ScrollText::TextSentence::draw(int x, int y, Rect area, 
      Farso::Surface* surface)
{
   if(width > 0)
   {
      Controller::getDraw()->setActiveColor(color);
      Font* f = FontManager::getFont(fontName);
      f->setSize(fontSize);
      f->setAlignment(Font::TEXT_LEFT);
      f->write(surface, x, y, area, text);
   }
}

/***********************************************************************
 *                               TextLine                              *
 ***********************************************************************/
ScrollText::TextLine::TextLine(int defaultHeight)
           :Kobold::List(Kobold::LIST_TYPE_ADD_AT_END)
{
   width = 0;
   height = defaultHeight;
}

/***********************************************************************
 *                              ~TextLine                              *
 ***********************************************************************/
ScrollText::TextLine::~TextLine()
{
}

/***********************************************************************
 *                             isEmptyLine                             *
 ***********************************************************************/
bool ScrollText::TextLine::isEmptyLine()
{
   return (getTotal() == 0) || 
          ((getTotal() == 1) && (((TextSentence*)getLast())->getWidth() == 0));
}

/***********************************************************************
 *                                 add                                 *
 ***********************************************************************/
void ScrollText::TextLine::add(int width, int height)
{
   this->width += width;
   if(height > this->height)
   {
      this->height = height;
   }
}

/***********************************************************************
 *                              getWidth                               *
 ***********************************************************************/
int ScrollText::TextLine::getWidth()
{
   return width;
}

/***********************************************************************
 *                               getHeight                             *
 ***********************************************************************/
int ScrollText::TextLine::getHeight()
{
   return height;
}

/***********************************************************************
 *                             setAlignment                            *
 ***********************************************************************/
void ScrollText::TextLine::setAlignment(Font::Alignment alignment)
{
   this->alignment = alignment;
}

/***********************************************************************
 *                           isCompatible                              *
 ***********************************************************************/
bool ScrollText::TextLine::isCompatible(Font::Alignment alignment)
{
   if(getTotal() == 0)
   {
      return true;
   }

   return this->alignment == alignment;
}


/***********************************************************************
 *                                render                               *
 ***********************************************************************/
void ScrollText::TextLine::draw(int x, int y, Rect area, 
      Farso::Surface* surface)
{
   TextSentence* sentence = (TextSentence*) getFirst();
   int posX;

   /* Let's define X position based on our line alignment */
   switch(alignment)
   {
      case Font::TEXT_CENTERED:
         posX = x + (width / 2);
      break;
      case Font::TEXT_RIGHT:
         posX = x + area.getWidth() - width;
      break;
      default:
      case Font::TEXT_LEFT:
         posX = x;
      break;
   }

   /* Let's render all line sentences */
   for(int i = 0; i < getTotal(); i++)
   {
      sentence->draw(posX, y, area, surface);
      posX += sentence->getWidth();
      sentence = (TextSentence*) sentence->getNext();
   }
}


