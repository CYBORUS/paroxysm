#include "LuaAPI.h"
#include "Tank.h"
#include <cassert>

LuaAPI* LuaAPI::luaThis = NULL;

LuaAPI::LuaAPI()
{
    mLua.loadFile("data/scripts/api.lua");
    activate();
}

LuaAPI::~LuaAPI()
{
}

size_t LuaAPI::addEntity()
{
    EntityRef entity = NULL;
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
    if (inIndex < mEntities.size())
    {
        if (!mEntities[inIndex].isNull())
            mEntities[inIndex] = NULL;
    }
}

int LuaAPI::luaAddEntity(lua_State* inState)
{
    assert(luaThis != NULL);
    size_t index = luaThis->addEntity();
    return 0;
}
