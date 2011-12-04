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

#include "LuaAPI.h"
#include <CGE/LuaReference.h>
#include <cassert>
#include <iostream>

using namespace std;

LuaAPI* LuaAPI::luaThis = NULL;

LuaAPI::LuaAPI() : mSkyBoxActor(&mSkyBox), mGridActor(&mGrid),
    mSourceTest(mSoundTest)
{
    activate();

    mCamera.setAngle(-60.0f);
    mCamera.setDistance(16.0f);
    mCamera.update();

    mGrid.setSize(20, 20);
    mGrid.buildVBO();

    mSoundTest.loadFile("data/audio/pew.wav");
    mBin.addActor(&mGridActor);

    mSkyBoxBin.addActor(&mSkyBoxActor);
    mSkyBoxActor.translate(10.0f, 10.0f, 0.0f);
    mSkyBoxActor.scale(100.0f);

    mCameraAnglesNode.addChildNode(&mSkyBoxActor);
    mCameraAnglesNode.addChildNode(&mCameraTranslationNode);
    mCameraTranslationNode.addChildNode(&mGridActor);

    mLua.addFunction("addEntity", luaAddEntity);
    mLua.addFunction("removeEntity", luaRemoveEntity);
    mLua.addFunction("addActor", luaAddActor);
    mLua.addFunction("setEntityDefaultRotation", luaSetEntityDefaultRotation);
    mLua.addFunction("setEntityActorRotation", luaSetEntityActorRotation);
    mLua.addFunction("resetEntityActorMatrix", luaResetEntityActorMatrix);
    mLua.addFunction("getEntityPosition", luaGetEntityPosition);
    mLua.addFunction("setEntityPosition", luaSetEntityPosition);
    mLua.addFunction("getEntityMass", luaGetEntityMass);
    mLua.addFunction("setEntityMass", luaSetEntityMass);
    mLua.addFunction("getEntityRadius", luaGetEntityRadius);
    mLua.addFunction("setEntityRadius", luaSetEntityRadius);
    mLua.addFunction("getEntityVelocity", luaGetEntityVelocity);
    mLua.addFunction("setEntityVelocity", luaSetEntityVelocity);
    mLua.addFunction("setEntityCollisionCR", luaSetEntityCollisionCR);
    mLua.addFunction("setTerrainSize", luaSetTerrainSize);
    mLua.addFunction("setUpdateCallback", luaSetUpdateCallback);
    mLua.addFunction("sendBoth", luaSendBoth);
    mLua.addFunction("createCommand", luaCreateCommand);
    mLua.addFunction("moveCamera", luaMoveCamera);
    mLua.addFunction("setCameraPosition", luaSetCameraPosition);
    mLua.addFunction("cameraFollow", luaCameraFollow);
    mLua.addFunction("cameraUnfollow", luaCameraUnfollow);
    mLua.addFunction("debugOutput", luaDebugOutput);
    mLua.loadFile("data/scripts/api.lua");
    mLua.loadFile("data/scripts/test.lua");
}

LuaAPI::~LuaAPI()
{
}

void LuaAPI::display()
{
    mBin.renderAll();
    mSkyBoxBin.renderAll();
}

void LuaAPI::update(const mat4f& inProjection)
{
    mSourceTest.update();

    if (mLuaUpdateCallback.isSet())
    {
        mLuaUpdateCallback.get();
        lua_call(mLua.getState(), 0, 0);
    }

    checkForCollisions();

    for (size_t i = 0; i < mEntities.size(); ++i)
    {
        EntityRef e = mEntities[i];
        if (!e.isNull()) e->update();
    }

//    if (mDeadEntities.size() > 0)
//    {
//
//        for (size_t i = 0; i < mDeadEntities.size(); ++i)
//        {
//            DeadEntity& de = mDeadEntities[i];
//            if (de.entity)
//            {
//                mCollisionEntities.remove(de.entity);
//                delete de.entity;
//                mEntities[de.index] = NULL;
//                mHoles.push_back(de.index);
//            }
//        }
//
//        mDeadEntities.clear();
    //mDebug = true;
    //}

    mCamera.update();

    mCameraAnglesNode.matrix() = mCamera.getAngleMatrix();
    mCameraTranslationNode.matrix() = mCamera.getTranslateMatrix();

    mCameraAnglesNode.updateMatrices(inProjection);
}

EntityRef LuaAPI::getEntity(size_t inIndex)
{
    return inIndex < mEntities.size() ? mEntities[inIndex] : NULL;
}

void LuaAPI::removeEntity(size_t inIndex)
{
    EntityRef e = getEntity(inIndex);
    if (!e.isNull())
    {
        // We have to check to see if we are in the middle of a collision.
        // Removing an entity from mCollisionEntities will invalidate the
        // iterator and cause a crash. So, instead, we store all the dead
        // entities to be destroyed after the collisions are done.
        cerr << "remove entity..";
        e->setIsBeingDeleted();
        mEntities[inIndex] = NULL;
        mHoles.push_back(inIndex);
        cerr << "done." << endl;

//        if (mBusyColliding)
//        {
//            DeadEntity de;
//            de.entity = e;
//            de.index = inIndex;
//            mDeadEntities.push_back(de);
//        }
//        else
//        {
//            mCollisionEntities.remove(e);
//            delete e;
//            mEntities[inIndex] = NULL;
//            mHoles.push_back(inIndex);
//        }
    }

}

void LuaAPI::addActor(size_t inIndex, const std::string& inModel)
{
    if (inIndex < mEntities.size() && !mEntities[inIndex].isNull())
    {
        CGE::ModelFromFile* mff = mModels.load(inModel);
        CGE::Actor* actor = new CGE::Actor(mff);
        mEntities[inIndex]->addActor(actor);
        mBin.addActor(actor);
    }
}

void LuaAPI::setEntityDefaultRotation(size_t inIndex, double inX, double inY,
                                      double inZ)
{
    if (inIndex < mEntities.size() && !mEntities[inIndex].isNull())
    {
        vec3d v;
        v[0] = inX;
        v[1] = inY;
        v[2] = inZ;
        mEntities[inIndex]->setDefaultRotation(v);
    }
}

void LuaAPI::setEntityActorRotation(size_t inEntity, size_t inActor, double inX,
                                    double inY, double inZ)
{
    if (inEntity < mEntities.size() && !mEntities[inEntity].isNull())
    {
        EntityRef which = mEntities[inEntity];

        if (inActor < which->numActors())
        {
            which->rotateActor(inActor, inX, inY, inZ);
        }
    }
}

void LuaAPI::resetEntityActorMatrix(size_t inEntity, size_t inActor)
{
    if (inEntity < mEntities.size() && !mEntities[inEntity].isNull())
    {
        EntityRef which = mEntities[inEntity];

        if (inActor < which->numActors())
        {
            which->resetActorMatrix(inActor);
        }
    }
}

void LuaAPI::setEntityPosition(size_t inIndex, double inX, double inY,
                               double inZ)
{
    if (inIndex < mEntities.size() && !mEntities[inIndex].isNull())
    {
        vec3d v;
        v[0] = inX;
        v[1] = inY;
        v[2] = inZ;

        mEntities[inIndex]->setPosition(v);
    }
}

int LuaAPI::luaAddEntity(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);
    lua_Integer outIndex = -1;

    if (argc > 0 && lua_istable(inState, 1))
    {
        EntityRef entity = new CGE::Entity(inState);
        luaThis->mCollisionEntities.push_back(entity);
        luaThis->mGridActor.addChildNode(entity);
        size_t index = luaThis->mEntities.size();

        if (luaThis->mHoles.size() > 0)
        {
            index = luaThis->mHoles.back();
            luaThis->mHoles.pop_back();
            luaThis->mEntities[index] = entity;
        }
        else
        {
            luaThis->mEntities.push_back(entity);
        }

        outIndex = index;

        assert(!entity->getIsBeingDeleted());

    }

    assert(luaThis->mCollisionEntities.size() == (luaThis->mEntities.size() - luaThis->mHoles.size()));

    lua_pushinteger(inState, outIndex);
    return 1;
}

int LuaAPI::luaRemoveEntity(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc > 0 && lua_isnumber(inState, 1))
    {
        size_t index = lua_tointeger(inState, 1);
        luaThis->removeEntity(index);
    }

    return 0;
}

int LuaAPI::luaSetEntityDefaultRotation(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc < 4) return 0;

    // Make sure we have 4 numbers
    for (int i = 1; i <= 4; ++i)
    {
        if (!lua_isnumber(inState, i))
        {
            return 0;
        }
    }

    size_t index = lua_tointeger(inState, 1);
    double x = lua_tonumber(inState, 2);
    double y = lua_tonumber(inState, 3);
    double z = lua_tonumber(inState, 4);

    luaThis->setEntityDefaultRotation(index, x, y, z);

    return 0;
}


int LuaAPI::luaSetEntityActorRotation(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc < 5) return 0;

    for (int i = 1; i <= 5; ++i)
    {
        if (!lua_isnumber(inState, i))
        {
            return 0;
        }
    }

    size_t entityIndex = lua_tointeger(inState, 1);
    size_t actorIndex = lua_tointeger(inState, 2);
    double x = lua_tonumber(inState, 3);
    double y = lua_tonumber(inState, 4);
    double z = lua_tonumber(inState, 5);

    luaThis->setEntityActorRotation(entityIndex, actorIndex, x, y, z);

    return 0;
}

int LuaAPI::luaResetEntityActorMatrix(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc < 2) return 0;

    for (int i = 1; i <= 2; ++i)
    {
        if (!lua_isnumber(inState, i))
        {
            return 0;
        }
    }

    size_t entityIndex = lua_tointeger(inState, 1);
    size_t actorIndex = lua_tointeger(inState, 2);

    luaThis->resetEntityActorMatrix(entityIndex, actorIndex);
    return 0;
}

int LuaAPI::luaGetEntityPosition(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    lua_Number x = 1.0;
    lua_Number y = 2.0;
    lua_Number z = 3.0;

    if (argc > 0 && lua_isnumber(inState, 1))
    {
        size_t index = lua_tointeger(inState, 1);
        EntityRef e = luaThis->getEntity(index);
        if (!e.isNull())
        {
            const vec3d& v = e->getPosition();
            x = v[0];
            y = v[1];
            z = v[2];
        }
    }

    lua_pushnumber(inState, x);
    lua_pushnumber(inState, y);
    lua_pushnumber(inState, z);
    return 3;
}

int LuaAPI::luaSetEntityPosition(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc < 4) return 0;

    // Make sure we have 4 numeric parameters.
    for (int i = 1; i <= 4; ++i)
    {
        if (!lua_isnumber(inState, i))
            return 0;
    }

    size_t index = lua_tointeger(inState, 1);
    double x = lua_tonumber(inState, 2);
    double y = lua_tonumber(inState, 3);
    double z = lua_tonumber(inState, 4);

    luaThis->setEntityPosition(index, x, y, z);

    return 0;
}

int LuaAPI::luaGetEntityMass(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);
    lua_Number mass = 0.0;

    if (argc > 0 && lua_isnumber(inState, 1))
    {
        size_t index = lua_tointeger(inState, 1);
        EntityRef e = luaThis->getEntity(index);
        if (!e.isNull()) mass = e->getMass();
    }

    lua_pushnumber(inState, mass);
    return 1;
}

int LuaAPI::luaSetEntityMass(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc > 1 && lua_isnumber(inState, 1) && lua_isnumber(inState, 2))
    {
        size_t index = lua_tointeger(inState, 1);
        lua_Number mass = lua_tonumber(inState, 2);
        EntityRef e = luaThis->getEntity(index);
        if (!e.isNull()) e->setMass(mass);
    }

    return 0;
}

int LuaAPI::luaGetEntityRadius(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);
    lua_Number radius = 0.0;

    if (argc > 0 && lua_isnumber(inState, 1))
    {
        size_t index = lua_tointeger(inState, 1);
        EntityRef e = luaThis->getEntity(index);
        if (!e.isNull()) radius = e->getMass();
    }

    lua_pushnumber(inState, radius);
    return 1;
}

int LuaAPI::luaSetEntityRadius(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc > 1 && lua_isnumber(inState, 1) && lua_isnumber(inState, 2))
    {
        size_t index = lua_tointeger(inState, 1);
        lua_Number radius = lua_tonumber(inState, 2);
        EntityRef e = luaThis->getEntity(index);
        if (!e.isNull()) e->setRadius(radius);
    }

    return 0;
}

int LuaAPI::luaGetEntityVelocity(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    lua_Number x = 1.0;
    lua_Number y = 1.0;
    lua_Number z = 1.0;

    if (argc > 0 && lua_isnumber(inState, 1))
    {
        size_t index = lua_tointeger(inState, 1);
        EntityRef e = luaThis->getEntity(index);
        if (!e.isNull())
        {
            const vec3d& v = e->getVelocity();
            x = v[0];
            y = v[1];
            z = v[2];
        }
    }

    lua_pushnumber(inState, x);
    lua_pushnumber(inState, y);
    lua_pushnumber(inState, z);
    return 3;
}

int LuaAPI::luaSetEntityVelocity(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc < 4) return 0;

    // Make sure we have 4 numeric parameters.
    for (int i = 1; i <= 4; ++i)
    {
        if (!lua_isnumber(inState, i))
            return 0;
    }

    size_t index = lua_tointeger(inState, 1);

    vec3d v;
    v[0] = lua_tonumber(inState, 2);
    v[1] = lua_tonumber(inState, 3);
    v[2] = lua_tonumber(inState, 4);

    EntityRef e = luaThis->getEntity(index);
    if (!e.isNull()) e->setVelocity(v);

    return 0;
}

int LuaAPI::luaAddActor(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc > 1 && lua_isnumber(inState, 1) && lua_isstring(inState, 2))
    {
        size_t index = lua_tointeger(inState, 1);
        size_t length = 0;
        const char* model = lua_tolstring(inState, 2, &length);
        EntityRef e = luaThis->getEntity(index);

        if (!e.isNull() && model && length)
        {
            CGE::ModelFromFile* mff = luaThis->mModels.load(model);
            CGE::Actor* actor = new CGE::Actor(mff);
            size_t actorIndex = 0;

            if (argc > 2 && lua_isnumber(inState, 3))
            {
                size_t parentActor = lua_tointeger(inState, 3);

                actorIndex = luaThis->mEntities[index]->addActor(actor,
                             parentActor);
            }
            else
            {
                actorIndex = luaThis->mEntities[index]->addActor(actor);
            }

            luaThis->mBin.addActor(actor);

            lua_Integer outIndex = actorIndex;
            lua_pushinteger(inState, outIndex);
            return 1;
        }
    }

    lua_pushnil(inState);
    return 1;
}

int LuaAPI::luaSetEntityCollisionCR(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc > 1 && lua_isnumber(inState, 1) && lua_isfunction(inState, 2))
    {
        if (argc > 2) lua_pop(inState, argc - 2);

        size_t index = lua_tointeger(inState, 1);
        EntityRef e = luaThis->getEntity(index);

        if (!e.isNull()) e->setCollisionCB(inState);
    }

    return 0;
}

void LuaAPI::checkForCollisions()
{
    std::list<EntityRef>::iterator i = mCollisionEntities.begin();
    while (i != mCollisionEntities.end())
    {
        EntityRef e1 = *i;
        if (e1->getIsBeingDeleted())
        {
            *i = NULL;
            e1 = NULL;
            cerr << "deleting entity...";
            i = mCollisionEntities.erase(i);
            cerr << "done." << endl;
        }
        else
        {
            std::list<EntityRef>::iterator j = i;
            ++j;
            while (j != mCollisionEntities.end())
            {
                EntityRef e2 = *j;

                if (e2->getIsBeingDeleted())
                {
                    *j = NULL;
                    e2 = NULL;
                    cerr << "deleting entity...";
                    j = mCollisionEntities.erase(j);
                    cerr << "done." << endl;
                }
                else
                {
                    if (e1->isInRangeOf(e2))
                    {
                        e1->onCollision(mLua.getState(), e2);
                        e2->onCollision(mLua.getState(), e1);
                    }
                    ++j;
                }

            }
            ++i;
        }
    }
}

int LuaAPI::luaSendBoth(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc > 1 && lua_isfunction(inState, 1) && lua_istable(inState, 2))
    {
        if (argc > 2) lua_pop(inState, argc - 2);

        CGE::LuaReference table(inState);
        CGE::LuaReference func(inState);

        func.get();
        table.get();
        lua_call(inState, 1, 0);
    }

    return 0;
}

int LuaAPI::luaSetTerrainSize(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc > 1 && lua_isnumber(inState, 1) && lua_isnumber(inState, 2))
    {
        LUA_INTEGER height = lua_tointeger(inState, 1);
        LUA_INTEGER width = lua_tointeger(inState, 2);

        if (height > 0 && height < 1000 && width > 0 && width < 1000)
        {
            luaThis->mGrid.setSize(height, width);
            luaThis->mGrid.buildVBO();
        }
    }

    return 0;
}

int LuaAPI::luaCreateCommand(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc > 1 && lua_isstring(inState, 1) && lua_isfunction(inState, 2))
    {
        if ( argc > 2)
        {
            if (lua_isnumber(inState, 3))
            {
                lua_pop(inState, argc - 3);
                lua_Integer keyNum = lua_tointeger(inState, 3);
                lua_pop(inState, argc - 2);
                luaThis->mLuaInputCommands.push_back(new LuaInputCommand(inState,
                                                     keyNum));
            }
            else
            {
                lua_pop(inState, argc - 2);
                luaThis->mLuaInputCommands.push_back(new LuaInputCommand(inState));
            }
        }
        else
            luaThis->mLuaInputCommands.push_back(new LuaInputCommand(inState));
    }

    return 0;
}

int LuaAPI::luaSetUpdateCallback(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc > 0 && lua_isfunction(inState, 1))
    {
        if (argc > 1) lua_pop(inState, argc - 1);

        luaThis->mLuaUpdateCallback.set(inState);
    }

    return 0;
}

int LuaAPI::luaCameraFollow(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc > 0 && lua_isnumber(inState, 1))
    {
        size_t index = lua_tointeger(inState, 1);
        EntityRef e = luaThis->getEntity(index);
        if (!e.isNull()) luaThis->mCamera.follow(e);
    }

    return 0;
}

int LuaAPI::luaMoveCamera(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc > 2 && lua_isnumber(inState, 1) && lua_isnumber(inState, 2)
            && lua_isnumber(inState, 3))
    {
        lua_Number x = lua_tonumber(inState, 1);
        lua_Number y = lua_tonumber(inState, 2);
        lua_Number z = lua_tonumber(inState, 3);
        luaThis->mCamera.changePosition(x, y, z);
    }

    return 0;
}

int LuaAPI::luaSetCameraPosition(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc > 2 && lua_isnumber(inState, 1) && lua_isnumber(inState, 2)
            && lua_isnumber(inState, 3))
    {
        lua_Number x = lua_tonumber(inState, 1);
        lua_Number y = lua_tonumber(inState, 2);
        lua_Number z = lua_tonumber(inState, 3);
        luaThis->mCamera.setPosition(x, y, z);
    }

    return 0;
}

int LuaAPI::luaCameraUnfollow(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc > 0)
    {
        if (lua_isboolean(inState, 1))
        {
            bool flag = lua_toboolean(inState, 1);
            luaThis->mCamera.unfollow(flag);
        }
        else if (lua_isnumber(inState, 1))
        {
            size_t index = lua_tointeger(inState, 1);
            EntityRef e = luaThis->getEntity(index);

            if (argc > 1 && lua_isboolean(inState, 2))
            {
                bool flag = lua_toboolean(inState, 2);
                luaThis->mCamera.unfollow(e, flag);
            }
            else
            {
                luaThis->mCamera.unfollow(e);
            }
        }
    }

    return 0;
}

int LuaAPI::luaDebugOutput(lua_State* inState)
{
    assert(luaThis != NULL);
    //int argc = lua_gettop(inState);

    cerr << "collision entities -- " << luaThis->mCollisionEntities.size()
        << "\nnormal entities -- " << luaThis->mEntities.size()
        << "\nholes -- " << luaThis->mHoles.size()
        << "\n";

    return 0;
}
