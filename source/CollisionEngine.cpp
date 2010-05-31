#include "CollisionEngine.h"

list<Entity*> CollisionEngine::mEntities;
float CollisionEngine::mLargestRadius = 0.0f;
Functor CollisionEngine::mFunc;
volatile bool CollisionEngine::mCollisionsRunning = false;
SDL_mutex* CollisionEngine::mEntityLock;

void CollisionEngine::onSetup()
{
    mEntityLock = SDL_CreateMutex();
}


void CollisionEngine::addEntity(Entity* inEntity)
{
    mEntities.push_back(inEntity);
    if (inEntity->getRadius() > mLargestRadius)
    {
        mLargestRadius = inEntity->getRadius() * 3.0f;
    }
}

void CollisionEngine::removeEntity(Entity* inEntity)
{
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
}


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
}

int CollisionEngine::checkCollisions(void* unused)
{
    mCollisionsRunning = true;

    while (mCollisionsRunning)
    {
        mEntities.sort(mFunc);

        list<Entity*>::iterator itFirst = mEntities.begin();
        list<Entity*>::iterator itSecond;
        list<Entity*>::iterator itEnd = mEntities.end();

        for (; itFirst != itEnd; ++itFirst)
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
    mEntities.clear();
    SDL_DestroyMutex(mEntityLock);
}
