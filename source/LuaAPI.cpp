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
#include "Tank.h"
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
    mLua.addFunction("setEntityPosition", luaSetEntityPosition);
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

    for (size_t i = 0; i < mEntities.size(); ++i)
    {
        CGE::Entity* e = mEntities[i];
        if (e) e->update();
    }
}

size_t LuaAPI::addEntity()
{
    CGE::Entity* entity = new CGE::Entity;
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

void LuaAPI::removeEntity(size_t inIndex)
{
    if (inIndex < mEntities.size() && mEntities[inIndex])
    {
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
