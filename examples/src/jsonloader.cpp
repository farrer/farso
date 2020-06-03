
#include "jsonloader.h"
#include "size.h"

#if FARSO_HAS_RAPIDJSON == 1

#include <sys/stat.h>
#include <stdlib.h>
#include <kobold/log.h>

using namespace FarsoExample;

/************************************************************************
 *                              Constructor                             *
 ************************************************************************/
JsonLoader::JsonLoader()
{
   shouldExit = false;
}

/************************************************************************
 *                               Destructor                             *
 ************************************************************************/
JsonLoader::~JsonLoader()
{
   Farso::Controller::finish();
}

/************************************************************************
 *                                  init                                *
 ************************************************************************/
void JsonLoader::init(Farso::Loader* loader,
      Farso::RendererType rendererType, 
      Farso::RendererJunctionInfo* extraInfo)
{
   /* Init Farso, with desired renderer */
   Farso::Controller::init(loader, rendererType, 
         FARSO_EXAMPLE_WINDOW_WIDTH, FARSO_EXAMPLE_WINDOW_HEIGHT, 
         32, "data/gui/", extraInfo);

   Farso::FontManager::setDefaultFont("fonts/LiberationSans-Regular.ttf");
   Farso::Controller::setCursor("cursor/sel.png");
   Farso::Controller::loadSkin("skins/clean.skin");

   /* Create the window for loading the JSON */
   loadWindow = new Farso::Window(300, 250, "Load");
   selector = new Farso::FileSelector(true, "data/json", true, loadWindow);
   selector->setFilter(".json");
   loadWindow->setExternPointer(&loadWindow);
   loadWindow->open();
}

/************************************************************************
 *                              shouldQuit                              *
 ************************************************************************/
bool JsonLoader::shouldQuit()
{
#if KOBOLD_PLATFORM != KOBOLD_PLATFORM_IOS &&\
    KOBOLD_PLATFORM != KOBOLD_PLATFORM_ANDROID
   /* Quit on ESC press. */
   return shouldExit ||
          Kobold::Keyboard::isKeyPressed(Kobold::KOBOLD_KEY_ESCAPE);
#endif
   return shouldExit;
}

/************************************************************************
 *                                loadFile                              *
 ************************************************************************/
Kobold::String JsonLoader::loadFile(const Kobold::String& filename)
{
   struct stat tagStat;
   FILE* pFile = fopen(filename.c_str(), "rb");
   if(!pFile)
   {
      Kobold::Log::add("Error: couldn't open file!");
      return "";
   }
   stat(filename.c_str(), &tagStat);
   char* buf = new char[tagStat.st_size];
   size_t result = fread((void*)buf, 1, tagStat.st_size, pFile);
   if(result != (size_t)tagStat.st_size)
   {
      Kobold::Log::add("Error: couldn't load the file");
      return "";
   }

   buf[tagStat.st_size-1] = '\0';
   Kobold::String res = buf;

   delete[] buf;

   return res;
}

/************************************************************************
 *                                  step                                *
 ************************************************************************/
void JsonLoader::step(bool leftButtonPressed, bool rightButtonPressed,
      int mouseX, int mouseY)
{
   if(Farso::Controller::verifyEvents(leftButtonPressed, rightButtonPressed, 
            mouseX, mouseY))
   {
      Farso::Event event = Farso::Controller::getLastEvent();
      if((event.getType() == Farso::EVENT_WINDOW_WILL_CLOSE) && 
         (event.getWidget() == loadWindow))
      {
         shouldExit = true;
      }
      else if(event.getWidget() == selector)
      {
         if(event.getType() == Farso::EVENT_FILESELECTOR_CANCEL)
         {
            shouldExit = true;
         }
         else if(event.getType() == Farso::EVENT_FILESELECTOR_ACCEPT)
         {
            Farso::Controller::insertFromJson(
                  loadFile(selector->getFilename()));
            loadWindow->close();
         }
      }
   }
}

#endif

