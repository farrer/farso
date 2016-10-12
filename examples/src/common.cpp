
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
         EXAMPLE_WINDOW_HEIGHT, 32, "data/gui/");

   /* Set a default font (this is only needed when not using a skin). */
   Farso::FontManager::setDefaultFont("fonts/LiberationSans-Regular.ttf");

   /* Set our cursor */
   Farso::Controller::setCursor("cursor/sel.png");

   /* Let's create our windows. Obviously, widgets could be created
    * any time, and not just on init. */
   createDialogWindow();
   createWindowWithStack();
   createOtherWindow();
   createWindowToTestContainers();
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
   window->setPosition(0, 200);

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
//   window->setPosition(300, 200);
   
   /* Add a stack tab to it */
   Farso::StackTab* tab = new Farso::StackTab(window);

   /* Add first option to the stack tab and add some children widgets to it */
   Farso::Container* tabCont = tab->insertTab("Option 1");
   (new Farso::ScrollBar(Farso::ScrollBar::TYPE_VERTICAL, 0, 0, 150, 
                         tabCont))->setTotals(10, 100);
   (new Farso::ScrollBar(Farso::ScrollBar::TYPE_HORIZONTAL, 0, 160, 150, 
                         tabCont))->setTotals(10, 5);
   
   Farso::ScrollText* scrollText = new Farso::ScrollText(160, 10, 130, 160, 
         "fonts/LiberationSans-Regular.ttf", 10, 
         Farso::Colors::colorText, tabCont);
   scrollText->addText(" A scroll text is an widget with ");
   scrollText->addText("multiple", Farso::Color(180, 0, 0, 255));
   scrollText->addText(" text lines, that could be, obviously, scrollable.");
   scrollText->addLineBreak();
   scrollText->addLineBreak();
   scrollText->addText("Subtitle", "fonts/LiberationSans-Regular.ttf", 13, 
       Farso::Font::TEXT_CENTERED, Farso::Color(0, 220, 150, 255));
   scrollText->addLineBreak();
   scrollText->addText(" Text could be defined with different fonts and ");
   scrollText->addText("co", Farso::Color(180, 0, 180, 255));
   scrollText->addText("lors", Farso::Color(180, 180, 0, 255));
   scrollText->addText(" And some more text to go over the total we have. ");


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

   Farso::Picture* pic = new Farso::Picture(120, 0, "cursor/talk.png", window);
   pic->setMouseHint("I'm a png image!");

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
 *                      createWindowToTestContainers                    *
 ************************************************************************/
void Example::createWindowToTestContainers()
{
   Farso::Window* window = new Farso::Window(400, 500, "Containers test");
   window->open();
   window->setPosition(10, 10);

   /* Add a stack tab to it */
   Farso::StackTab* tab = new Farso::StackTab(window);

   /* Add first option to the stack tab and add some children widgets to it */
   Farso::Container* tabCont = tab->insertTab("T. Left");
   Farso::Container* cont = new Farso::Container(
         Farso::Container::TYPE_TOP_LEFT, tabCont);
   addAllWidgetsToContainer(cont);

   tabCont = tab->insertTab("T. Center");
   cont = new Farso::Container(Farso::Container::TYPE_TOP_CENTERED, tabCont);
   addAllWidgetsToContainer(cont);

   tabCont = tab->insertTab("T. Right");
   cont = new Farso::Container(Farso::Container::TYPE_TOP_RIGHT, tabCont);
   addAllWidgetsToContainer(cont);

   tabCont = tab->insertTab("B. Left");
   cont = new Farso::Container(Farso::Container::TYPE_BOTTOM_LEFT, tabCont);
   addAllWidgetsToContainer(cont);

   tabCont = tab->insertTab("B. Center");
   cont = new Farso::Container(Farso::Container::TYPE_BOTTOM_CENTERED,
         tabCont); 
   addAllWidgetsToContainer(cont);

   tabCont = tab->insertTab("B. Right");
   cont = new Farso::Container(Farso::Container::TYPE_BOTTOM_RIGHT, tabCont);
   addAllWidgetsToContainer(cont);
}

/************************************************************************
 *                         addAllWidgetsToContainer                     *
 ************************************************************************/
void Example::addAllWidgetsToContainer(Farso::Container* cont)
{
   new Farso::Button(2, 10, 80, 21, "Button", cont);
   new Farso::Label(2, 33, 80, 21, "Label", cont);
   new Farso::TextEntry(2, 54, 80, 21, cont);
   new Farso::CheckBox(2, 75, 100, "Check box", true, cont);
   Farso::Button* button = new Farso::Button(10, 98, 40, 40, "", cont);
   new Farso::Picture(4, 4, "cursor/talk.png", button);
   (new Farso::ProgressBar(2, 140, 100, 16, cont))->setValue(50);
   new Farso::Picture(2, 158, "cursor/talk.png", cont);
   /* Add a container to our text selector */
   Farso::Container* cont2 = new Farso::Container(
         Farso::Container::TYPE_TOP_LEFT, 2, 192, 140, 60, cont);
   Farso::TextSelector* sel = new Farso::TextSelector(cont2);
   sel->addOption("Morning Star");
   sel->addOption("Rats and Monkeys");
   sel->addOption("Heart of Stone");
   Farso::ScrollText* scrollText = new Farso::ScrollText(2, 257, 120, 100, 
         "fonts/LiberationSans-Regular.ttf", 10, 
         Farso::Colors::colorText, cont);
   scrollText->addText("Some text to the scroll to be scrollable, it is?");
   scrollText->addLineBreak();
   scrollText->addText("And more text maybe it is now, right?");
   scrollText->addLineBreak();
   scrollText->addText("Now it really is, I'm sure!");
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
      /* Got an event, we must treat it. For this example, just echo'ing
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
         (Farso::Controller::getSkin()->getFilename() != "skins/moderna.skin"))
      {
         Farso::Controller::loadSkin("skins/moderna.skin");
      }
   }
   else if(Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_W))
   {
      if((Farso::Controller::getSkin() == NULL) ||
            (Farso::Controller::getSkin()->getFilename() !=
             "skins/wyrmheart.skin"))
      {
         Farso::Controller::loadSkin("skins/wyrmheart.skin");
      }
   }
#endif

}


