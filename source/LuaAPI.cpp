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

LuaAPI::LuaAPI(CGE::SceneGraphNode& inHeadNode) : mHeadNode(inHeadNode)
{
    activate();
    mGrid.create(20, 20);
    mGrid.buildVBO();

    CGE::Actor* a = new CGE::Actor(&mGrid);
    inHeadNode.addChildNode(a);
    mBin.addActor(a);

    mLua.addFunction("addEntity", luaAddEntity);
    mLua.addFunction("removeEntity", luaRemoveEntity);
    mLua.addFunction("addActor", luaAddActor);
    mLua.addFunction("setEntityDefaultRotation", luaSetEntityDefaultRotation);
    mLua.addFunction("getEntityPosition", luaGetEntityPosition);
    mLua.addFunction("setEntityPosition", luaSetEntityPosition);
    mLua.addFunction("getEntityMass", luaGetEntityMass);
    mLua.addFunction("setEntityMass", luaSetEntityMass);
    mLua.addFunction("getEntityRadius", luaGetEntityRadius);
    mLua.addFunction("setEntityRadius", luaSetEntityRadius);
    mLua.addFunction("getEntityVelocity", luaGetEntityVelocity);
    mLua.addFunction("setEntityVelocity", luaSetEntityVelocity);
    mLua.addFunction("setEntityCollisionCR", luaSetEntityCollisionCR);
    mLua.addFunction("playSound", luaPlaySound);
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

size_t LuaAPI::addEntity()
{
    CGE::Entity* entity = new CGE::Entity;
    mCollisionEntities.push_back(entity);
    mHeadNode.addChildNode(entity);
    size_t outIndex = mEntities.size();

    if (mHoles.size() > 0)
    {
        outIndex = mHoles.back();
        mHoles.pop_back();
        mEntities[outIndex] = entity;
    }
    else
    {
        mEntities.push_back(entity);
    }

    return outIndex;
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
    size_t index = luaThis->addEntity();

    lua_Integer outIndex = index;
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

        if (model && length) luaThis->addActor(index, model);
    }

    return 0;
}

int LuaAPI::luaPlaySound(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc > 0 && lua_isstring(inState, 1))
    {
        const char* path = lua_tostring(inState, 1);
        CGE::Sound* s = luaThis->mSounds.load(path);
        s->play();
    }

    return 0;
}

int LuaAPI::luaSetEntityCollisionCR(lua_State* inState)
{
    assert(luaThis != NULL);
    int argc = lua_gettop(inState);

    if (argc > 2 && lua_isnumber(inState, 1) && lua_isfunction(inState, 2)
        && lua_istable(inState, 3))
    {
        if (argc > 3) lua_pop(inState, argc - 3);

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
