#include "GameModule.h"

GameModule::GameModule(const char* inMapFile)
{
    ifstream input(inMapFile);

    input >> mTerrain;
}

GameModule::~GameModule()
{
    //dtor
}
