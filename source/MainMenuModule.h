#ifndef MAINMENUMODULE_H
#define MAINMENUMODULE_H

#include "Module.h"
#include "DisplayEngine.h"

#include <SDL_opengl.h>


#define RANGE 10.0

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

};

#endif // MAINMENUMODULE_H
