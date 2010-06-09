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

#include "CollisionEngine.h"

list<Entity*> CollisionEngine::mEntities;
float CollisionEngine::mLargestRadius = 0.0f;
Functor CollisionEngine::mFunc;
volatile bool CollisionEngine::mCollisionsRunning = false;
SDL_mutex* CollisionEngine::mEntityLock;
long CollisionEngine::mTimes = 0;

void CollisionEngine::onSetup()
{
    //mEntities = inEntities;
    mEntityLock = SDL_CreateMutex();
    //this is a temporary fix, we need to find a way to choose
    //a radius based on the list of entities
    mLargestRadius = 0;
}


void CollisionEngine::addEntity(Entity* inEntity)
{
    SDL_mutexP(mEntityLock);
    mEntities.push_back(inEntity);
    SDL_mutexV(mEntityLock);
    if (inEntity->getRadius() > mLargestRadius)
    {
        mLargestRadius = inEntity->getRadius() * 3.0f;
    }
}

/*
void CollisionEngine::removeEntity(Entity* inEntity)
{
    SDL_mutexP(mEntityLock);
    list<Entity*>::iterator it = mEntities.begin();

    while (it != mEntities.end())
    {
        if (*it == inEntity)
        {
            it = mEntities.erase(it);
            break;
        }
        ++it;
    }
    SDL_mutexV(mEntityLock);
}
*/

/*
void CollisionEngine::checkCollisions()
{
    mEntities.sort(mFunc);

    list<Entity*>::iterator itFirst = mEntities.begin();
    list<Entity*>::iterator itSecond;

    for (; itFirst != mEntities.end(); ++itFirst)
    {
        itSecond = itFirst;
        ++itSecond;

        Entity* first = *itFirst;
        for (; itSecond != mEntities.end(); ++itSecond)
        {
            Entity* second = *itSecond;
            if (abs(first->getPosition()[0] - second->getPosition()[0]) > mLargestRadius)
            {
                break;
            }

            float distance = (first->getPosition() - second->getPosition()).length();

            if (distance < first->getRadius() + second->getRadius())
            {
                first->onCollision(second);
            }

            //after we've had one of the two objects deal with it's collision, test to see if there's still a collision
            distance = (first->getPosition() - second->getPosition()).length();

            if (distance < first->getRadius() + second->getRadius())
            {
                //if there is still a collision, call the second objects collision method
                second->onCollision(first);
            }
        }
    }

    /*
    for (unsigned int i = 0; i < mEntities.size() - 1; ++i)
    {
        for (unsigned int j = i + 1; j < mEntities.size(); ++j)
        {
            float distance = (mEntities[i]->getPosition() - mEntities[j]->getPosition()).length();

            if (distance < mEntities[i]->getRadius() + mEntities[j]->getRadius())
            {
                mEntities[i]->onCollision(mEntities[j]);
            }

            //after we've had one of the two objects deal with it's collision, test to see if there's still a collision
            distance = (mEntities[i]->getPosition() - mEntities[j]->getPosition()).length();

            if (distance < mEntities[i]->getRadius() + mEntities[j]->getRadius())
            {
                //if there is still a collision, call the second objects collision method
                mEntities[j]->onCollision(mEntities[i]);
            }
        }
    }
    */
//}


int CollisionEngine::checkCollisions(void* unused)
{
    mCollisionsRunning = true;
    //SDL_mutex* lock = (SDL_mutex*)inEntityLock;
    //Vector3D<float> pos(10, 10, 0);
    while (mCollisionsRunning)
    {
        //SDL_mutexP(lock);
        //++mTimes;
        SDL_mutexP(mEntityLock);
        //list<Entity*> tempEntities = *mEntities;
        //tempEntities.sort(mFunc);
        mEntities.sort(mFunc);

        list<Entity*>::iterator itFirst = mEntities.begin();
        list<Entity*>::iterator itSecond;
        list<Entity*>::iterator itEnd = mEntities.end();

        while (itFirst != itEnd)
        {
            itSecond = itFirst;
            ++itSecond;

            Entity* first = *itFirst;
            while (itSecond != mEntities.end())
            {
                Entity* second = *itSecond;
                if (abs(first->getPosition()[0] - second->getPosition()[0]) > mLargestRadius)
                {
                    break;
                }

                float distance = (first->getPosition() - second->getPosition()).length();

                if (distance < first->getRadius() + second->getRadius())
                {
                    first->onCollision(second);
                }

                //after we've had one of the two objects deal with it's collision, test to see if there's still a collision
                distance = (first->getPosition() - second->getPosition()).length();

                if (distance < first->getRadius() + second->getRadius())
                {
                    //if there is still a collision, call the second objects collision method
                    second->onCollision(first);
                }

                if (!second->isAlive())
                {
                    itSecond = mEntities.erase(itSecond);
                    second->setGameDead();
                }
                else
                {
                    ++itSecond;
                }
            }

            if (!first->isAlive())
            {
                itFirst = mEntities.erase(itFirst);
                first->setGameDead();
            }
            else
            {
                ++itFirst;
            }
        }
        //SDL_mutexV(lock);
        SDL_mutexV(mEntityLock);
        SDL_Delay(1);
        /*
        for (unsigned int i = 0; i < mEntities.size() - 1; ++i)
        {
            for (unsigned int j = i + 1; j < mEntities.size(); ++j)
            {
                float distance = (mEntities[i]->getPosition() - mEntities[j]->getPosition()).length();

                if (distance < mEntities[i]->getRadius() + mEntities[j]->getRadius())
                {
                    mEntities[i]->onCollision(mEntities[j]);
                }

                //after we've had one of the two objects deal with it's collision, test to see if there's still a collision
                distance = (mEntities[i]->getPosition() - mEntities[j]->getPosition()).length();

                if (distance < mEntities[i]->getRadius() + mEntities[j]->getRadius())
                {
                    //if there is still a collision, call the second objects collision method
                    mEntities[j]->onCollision(mEntities[i]);
                }
            }
        }
        */
    }
    return 0;

}


void CollisionEngine::onUnload()
{
    //mCollisionsRunning = false;
    mEntities.clear();
    SDL_DestroyMutex(mEntityLock);
}
