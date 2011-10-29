#include "LuaAPI.h"

LuaAPI::LuaAPI()
{
    mLua.loadFile("data/scripts/api.lua");
}

LuaAPI::~LuaAPI()
{
}
