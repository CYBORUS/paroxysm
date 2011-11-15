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

LuaAPI::LuaAPI(CGE::SceneGraphNode& inHeadNode) : mSkyBoxActor(&mSkyBox),
    mHeadNode(inHeadNode)
{
    activate();
    mGrid.setSize(20, 20);
    mGrid.buildVBO();

    CGE::Actor* a = new CGE::Actor(&mGrid);
    inHeadNode.addChildNode(a);
    mBin.addActor(a);

    mSkyBoxBin.addActor(&mSkyBoxActor);
    mHeadNode.addChildNode(&mSkyBoxActor);
    mSkyBoxActor.matrix().translate(10.0f, 10.0f, 0.0f);
    mSkyBoxActor.matrix().scale(100.0f);

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
    mLua.addFunction("sendBoth", luaSendBoth);
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

void LuaAPI::update()
{
    mLua.runCommand("update()");
    checkForCollisions();

    for (size_t i = 0; i < mEntities.size(); ++i)
    {
        CGE::Entity* e = mEntities[i];
        if (e) e->update();
    }
}

CGE::Entity* LuaAPI::getEntity(size_t inIndex)
{
    return inIndex < mEntities.size() ? mEntities[inIndex] : NULL;
}

void LuaAPI::removeEntity(size_t inIndex)
{
    if (inIndex < mEntities.size() && mEntities[inIndex])
    {
        mCollisionEntities.remove(mEntities[inIndex]);
        delete mEntities[inIndex];
        mEntities[inIndex] = NULL;
        mHoles.push_back(inIndex);
    }
}

void LuaAPI::addActor(size_t inIndex, const std::string& inModel)
{
    if (inIndex < mEntities.size() && mEntities[inIndex])
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
    if (inIndex < mEntities.size() && mEntities[inIndex])
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
    if (inEntity < mEntities.size() && mEntities[inEntity])
    {
        CGE::Entity* which = mEntities[inEntity];

        if (inActor < which->numActors())
        {
            which->rotateActor(inActor, inX, inY, inZ);
        }
    }
}

void LuaAPI::resetEntityActorMatrix(size_t inEntity, size_t inActor)
{
    if (inEntity < mEntities.size() && mEntities[inEntity])
    {
        CGE::Entity* which = mEntities[inEntity];

        if (inActor < which->numActors())
        {
            which->resetActorMatrix(inActor);
        }
    }
}

void LuaAPI::setEntityPosition(size_t inIndex, double inX, double inY,
    double inZ)
{
    if (inIndex < mEntities.size() && mEntities[inIndex])
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
        CGE::Entity* entity = new CGE::Entity(inState);
        luaThis->mCollisionEntities.push_back(entity);
        luaThis->mHeadNode.addChildNode(entity);
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
    }

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
        CGE::Entity* e = luaThis->getEntity(index);
        if (e)
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
        CGE::Entity* e = luaThis->getEntity(index);
        if (e) mass = e->getMass();
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
        CGE::Entity* e = luaThis->getEntity(index);
        if (e) e->setMass(mass);
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
        CGE::Entity* e = luaThis->getEntity(index);
        if (e) radius = e->getMass();
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
        CGE::Entity* e = luaThis->getEntity(index);
        if (e) e->setRadius(radius);
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
        CGE::Entity* e = luaThis->getEntity(index);
        if (e)
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

    CGE::Entity* e = luaThis->getEntity(index);
    if (e) e->setVelocity(v);

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
        CGE::Entity* e = luaThis->getEntity(index);

        if (e && model && length)
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
        CGE::Entity* e = luaThis->getEntity(index);

        if (e) e->setCollisionCB(inState);
    }

    return 0;
}

void LuaAPI::checkForCollisions()
{
    for (std::list<CGE::Entity*>::iterator i = mCollisionEntities.begin();
         i != mCollisionEntities.end(); ++i)
    {
         std::list<CGE::Entity*>::iterator j = i;
         ++j;
         for (; j != mCollisionEntities.end(); ++j)
         {
              CGE::Entity* e1 = *i;
              CGE::Entity* e2 = *j;
              if (e1->isInRangeOf(e2))
              {
                  e1->onCollision(mLua.getState(), e2);
                  e2->onCollision(mLua.getState(), e1);
              }
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
