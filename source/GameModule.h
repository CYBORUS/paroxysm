#ifndef GAMEMODULE_H
#define GAMEMODULE_H

#include "Module.h"
#include "TerrainGrid.h"


#include <iostream>
#include <fstream>
using namespace std;

class GameModule : public Module
{
    public:
        GameModule(const char* inMapFile);
        virtual ~GameModule();


    protected:
        TerrainGrid mTerrain;
};

#endif // GAMEMODULE_H
