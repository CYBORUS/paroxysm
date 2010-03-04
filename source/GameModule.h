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

#ifndef GAMEMODULE_H
#define GAMEMODULE_H

#include "Module.h"
#include "TerrainGrid.h"
#include "Point2D.h"
#include "LuaMachine.h"
#include "TextBox.h"
#include "Tank.h"
#include "Control.h"
#include "PlayerControl.h"
#include "GameCamera.h"
#include "Label.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

class GameModule : public Module
{
    public:
        GameModule(const char* inMapFile);
        virtual ~GameModule();

        virtual void onLoop();
        virtual void onFrame();
        virtual bool onLoad();
        virtual void onInit();
        virtual void onCleanup();

        virtual void addTank(ControlType inControlType);

        /// Lua wiring
        static int luaCameraPan(lua_State* inState);
        static int luaAddTank(lua_State* inState);
        static int luaGetHeight(lua_State* inState);
        static GameModule* luaGM;
        static GameCamera* luaCamera;
        static vector<Tank*>* luaTanks;

    protected:
        virtual void onRButtonDown(int inX, int inY);
        virtual void onRButtonUp(int inX, int inY);
        virtual void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);
        virtual void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onMouseWheel(bool inUp, bool inDown);

        void getHeight(float inX, float inZ);

        LuaMachine mLua;
        TextBox* mLuaConsole;
        GameCamera mCamera;

        TerrainGrid mTerrain;
        GLuint mTerrainDisplay;

        bool mDead;
        Module* mNextModule;

        Point2D<int> mDisplay;
        Point2D<int> mCenter;
        Point2D<int> mTerrainSize;
        Point2D<int> mOldMouse;
        int mMouseMode;

        Light mLight;

        Vector3D<GLfloat> mTrackball;
        Vector3D<GLfloat> mPanning;

        vector<Tank*> mTanks;
        vector<Control*> mControls;

        Label* mFPSLabel;
        unsigned int mFPS;
};

#endif
