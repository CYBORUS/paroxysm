/**
 *  This file is part of "Paroxysm".
 *
 *  "Paroxysm" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Paroxysm" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with "Paroxysm".  If not, see <http://www.gnu.org/licenses/>.
 */

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
        virtual bool onLoad();
        virtual void onOpen();
        virtual void onLoop();
        virtual void onFrame();
        virtual Module* next();
        virtual bool isDead();

    protected:
        virtual void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);

        virtual void onButtonPress(int inID);

    private:
        Module* mNextModule;

        Point2D<int> mDisplay;

        bool mDead;
};

#endif
