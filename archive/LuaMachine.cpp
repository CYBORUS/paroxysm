/**
 *  This file is part of "Paroxysm".
 *
 *  "Paroxysm" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Paroxysm" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with "Paroxysm".  If not, see <http://www.gnu.org/licenses/>.
 */

#include "LuaMachine.h"

LogFile LuaMachine::mLogFile;

LuaMachine::LuaMachine() : mLuaState(luaL_newstate())
{
    //luaL_openlibs(mLuaState); // massive security hole
    mLogFile.start("lua");
}

LuaMachine::~LuaMachine()
{
    lua_close(mLuaState);
}

void LuaMachine::reportErrors()
{
    if (mStatus)
    {
        mError = "-- ";
        mError += lua_tostring(mLuaState, -1);
        mLogFile.addLine(mError);
        lua_pop(mLuaState, 1); // remove error message
    }
}

void LuaMachine::execute()
{
    mStatus = lua_pcall(mLuaState, 0, LUA_MULTRET, 0);
    if (mStatus) reportErrors();
}

void LuaMachine::loadFile(const char* inFile)
{
    mStatus = luaL_loadfile(mLuaState, inFile);
    if (mStatus)
        reportErrors();
    else
        execute();
}

void LuaMachine::runCommand(const char* inCommand)
{
    mStatus = luaL_loadstring(mLuaState, inCommand);
    if (mStatus)
        reportErrors();
    else
        execute();
}
