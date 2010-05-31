#ifndef COLLISIONENGINE_H
#define COLLISIONENGINE_H

#include "Entity.h"

#include <iostream>
#include <vector>
#include <list>
using namespace std;

struct Functor
{
    bool operator ()(Entity* a, Entity* b)
    {
        Vector3D<float> x = a->getPosition();
        Vector3D<float> y = b->getPosition();
        //cerr << "returning: " << x[0] << " < " << y[0] << endl;
        return (x[0] < y[0]);
    }
};

class CollisionEngine
{
    public:
        static void onSetup();
        static void onUnload();

        static void addEntity(Entity* inEntity);
        static void removeEntity(Entity* inEntity);

        static void checkCollisions();
        static int checkCollisions(void* unused);

        static volatile bool mCollisionsRunning;

    private:
        static Functor mFunc;
        static list<Entity*> mEntities;
        static float mLargestRadius;

};

#endif // COLLISIONENGINE_H
