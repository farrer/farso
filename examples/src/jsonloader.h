
#include "farsoconfig.h"

#if FARSO_HAS_RAPIDJSON == 1
#include "../../src/controller.h"

namespace FarsoExample
{

   /*! An example to load widgets from a JSON file */
   class JsonLoader
   {
      public:
         /*! Constructor */
         JsonLoader();
         /*! Destructor */
         ~JsonLoader();

         /*! Init the example code */
         void init(Farso::Loader* loader, 
               Farso::RendererType rendererType, 
               Farso::RendererJunctionInfo* extraInfo);

         /* \return if should quit the example */
         bool shouldQuit();

         /* Each frame step */
         void step(bool leftButtonPressed, bool rightButtonPressed,
               int mouseX, int mouseY);

      private:
         Kobold::String loadFile(const Kobold::String& filename);

         Farso::Window* loadWindow;
         Farso::FileSelector* selector;
         bool shouldExit;
   };

}

#endif

