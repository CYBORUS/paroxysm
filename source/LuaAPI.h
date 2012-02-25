/// This file is part of "Paroxysm".
///
/// "Paroxysm" is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// "Paroxysm" is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with "Paroxysm".  If not, see <http://www.gnu.org/licenses/>.

#ifndef LUAAPI_H
#define LUAAPI_H

#include <CGE/OpenAL/SoundBuffer.h>
#include <CGE/OpenAL/SoundSource.h>
#include <CGE/Camera.h>
#include <CGE/PublicMatrixNode.h>
#include <CGE/LuaMachine.h>
#include <CGE/LuaReference.h>
#include <CGE/LuaIndexedResource.h>
#include <CGE/ResourceManager.h>
#include <CGE/ModelFromFile.h>
#include <CGE/Reference.h>

#include "Entity.h"
#include "GeneralBin.h"
#include "SkyBoxBin.h"
#include "SkyBox.h"
#include "TerrainGrid.h"

#include <vector>
#include <map>
#include <string>

typedef CGE::Reference<Entity> EntityRef;

const int MillisecondsPerFrame = 25;

class LuaAPI
{
    public:
        LuaAPI();
        virtual ~LuaAPI();

        inline void activate() { luaThis = this; }

        void display();
        void update(const mat4f& inProjection);
        void onKey(SDLKey inKey, double inIntensity);

    protected:
    private:
        void checkForCollisions();
        void addActor(size_t inIndex, const std::string& inModel);
        void setEntityDefaultRotation(size_t inIndex, double inX, double inY,
            double inZ);
        void setEntityActorRotation(size_t inEntity, size_t inActor, double inX,
            double inY, double inZ);
        void resetEntityActorMatrix(size_t inEntity, size_t inActor);
        void setEntityPosition(size_t inIndex, double inX, double inY,
            double inZ);

        SkyBox mSkyBox;
        CGE::Actor mSkyBoxActor;
        TerrainGrid mGrid;
        CGE::Actor mGridActor;
        GeneralBin mBin;
        SkyBoxBin mSkyBoxBin;

        CGE::LuaMachine mLua;
        CGE::LuaReference mLuaUpdateCallback;
        CGE::ResourceManager<CGE::ModelFromFile> mModels;
        CGE::Camera mCamera;
        CGE::PublicMatrixNode mCameraAnglesNode;
        CGE::PublicMatrixNode mCameraTranslationNode;
        CGE::SoundBuffer mSoundTest;
        CGE::SoundSource mSourceTest;

        CGE::LuaReference mKeyCallbacks[SDLK_LAST];
        std::map<std::string, CGE::LuaReference*> mKeysByName;
        std::list<EntityRef> mCollisionEntities;
        CGE::LuaIndexedResource<EntityRef> mEntities;

        static LuaAPI* luaThis;

        /// Debug functions
        static int luaDebugOutput(lua_State* inState);

        /// Entity management functions
        static int luaAddEntity(lua_State* inState);
        static int luaRemoveEntity(lua_State* inState);
        static int luaSetEntityDefaultRotation(lua_State* inState);
        static int luaSetEntityActorRotation(lua_State* inState);
        static int luaResetEntityActorMatrix(lua_State* inState);
        static int luaGetEntityPosition(lua_State* inState);
        static int luaSetEntityPosition(lua_State* inState);
        static int luaGetEntityMass(lua_State* inState);
        static int luaSetEntityMass(lua_State* inState);
        static int luaGetEntityRadius(lua_State* inState);
        static int luaSetEntityRadius(lua_State* inState);
        static int luaGetEntityVelocity(lua_State* inState);
        static int luaSetEntityVelocity(lua_State* inState);
        static int luaSetTerrainSize(lua_State* inState);
        static int luaAddActor(lua_State* inState);
        static int luaSetEntityCollisionCallback(lua_State* inState);
        static int luaAddEntityLocalMomentum(lua_State* inState);
        static int luaAddEntityGlobalMomentum(lua_State* inState);
        static int luaAddEntityLocalVelocity(lua_State* inState);
        static int luaAddEntityGlobalVelocity(lua_State* inState);

        /// General game state management
        static int luaSetUpdateCallback(lua_State* inState);
        static int luaCreateCommand(lua_State* inState);
        static int luaSendBoth(lua_State* inState);

        /// Camera control
        static int luaCameraFollow(lua_State* inState);
        static int luaCameraUnfollow(lua_State* inState);
        static int luaMoveCamera(lua_State* inState);
        static int luaSetCameraPosition(lua_State* inState);
        static int luaShakeCamera(lua_State* inState);
        static int luaStopCameraShaking(lua_State* inState);
};

#endif
