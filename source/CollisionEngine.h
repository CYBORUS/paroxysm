#ifndef COLLISIONENGINE_H
#define COLLISIONENGINE_H

#include "Entity.h"

#include <iostream>
#include <vector>
using namespace std;


class CollisionEngine
{
    public:
        static void addEntity(Entity* inEntity);

        static void checkCollisions();

    private:
        static vector<Entity*> mEntities;
};

#endif // COLLISIONENGINE_H
