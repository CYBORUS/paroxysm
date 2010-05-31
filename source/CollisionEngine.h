/**
 *  This file is part of "Paroxysm".
 *
 *  "Paroxysm" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Paroxysm" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with "Paroxysm".  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COLLISIONENGINE_H
#define COLLISIONENGINE_H

#include "Entity.h"

#include <SDL_mutex.h>
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

        static SDL_mutex* mEntityLock;
};

#endif // COLLISIONENGINE_H
