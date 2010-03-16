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

#include "CollisionEngine.h"

#include "Module.h"
#include "TerrainGrid.h"
#include "Point2D.h"
#include "LuaMachine.h"
#include "TextBox.h"
#include "Tank.h"
#include "Bullet.h"
#include "Control.h"
#include "PlayerControl.h"
#include "GameCamera.h"
#include "Label.h"
#include "Model3D.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>
#include <map>
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

        void addTank(ControlType inControlType,
            const Vector3D<float>& inPosition = Vector3D<float>(10.0f));

    protected:
        virtual void onLButtonDown(int inX, int inY);
        virtual void onLButtonUp(int inX, int inY);
        virtual void onRButtonDown(int inX, int inY);
        virtual void onRButtonUp(int inX, int inY);
        virtual void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);
        virtual void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onMouseWheel(bool inUp, bool inDown);

        inline Vector3D<float> findMouseObjectPoint(int inX, int inY);

        void getHeight(float inX, float inZ);

        LuaMachine mLua;
        TextBox* mLuaConsole;
        GameCamera mCamera;

        TerrainGrid mTerrain;

        Matrix<GLdouble> mModelView;
        Vector3D<GLint> mViewport;
        Matrix<GLdouble> mProjection;
        bool mSceneChanged;


        bool mDead;
        Module* mNextModule;

        Point2D<int> mDisplay;
        Point2D<int> mCenter;
        Point2D<int> mTerrainSize;
        Point2D<int> mOldMouse;
        int mMouseMode;

        Light mLight;

        TSphere mSun;

        int mNumTanks;
        Model3D* mTestModel;

        Vector3D<GLfloat> mTrackball;
        Vector3D<GLfloat> mPanning;

        list<Entity*> mEntities;
        Tank* mPlayerTank;
        Control* mPlayerControls;
        map<Tank*, Control*> mControls;

        Label* mFPSLabel;
        unsigned int mFPS;

        /// Lua wiring
        static int luaCameraPan(lua_State* inState);
        static int luaAddTank(lua_State* inState);
        static int luaGetHeight(lua_State* inState);
        static int luaSetFriction(lua_State* inState);

        static TerrainGrid* luaTG;
        static GameModule* luaGM;
        static GameCamera* luaCamera;
        static vector<Tank*>* luaTanks;
};

#endif
