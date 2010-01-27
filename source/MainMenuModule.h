#ifndef MAINMENUMODULE_H
#define MAINMENUMODULE_H

#include "Module.h"
#include "DisplayEngine.h"

#include <SDL_opengl.h>

#include <sstream>
#include <iostream>
using namespace std;


#define RANGE 10.0

#define NUM_TEXTURES 20

class MainMenuModule : public Module
{
    public:
        bool onInit();
        void onLoop();
        void onFrame();
        void onCleanup();
        Module* next();


    protected:
        Module* mNextModule;

        GLuint mTextures[NUM_TEXTURES];

};

#endif // MAINMENUMODULE_H
