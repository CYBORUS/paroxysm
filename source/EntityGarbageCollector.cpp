#include "EntityGarbageCollector.h"

volatile bool EntityGarbageCollector::mGameRunning;
list<Entity*> EntityGarbageCollector::mEntities;
map<Entity*, Control*> EntityGarbageCollector::mControls;
SDL_mutex* EntityGarbageCollector::mEntityLock;
SDL_mutex* EntityGarbageCollector::mControlLock;

void EntityGarbageCollector::onSetup()
{
    mEntityLock = SDL_CreateMutex();
    mControlLock = SDL_CreateMutex();
}


int EntityGarbageCollector::runGarbageCollection(void* unused)
{
    mGameRunning = true;

    while (mGameRunning)
    {
        SDL_mutexP(mEntityLock);
        list<Entity*>::iterator itEntities = mEntities.begin();
        while (itEntities != mEntities.end())
        {
            if (!(*itEntities)->isAlive() && (*itEntities)->isGameDead() && (*itEntities)->isRenderDead())
            {
                Entity* dead = *itEntities;
                itEntities = mEntities.erase(itEntities);
                delete dead;
            }
            else
            {
                ++itEntities;
            }
        }

        SDL_mutexV(mEntityLock);
        SDL_Delay(100);
    }


    return 0;
}


void EntityGarbageCollector::addEntity(Entity* inEntity)
{
    SDL_mutexP(mEntityLock);
    mEntities.push_back(inEntity);
    SDL_mutexV(mEntityLock);
}

void EntityGarbageCollector::addControl(Entity* inEntity, Control* inControl)
{
    SDL_mutexP(mControlLock);
    mControls[inEntity] = inControl;
    SDL_mutexV(mControlLock);
}


void EntityGarbageCollector::onUnload()
{
    //mGameRunning = false;

    map<Entity*, Control*>::iterator itControls = mControls.begin();

    for (; itControls != mControls.end(); ++itControls)
    {
        delete itControls->second;
    }

    list<Entity*>::iterator itEntities = mEntities.begin();
    for (; itEntities != mEntities.end(); ++itEntities)
    {
        delete *itEntities;
    }


    mEntities.clear();
    mControls.clear();

    SDL_DestroyMutex(mEntityLock);
}
