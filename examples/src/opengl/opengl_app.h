
#ifndef _farso_opengl_app_h
#define _farso_opengl_app_h

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <kobold/log.h>

#include "../../../src/opengl/openglrenderer.h"
#include "../../../src/loader.h"

namespace FarsoExample
{

#define SDL_WINDOW_WIDTH 1024
#define SDL_WINDOW_HEIGHT 768

class OpenGLApp
{
    public:
       OpenGLApp();
       virtual ~OpenGLApp();

       void run();

       virtual void init() = 0;

       virtual bool shouldQuit() = 0;

       virtual void step(bool leftButtonPressed, bool rightButtonPressed,
             int mouseX, int mouseY) = 0; 

    protected:
       Farso::DefaultLoader loader;
       Farso::OpenGLRenderer* renderer;

    private:
       /*! The window used */
       SDL_Window* window;
       /*! Our OpenGL context */
       SDL_GLContext glcontext;
       /*! Default log to use */
       Kobold::DefaultLog log;
};

}

#endif
