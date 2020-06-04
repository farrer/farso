
#include "ogre3d_example.h"

#include <kobold/platform.h>
#include <kobold/log.h>

using namespace FarsoExample;

/************************************************************************
 *                              Ogre3dExample                           *
 ************************************************************************/
Ogre3dExample::Ogre3dExample()
              :Goblin::BaseApp(Goblin::CameraConfig())
{
   example = NULL;
   renderer = NULL;
}

/************************************************************************
 *                             ~Ogre3dExample                           *
 ************************************************************************/
Ogre3dExample::~Ogre3dExample()
{
   if(example != NULL)
   {
      delete example;
   }
   if(renderer != NULL)
   {
      delete renderer;
   }
}

/************************************************************************
 *                               doCycleInit                            *
 ************************************************************************/
bool Ogre3dExample::doCycleInit(int callCount, bool& shouldAbort)
{
   example = new Example();
   renderer = new Farso::OgreRenderer(getSceneManager(), 
         ogreRoot->getRenderSystem());
   example->init(&loader, renderer, "");
   return true;
}

/************************************************************************
 *                            getDataDirectories                        *
 ************************************************************************/
void Ogre3dExample::getDataDirectories(Ogre::String** dataDirectories,
      Ogre::String** dataGroups, int& total)
{
   static Ogre::String dirs[] = {"gui"};
   (*dataDirectories) = &dirs[0];
   (*dataGroups) = &dirs[0];
   total = 1;
}

/************************************************************************
 *                            doLowMemoryClean                          *
 ************************************************************************/
void Ogre3dExample::doLowMemoryClean()
{
}

/************************************************************************
 *                          doSendToBackground                          *
 ************************************************************************/
void Ogre3dExample::doSendToBackground()
{
}

/************************************************************************
 *                          doSendToForeground                          *
 ************************************************************************/
void Ogre3dExample::doSendToForeground()
{
}

/************************************************************************
 *                             doBeforeRender                           *
 ************************************************************************/
void Ogre3dExample::doBeforeRender()
{
   example->step(leftButtonPressed, false, mouseX, mouseY);
}

/************************************************************************
 *                             doAfterRender                            *
 ************************************************************************/
void Ogre3dExample::doAfterRender()
{
}

/************************************************************************
 *                              shouldQuit                              *
 ************************************************************************/
bool Ogre3dExample::shouldQuit()
{
   return example->shouldQuit();
}

