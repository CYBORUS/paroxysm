#ifndef GAMEMODULE_H
#define GAMEMODULE_H

#include "Module.h"
#include "Point2D.h"
#include "HUD.h"

#include "Button.h"
#include "ScrollList.h"


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

        void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);
        void onMouseWheel(bool inUp, bool inDown);
        void onLButtonDown(int inX, int inY);
        void onLButtonUp(int inX, int inY);
        void onRButtonDown(int inX, int inY);
        void onRButtonUp(int inX, int inY);

    protected:
        Module* mNextModule;

        string mSelectedMap;

        HUD mHUD;

};

#endif // GAMEMODULE_H
