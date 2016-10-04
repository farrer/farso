
#include "ogre3d_example.h"

#include <kobold/platform.h>
#include <kobold/log.h>

using namespace FarsoExample;

/************************************************************************
 *                              Ogre3dExample                           *
 ************************************************************************/
Ogre3dExample::Ogre3dExample()
{
   example = NULL;
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
}

/************************************************************************
 *                                 doInit                               *
 ************************************************************************/
bool Ogre3dExample::doInit()
{
   example = new Example();
   example->init(Farso::RENDERER_TYPE_OGRE3D);
   return true;
}

/************************************************************************
 *                            getDataDirectories                        *
 ************************************************************************/
void Ogre3dExample::getDataDirectories(Ogre::String** dataDirectories,
      Ogre::String** dataGroups, int& total)
{
   static Ogre::String dirs[] = {"gui", "fonts"};
   (*dataDirectories) = &dirs[0];
   (*dataGroups) = &dirs[0];
   total = 2;
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
 *                                doCycle                               *
 ************************************************************************/
void Ogre3dExample::doCycle()
{
   example->step(leftButtonPressed, false, mouseX, mouseY);
}

/************************************************************************
 *                              shouldQuit                              *
 ************************************************************************/
bool Ogre3dExample::shouldQuit()
{
   return example->shouldQuit();
}

