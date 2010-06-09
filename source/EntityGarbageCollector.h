#ifndef ENTITYGARBAGECOLLECTOR_H
#define ENTITYGARBAGECOLLECTOR_H

#include "Entity.h"
#include "Control.h"

#include <SDL_mutex.h>
#include <list>
#include <map>
using namespace std;


class EntityGarbageCollector
{
    public:
        static int runGarbageCollection(void* unused);

        static void onSetup();

        static void addEntity(Entity* inEntity);
        static void addControl(Entity* inEntity, Control* inControl);

        static void onUnload();

        static volatile bool mGameRunning;

    private:
        static list<Entity*> mEntities;
        static map<Entity*, Control*> mControls;

        static SDL_mutex* mEntityLock;
        static SDL_mutex* mControlLock;

};

#endif // ENTITYGARBAGECOLLECTOR_H
