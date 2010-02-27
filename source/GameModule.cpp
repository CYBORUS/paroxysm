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
}


void GameModule::onInit()
{
}


void GameModule::onCleanup()
{
}

