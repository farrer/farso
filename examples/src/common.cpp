
#include "common.h"
#include <kobold/log.h>
#include <kobold/platform.h>
using namespace FarsoExample;

/************************************************************************
 *                              Constructor                             *
 ************************************************************************/
Example::Example()
{
}

/************************************************************************
 *                               Destructor                             *
 ************************************************************************/
Example::~Example()
{
   /* When done running, we must finish Farso. Note that it will delete all
    * pointers accessible by the controller. */
   Farso::Controller::finish();
}

/************************************************************************
 *                                  init                                *
 ************************************************************************/
void Example::init(Farso::RendererType rendererType)
{
   /* Init Farso, with Ogre3d renderer */
   Farso::Controller::init(rendererType, EXAMPLE_WINDOW_WIDTH, 
         EXAMPLE_WINDOW_HEIGHT, "data/gui/");

   /* Set a default font (this is only needed when not using a skin). */
   Farso::FontManager::setDefaultFont("fonts/LiberationSans-Regular.ttf");

   /* Let's create our windows. Obviously, widgets could be created
    * any time, and not just on init. */
   createDialogWindow();
   createWindowWithStack();
   createOtherWindow();
}

/************************************************************************
 *                              shouldQuit                              *
 ************************************************************************/
bool Example::shouldQuit()
{
#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS &&\
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID
   /* Quit on ESC press. */
   return Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_ESCAPE);
#endif
   return false;
}

/************************************************************************
 *                          createDialogWindow                          *
 ************************************************************************/
void Example::createDialogWindow()
{
   /* Create the window. Not that any widget without parents is automatically
    * added as a 'root' widget to the controller. */
   Farso::Window* window = new Farso::Window(450, 200, "Window 2");

   /* Define a position where it will be when opened */
   window->getWidgetRenderer()->setPosition(0, 200);

   /* Create a text selector with some options to it */
   Farso::TextSelector* sel = new Farso::TextSelector(window);
   sel->addOption("First option");
   sel->addOption("Another option, with some ammount of text to break the whole line and test what will be with the calculation of height for this widget.");
   sel->addOption("Even another option");

   window->open();
}

/************************************************************************
 *                          createWindowWithStack                       *
 ************************************************************************/
void Example::createWindowWithStack()
{
   /* Create the window and set its position */
   Farso::Window* window = new Farso::Window(450, 400, "Window 3");
   window->getWidgetRenderer()->setPosition(300, 200);
   
   /* Add a stack tab to it */
   Farso::StackTab* tab = new Farso::StackTab(window);

   /* Add first option to the stack tab and add some children widgets to it */
   Farso::Container* tabCont = tab->insertTab("Option 1");
   (new Farso::ScrollBar(Farso::ScrollBar::TYPE_VERTICAL, 0, 0, 150, 
                         tabCont))->setTotals(10, 100);
   (new Farso::ScrollBar(Farso::ScrollBar::TYPE_HORIZONTAL, 0, 160, 150, 
                         tabCont))->setTotals(10, 5);

   /* Add another option to the stack tab and some children */
   tabCont = tab->insertTab("Option 2");
   new Farso::Button(0, 2, 80, 21, "Button", tabCont);
   (new Farso::TextEntry(0, 24, 100, 21, tabCont))->setCaption("text");
   Farso::TextEntry* textEntry = new Farso::TextEntry(0, 46, 100, 21, tabCont);
   textEntry->setCaption("text disabled");

   /* Disable the last TextEntry */
   textEntry->disable();

   /* Open the window */
   window->open();
}

/************************************************************************
 *                        createOtherWindow                             *
 ************************************************************************/
void Example::createOtherWindow()
{
   /* Create the window and add some widgets to it */
   Farso::Window* window = new Farso::Window(300, 400, "Window");
   
   new Farso::Button(0, 0, 100, 21, "Rectangular", window);
   (new Farso::Button(0, 22, 100, 21, "Oval", window))->setRoundedEdges();
   (new Farso::Button(0, 43, 100, 21, "Disabled", window))->disable();
   new Farso::Label(10, 66, 100, 21, "Label", window);
   new Farso::CheckBox(0, 88, 100, "Check box", true, window);
   (new Farso::CheckBox(0, 110, 100, "Disabled", false, window))->disable();
   (new Farso::CheckBox(0, 132, 100, "Checked", true, window))->disable();

   if(Farso::Controller::getRendererType() == Farso::RENDERER_TYPE_OGRE3D)
   {
      new Farso::Picture(120, 0, "cursor/talk.png", window);
   }
   else
   {
      new Farso::Picture(120, 0, "data/gui/cursor/talk.png", window);
   }

   /* We can use containers to set as reference coordinates to children widgets,
    * allowing us to align them to the window without any explicit maths */
   Farso::Container* cont = new Farso::Container(
         Farso::Container::TYPE_BOTTOM_RIGHT, window);
   new Farso::Button(0, 2, 80, 21, "Right", cont);
   cont = new Farso::Container(
         Farso::Container::TYPE_BOTTOM_CENTERED, window);
   new Farso::Button(0, 2, 80, 21, "Center", cont);
   (new Farso::ProgressBar(0, 40, 200, 16, cont))->setValue(60);
   cont = new Farso::Container(
         Farso::Container::TYPE_BOTTOM_LEFT, window);
   new Farso::Button(0, 2, 80, 21, "Left", cont);

   /* Open the window and set a target position to it */
   window->open();
   window->getWidgetRenderer()->setTargetPosition(400, 100);
}

/************************************************************************
 *                                  step                                *
 ************************************************************************/
void Example::step(bool leftButtonPressed, bool rightButtonPressed,
      int mouseX, int mouseY)
{
   /* Let's verify if received any event from Farso */
   if(Farso::Controller::verifyEvents(leftButtonPressed, rightButtonPressed, 
            mouseX, mouseY))
   {
      /* Got an event, we musrt treat it. For this example, just echoing
       * the event code to log */
      Farso::Event event = Farso::Controller::getLastEvent();
      Kobold::Log::add(Kobold::Log::LOG_LEVEL_NORMAL, "Event got: %d", 
            event.getType());
   }

   /* Let's treat some dynamic skin change via keys C, M and W. */
#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID &&\
    KOBOLD_PLATFROM != KOBOLD_PLATFORM_IOS
   else if(Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_C))
   {
      if(Farso::Controller::getSkin() != NULL)
      {
         Farso::Controller::unloadSkin();
      }
   }
   else if(Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_M))
   {
      if((Farso::Controller::getSkin() == NULL) ||
         (Farso::Controller::getSkin()->getFileName() != "skins/moderna.skin"))
      {
         Farso::Controller::loadSkin("skins/moderna.skin");
      }
   }
   else if(Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_W))
   {
      if((Farso::Controller::getSkin() == NULL) ||
            (Farso::Controller::getSkin()->getFileName() !=
             "skins/wyrmheart.skin"))
      {
         Farso::Controller::loadSkin("skins/wyrmheart.skin");
      }
   }
#endif

}


