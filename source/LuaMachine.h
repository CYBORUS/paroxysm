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

#ifndef _LUAMACHINE_H_
#define _LUAMACHINE_H_

#include "LogFile.h"

#include <lua.hpp>

#include <iostream>
#include <string>
using namespace std;

class LuaMachine
{
    public:
        LuaMachine();
        virtual ~LuaMachine();

        void loadFile(const char* inFile);
        void runCommand(const char* inCommand);
        void regFunc(const char* inName, lua_CFunction inFunction);

    private:
        void execute();
        void reportErrors();

        static LogFile mLogFile;

        lua_State* mLuaState;
        int mStatus;
        string mError;
};

inline void LuaMachine::regFunc(const char* inName, lua_CFunction inFunction)
{
    lua_register(mLuaState, inName, inFunction);
}

#endif
