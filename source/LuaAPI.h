#ifndef LUAAPI_H
#define LUAAPI_H

#include <CGE/LuaMachine.h>
#include <CGE/Reference.h>
#include <CGE/Entity.h>
#include <vector>

class LuaAPI
{
    typedef CGE::Reference< CGE::Entity > EntityRef;

    public:
        LuaAPI();
        virtual ~LuaAPI();

        inline void activate() { luaThis = this; }

    protected:
    private:
        size_t addEntity();
        void removeEntity(size_t inIndex);

        CGE::LuaMachine mLua;

        std::vector<size_t> mHoles;
        std::vector<EntityRef> mEntities;

        static LuaAPI* luaThis;
        static int luaAddEntity(lua_State* inState);
};

#endif
