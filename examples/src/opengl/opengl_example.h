
#include "../common.h"

#ifndef _farso_opengl_example_h
#define _farso_opengl_example_h

namespace FarsoExample
{

class OpenGLExample
{
    public:
       OpenGLExample();
       ~OpenGLExample();

       void run();

    private:
       /*! The window used */
       SDL_Window* window;
       /*! Our OpenGL context */
       SDL_GLContext glcontext;
       /*! The example itself */
       Example* example;
};

#endif

}

