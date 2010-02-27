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

bool GameModule::onLoad()
{
    return true;
}


void GameModule::onInit()
{
}


void GameModule::onCleanup()
{
}

