#ifndef _farso_opengl_json_loader_h
#define _farso_opengl_json_loader_h

#include "farsoconfig.h"

#if FARSO_HAS_RAPIDJSON == 1

#include "../jsonloader.h"

#include "opengl_app.h"

namespace FarsoExample
{

class OpenGLJsonLoader : public OpenGLApp
{
    public:
       OpenGLJsonLoader();
       ~OpenGLJsonLoader();

       void init();

       bool shouldQuit();

       void step(bool leftButtonPressed, bool rightButtonPressed,
             int mouseX, int mouseY);

    private:
       JsonLoader* jsonLoader;
};

}

#endif

#endif

