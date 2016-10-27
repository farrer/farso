
#include "../../src/controller.h"

namespace FarsoExample
{

#define EXAMPLE_WINDOW_WIDTH    1024
#define EXAMPLE_WINDOW_HEIGHT    768

   /*! The rendering-independent example class */
   class Example
   {
      public:
         /*! Constructor */
         Example();
         /*! Destructor */
         ~Example();

         /*! Init the example code */
         void init(Farso::RendererType  rendererType);

         /* \return if should quit the example */
         bool shouldQuit();

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

      private:
         Farso::Menu* menu; /**< A menu to be used */
   };

}

