#ifndef GAMEMODULE_H
#define GAMEMODULE_H

#include "Module.h"
#include "Point2D.h"
#include "HUD.h"

#include "Button.h"


#include <SDL_opengl.h>

#include <sstream>
#include <iostream>
using namespace std;

#define GAME_RANGE 10.0

class GameModule : public Module
{
    public:
        bool onLoad();
        void onInit();
        void onLoop();
        void onFrame();
        void onCleanup();
        Module* next();


    protected:
        Module* mNextModule;

        HUD mHUD;

};

#endif // GAMEMODULE_H
