#ifndef MAINMENUMODULE_H
#define MAINMENUMODULE_H

#include "Module.h"
#include "DisplayEngine.h"
#include "Point2D.h"
#include "HUD.h"

#include "Button.h"

#include <SDL_opengl.h>

#include <sstream>
#include <iostream>
using namespace std;


#define RANGE 10.0

#define NUM_TEXTURES 20

class MainMenuModule : public Module
{
    public:
        MainMenuModule();

        bool onInit();
        void onLoop();
        void onFrame();
        void onCleanup();
        Module* next();


    protected:
        void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);


        Module* mNextModule;

        GLuint mTextures[NUM_TEXTURES];
        GLuint mList;

        Button* mNewGameButton;
        Button* mMapEditorButton;

        Point2D<int> mDisplay;
        HUD mHUD;

        int mCounter;
        bool newHover;
        bool mapHover;

        void displayButton(float inX, float inY, float inWidthRange, float inHeightRange, GLuint inTexture);

};

#endif // MAINMENUMODULE_H
