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
#include <CGE/LuaMachine.h>
#include "LuaInputCommand.h"
#include <CGE/Entity.h>
#include <CGE/ResourceManager.h>
#include <CGE/ModelFromFile.h>
#include "GeneralBin.h"
#include "SkyBoxBin.h"
#include "SkyBox.h"
#include "TerrainGrid.h"
#include <vector>
#include <string>

class LuaAPI
{
    public:
        LuaAPI(CGE::SceneGraphNode& inHeadNode);
        virtual ~LuaAPI();

        inline void activate() { luaThis = this; }

        void display();
        void update();
        inline const vector<LuaInputCommand*>& getLuaInputCommands() const { return mLuaInputCommands;};

    protected:
    private:
        CGE::Entity* getEntity(size_t inIndex);
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
        GeneralBin mBin;
        SkyBoxBin mSkyBoxBin;

        CGE::LuaMachine mLua;
        CGE::SceneGraphNode& mHeadNode;
        CGE::ResourceManager<CGE::ModelFromFile> mModels;
        CGE::SoundBuffer mSoundTest;
        CGE::SoundSource mSourceTest;

        std::vector<size_t> mHoles;
        std::vector<CGE::Entity*> mEntities;
        std::vector<LuaInputCommand*> mLuaInputCommands;
        std::list<CGE::Entity*> mCollisionEntities;

        static LuaAPI* luaThis;
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
        static int luaCreateCommand(lua_State* inState);

        static int luaSetEntityCollisionCR(lua_State* inState);
        void checkForCollisions();

        static int luaSendBoth(lua_State* inState);
};

#endif
