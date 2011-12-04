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
#include <CGE/Entity.h>
#include <CGE/ResourceManager.h>
#include <CGE/ModelFromFile.h>
#include <CGE/Reference.h>

#include "GeneralBin.h"
#include "SkyBoxBin.h"
#include "SkyBox.h"
#include "TerrainGrid.h"
#include "LuaInputCommand.h"

#include <vector>
#include <string>

typedef CGE::Reference<CGE::Entity> EntityRef;

class LuaAPI
{
    public:
        LuaAPI();
        virtual ~LuaAPI();

        inline void activate() { luaThis = this; }

        void display();
        void update(const mat4f& inProjection);

        inline const vector<LuaInputCommand*>& getLuaInputCommands() const
        {
            return mLuaInputCommands;
        }

    protected:
    private:
        EntityRef getEntity(size_t inIndex);
        void checkForCollisions();
        void removeEntity(size_t inIndex);
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
        //bool mBusyColliding;
        //bool mDebug;

        CGE::LuaMachine mLua;
        CGE::LuaReference mLuaUpdateCallback;
        CGE::ResourceManager<CGE::ModelFromFile> mModels;
        CGE::Camera mCamera;
        CGE::PublicMatrixNode mCameraAnglesNode;
        CGE::PublicMatrixNode mCameraTranslationNode;
        CGE::SoundBuffer mSoundTest;
        CGE::SoundSource mSourceTest;

        std::vector<size_t> mHoles;
        std::vector<EntityRef> mEntities;
        std::vector<LuaInputCommand*> mLuaInputCommands;
        std::list<EntityRef> mCollisionEntities;

//        struct DeadEntity
//        {
//            EntityRef entity;
//            size_t index;
//        };
//
//        std::vector<DeadEntity> mDeadEntities;

        static LuaAPI* luaThis;

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
        static int luaSetEntityCollisionCR(lua_State* inState);

        /// General game state management
        static int luaSetUpdateCallback(lua_State* inState);
        static int luaCreateCommand(lua_State* inState);
        static int luaSendBoth(lua_State* inState);

        /// Camera control
        static int luaCameraFollow(lua_State* inState);
        static int luaCameraUnfollow(lua_State* inState);
        static int luaMoveCamera(lua_State* inState);
        static int luaSetCameraPosition(lua_State* inState);
};

#endif
