
#ifndef _farso_ogre3d_example_h
#define _farso_ogre3d_example_h

#include "../common.h"
#include <goblin/baseapp.h>


namespace FarsoExample
{

#define APP_NAME         "farso_ogre3d_example"
#define BASE_DATA_DIR    "data"
#define DEFAULT_WINDOW_WIDTH   EXAMPLE_WINDOW_WIDTH
#define DEFAULT_WINDOW_HEIGHT  EXAMPLE_WINDOW_HEIGHT
#define WIDTH_TO_DOUBLE_SIZE    10000 // No double size.

/*! The ogre3d example core application. Note that we use Goblin to
 * simplify our code creation. */
class Ogre3dExample : public Goblin::BaseApp
{
   public:
      /*! Constructor */
      Ogre3dExample();
      /*! Destructor */
      ~Ogre3dExample();

   protected:
      /* virtual functions from Goblin */
      Ogre::String getApplicationUnixName() { return APP_NAME; };
      Ogre::String getBaseDataDir() { return BASE_DATA_DIR; };
      int getDefaultWindowWidth() { return DEFAULT_WINDOW_WIDTH; };
      int getDefaultWindowHeight() { return DEFAULT_WINDOW_HEIGHT; };
      Orientation getDefaultOrientation() { return LANDSCAPE; };
      int getMinWidthToUseDoubleSizedGui() { return WIDTH_TO_DOUBLE_SIZE; };
      
      /* virtual functions from Goblin */
      bool doInit();
      void getDataDirectories(Ogre::String** dataDirectories,
            Ogre::String** dataGroups, int& total);
      void doLowMemoryClean();
      void doSendToBackground();
      void doSendToForeground();
      void doCycle();
      bool shouldQuit();

   private:
      Example* example;
};


}



#endif

