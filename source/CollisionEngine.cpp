#include "CollisionEngine.h"

list<Entity*> CollisionEngine::mEntities;
float CollisionEngine::mLargestRadius = 0.0f;
Functor CollisionEngine::mFunc;


void CollisionEngine::onSetup()
{
    mEntities.sort(mFunc);
}


void CollisionEngine::addEntity(Entity* inEntity)
{
    mEntities.push_back(inEntity);
    if (inEntity->getRadius() > mLargestRadius)
    {
        mLargestRadius = inEntity->getRadius() * 2.0f;
    }
    /*
    mEntities.sort(mFunc);

    int i = 0;

    for (list<Entity*>::iterator it = mEntities.begin(); it != mEntities.end(); ++it, ++i)
    {
        cerr << i << ": " << (*it)->getPosition()[0] << endl;
    }
    cerr << endl << endl;
    */
}

void CollisionEngine::checkCollisions()
{
    mEntities.sort(mFunc);

    list<Entity*>::iterator itFirst = mEntities.begin();
    list<Entity*>::iterator itSecond;// = itFirst;
    //++itSecond;

    for (; itFirst != mEntities.end(); ++itFirst)
    {
        itSecond = itFirst;
        ++itSecond;

        for (; itSecond != mEntities.end(); ++itSecond)
        {
            if (abs((*itFirst)->getPosition()[0] - (*itSecond)->getPosition()[0]) > mLargestRadius)
            {
                break;
            }

            float distance = ((*itFirst)->getPosition() - (*itSecond)->getPosition()).length();

            if (distance < (*itFirst)->getRadius() + (*itSecond)->getRadius())
            {
                (*itFirst)->onCollision((*itSecond));
            }

            //after we've had one of the two objects deal with it's collision, test to see if there's still a collision
            distance = ((*itFirst)->getPosition() - (*itSecond)->getPosition()).length();

            if (distance < (*itFirst)->getRadius() + (*itSecond)->getRadius())
            {
                //if there is still a collision, call the second objects collision method
                (*itSecond)->onCollision((*itFirst));
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

void CollisionEngine::onCleanup()
{
    mEntities.clear();
}
