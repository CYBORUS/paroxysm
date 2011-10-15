#ifndef GAMEMODULE_H
#define GAMEMODULE_H

#include "ParoxysmModule.h"
#include "Tank.h"


class GameModule : public ParoxysmModule
{
    public:
        GameModule();
        virtual ~GameModule();

    protected:
    private:

        Tank* mPlayerTank;
};

#endif // GAMEMODULE_H
