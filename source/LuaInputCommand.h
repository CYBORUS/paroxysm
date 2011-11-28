#ifndef LUAINPUTCOMMAND_H
#define LUAINPUTCOMMAND_H

#include <CGE/LuaReference.h>
#include <string>

class LuaInputCommand
{
    public:
        LuaInputCommand(lua_State* inState);
        LuaInputCommand(lua_State* inState, lua_Integer inKeyNum);
        virtual ~LuaInputCommand();

        void issueCommand(double inIntensity) const;
        inline lua_Integer getKeyNum() const { return mKeyNum; };
    protected:
    private:
        CGE::LuaReference mLuaCallback;
        lua_State* mState;
        std::string mCallbackName;
        lua_Integer mKeyNum;
};

#endif // LUAINPUTCOMMAND_H
