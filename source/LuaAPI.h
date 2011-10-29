#ifndef LUAAPI_H
#define LUAAPI_H

#include <CGE/LuaMachine.h>
#include <CGE/Reference.h>
#include <CGE/Entity.h>
#include <vector>

class LuaAPI
{
    public:
        LuaAPI();
        virtual ~LuaAPI();

    protected:
    private:
        CGE::LuaMachine mLua;

        std::vector<size_t> mEntityHoles;
        std::vector< CGE::Reference<CGE::Entity> > mEntities;
};

#endif
