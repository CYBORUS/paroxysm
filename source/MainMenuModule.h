#ifndef MAINMENUMODULE_H
#define MAINMENUMODULE_H

#include "Module.h"
#include "DisplayEngine.h"
#include "Point2D.h"
#include "HUD.h"

//#include "MapEditorModule.h"
#include "LoadMapModule.h"
#include "LoadGameModule.h"

#include "Button.h"

#include "OGL.h"

#include <sstream>
#include <iostream>
using namespace std;


#define MAIN_MENU_RANGE 10.0

#define NUM_TEXTURES 20


//define the controls here
#define NO_CONTROL_CLICKED 100
#define MAP_EDITOR 101
#define NEW_GAME 102

class MainMenuModule : public Module
{
    public:
        bool onLoad();
        void onInit();
        void onLoop();
        void onFrame();
        void onCleanup();
        Module* next();

        bool isDead();


    protected:
        void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);

        virtual void onButtonPress(int inID);

    private:
        Module* mNextModule;

        //GLuint mTextures[NUM_TEXTURES];
        //GLuint mList;

        Point2D<int> mDisplay;

        bool mDead;

        //void displayButton(float inX, float inY, float inWidthRange, float inHeightRange, GLuint inTexture);

};

#endif // MAINMENUMODULE_H
