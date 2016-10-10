
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
   };

}
