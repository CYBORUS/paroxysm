#include "LuaInputCommand.h"

#include <cassert>
using namespace std;

LuaInputCommand::LuaInputCommand(lua_State* inState)
{
    assert(inState != NULL);
    mCallbackName = lua_tostring(inState, 1);
    mLuaCallback.set(inState);
    mState = inState;
}

LuaInputCommand::LuaInputCommand(lua_State* inState, LUA_INTEGER inKeyNum)
{
    assert(inState != NULL);
    mCallbackName = lua_tostring(inState, 1);
    mLuaCallback.set(inState);
    mState = inState;
    mKeyNum = inKeyNum;
}

LuaInputCommand::~LuaInputCommand()
{
    //dtor
}

void LuaInputCommand::issueCommand(double inIntensity) const
{
   if (mLuaCallback.isSet())
   {
       //pushing onto stack
       mLuaCallback.get();
       lua_pushnumber(mState, inIntensity);

       //calling function
       lua_call(mState, 1, 0);
   }
}
