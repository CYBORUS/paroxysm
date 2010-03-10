#include "CollisionEngine.h"

vector<Entity*> CollisionEngine::mEntities;

void CollisionEngine::addEntity(Entity* inEntity)
{
    mEntities.push_back(inEntity);
}

void CollisionEngine::checkCollisions()
{
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
}
