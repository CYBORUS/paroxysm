#include "EntityGarbageCollector.h"

bool EntityGarbageCollector::mGameRunning;
list<Entity*> EntityGarbageCollector::mEntities;
map<Entity*, Control*> EntityGarbageCollector::mControls;

int EntityGarbageCollector::runGarbageCollection(void* unused)
{
    mGameRunning = true;

    while (mGameRunning)
    {
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

        SDL_Delay(100);
    }


    return 0;
}


void EntityGarbageCollector::addEntity(Entity* inEntity)
{
    mEntities.push_back(inEntity);
}

void EntityGarbageCollector::addControl(Entity* inEntity, Control* inControl)
{
    mControls[inEntity] = inControl;
}


void EntityGarbageCollector::onUnload()
{
    mGameRunning = false;
}
