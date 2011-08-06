#ifndef ENTITYGARBAGECOLLECTOR_H
#define ENTITYGARBAGECOLLECTOR_H

#include "Entity.h"
#include "Control.h"

#include <SDL_mutex.h>
#include <list>
using namespace std;


class EntityGarbageCollector
{
    public:
        static int runGarbageCollection(void* unused);

        static void onSetup();

        static void addControl(Control* inControl);

        static void onUnload();

        static volatile bool mGameRunning;

    private:
        static list<Control*> mControls;

        static SDL_mutex* mControlLock;

};

#endif // ENTITYGARBAGECOLLECTOR_H
