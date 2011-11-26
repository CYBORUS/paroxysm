#ifndef LUAINPUTCOMMAND_H
#define LUAINPUTCOMMAND_H

#include <CGE/LuaReference.h>
#include <string>

class LuaInputCommand
{
    public:
        LuaInputCommand(lua_State* inState);
        virtual ~LuaInputCommand();

        void issueCommand(double inIntensity) const;
    protected:
    private:
        CGE::LuaReference mLuaCallback;
        lua_State* mState;
        std::string mCallbackName;
};

#endif // LUAINPUTCOMMAND_H
