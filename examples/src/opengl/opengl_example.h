
#ifndef _farso_opengl_example_h
#define _farso_opengl_example_h

#include "../common.h"

#include "opengl_app.h"

namespace FarsoExample
{

class OpenGLExample : public OpenGLApp
{
    public:
       OpenGLExample();
       ~OpenGLExample();

       void init();

       bool shouldQuit();

       void step(bool leftButtonPressed, bool rightButtonPressed,
             int mouseX, int mouseY);

    private:
       /*! The example itself */
       Example* example;
       Farso::DefaultLoader loader;
};

}

#endif

