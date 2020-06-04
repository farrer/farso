
#include "../../src/controller.h"

namespace FarsoExample
{

   /*! The rendering-independent example class.
    * \note Although it is defined as an WidgetEventListener, we are only
    *       using it for the "exitButton". The use of listeners is fully
    *       optional */
   class Example : Farso::WidgetEventListener
   {
      public:
         /*! Constructor */
         Example();
         /*! Destructor */
         ~Example();

         /*! Init the example code */
         void init(Farso::Loader* loader, Farso::Renderer* renderer,
               const Kobold::String& dataDir="data/gui/"); 

         /* \return if should quit the example */
         bool shouldQuit();

         /*! From WidgetEventListener. 
          * \note we are only listening for exitButton events */
         void onEvent(const Farso::EventType& eventType, Farso::Widget* widget);

         /* Each frame step */
         void step(bool leftButtonPressed, bool rightButtonPressed,
               int mouseX, int mouseY);

      protected:
         /*! Create a simple dialog window */
         void createDialogWindow();
         /*! Create a window with a stack */
         void createWindowWithStack();
         /*! Create another example window */
         void createOtherWindow();

         /*! Create an window with load/save file selector inner a stack */
         void createLoadSaveWindow();

         /*! Create the window to test all container types */
         void createWindowToTestContainers();
         /*! Add widgets to a container of the window to test containers */
         void addAllWidgetsToContainer(Farso::Container* cont);

         /*! Create our menu to test */
         void createMenu();

         /*! Create a window to test the TreeView widget */
         void createTreeViewWindow();

         /*! Change current skin */
         void changeSkin(const Kobold::String& skin);

      private:
         Farso::Menu* menu; /**< A menu to be used */
         Farso::Button* exitButton; /**< Button to exit example */
         
         Farso::TreeView* treeView; /**< A tree view to use */
         Farso::TextEntry* treeViewEntry; /**< Text Entry to enter new elements
                                               on the tree view */
         Farso::Button* treeViewInsertButton; /**< Button to insert */
         Farso::Button* treeViewRemoveButton; /**< Button to remove */
         Farso::ComboBox* skinCombo; /**< Combo for skin change */

         bool shouldExit; /**< Exit flag */
   };

}

