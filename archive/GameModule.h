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
#include "EntityGarbageCollector.h"

#include "Module.h"
#include "TerrainGrid.h"
#include "Point2D.h"
#include "LuaMachine.h"
#include "TextBox.h"
#include "Tank.h"
#include "Bullet.h"
#include "PlayerControl.h"
#include "GameCamera.h"
#include "Label.h"
#include "ModelStack.h"
#include "DummyBulletControl.h"

#include <SDL_thread.h>
#include <SDL_mutex.h>
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
        virtual void onOpen();
        virtual void onClose();
        virtual void onUnload();

        void addTank(Control::ControlType inControlType,
            const Vector3D<float>& inPosition = Vector3D<float>(10.0f));

        void addControl(Control* inControl);

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

        //dummy cheat variable
        bool mProduceBullet;

        LuaMachine mLua;
        TextBox* mLuaConsole;
        GameCamera mCamera;

        SDL_Thread* mCollisionThread;
        SDL_Thread* mEntityGarbageCollectorThread;
        //SDL_mutex* mEntityLock;
        long mTimes;

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

        Light mSunLight;
        Light mMoonLight;
        bool mLights[2];

        TSphere mSun;
        TSphere mMoon;
        TSphere mBullet;
        float mSunRotation;
        float mMoonRotation;

        int mNumTanks;
        ModelStack* mTestModel;

        Vector3D<GLfloat> mTrackball;
        Vector3D<GLfloat> mPanning;

        list<Control*> mControls;
        Tank* mPlayerTank;
        Control* mPlayerControls;

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
