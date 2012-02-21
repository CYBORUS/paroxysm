#include "EntityGarbageCollector.h"

volatile bool EntityGarbageCollector::mGameRunning;
list<Control*> EntityGarbageCollector::mControls;
SDL_mutex* EntityGarbageCollector::mControlLock;

void EntityGarbageCollector::onSetup()
{
    mControlLock = SDL_CreateMutex();
}


int EntityGarbageCollector::runGarbageCollection(void* unused)
{
    mGameRunning = true;

    while (mGameRunning)
    {
        SDL_mutexP(mControlLock);
        list<Control*>::iterator itControls = mControls.begin();
        while (itControls != mControls.end())
        {
            if (!(*itControls)->getEntity()->isAlive() && !(*itControls)->getEntity()->hasReferences()) /*(*itControls)->isGameDead() && (*itControls)->getEntity()->isRenderDead())*/
            {
                Control* dead = *itControls;
                itControls = mControls.erase(itControls);
                delete dead;
            }
            else
            {
                ++itControls;
            }
        }

        SDL_mutexV(mControlLock);
        SDL_Delay(100);
    }


    return 0;
}

void EntityGarbageCollector::addControl(Control* inControl)
{
    SDL_mutexP(mControlLock);
    mControls.push_back(inControl);
    SDL_mutexV(mControlLock);
}


void EntityGarbageCollector::onUnload()
{
    //mGameRunning = false;

    list<Control*>::iterator itControls = mControls.begin();

    for (; itControls != mControls.end(); ++itControls)
    {
        delete *itControls;
    }

    mControls.clear();
}
