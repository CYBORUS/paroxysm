#ifndef ENTITYGARBAGECOLLECTOR_H
#define ENTITYGARBAGECOLLECTOR_H

#include "Entity.h"
#include "Control.h"

#include <list>
#include <map>
using namespace std;


class EntityGarbageCollector
{
    public:
        static int runGarbageCollection(void* unused);

        static void addEntity(Entity* inEntity);
        static void addControl(Entity* inEntity, Control* inControl);

        static void onUnload();

        static bool mGameRunning;

    private:
        static list<Entity*> mEntities;
        static map<Entity*, Control*> mControls;
};

#endif // ENTITYGARBAGECOLLECTOR_H
