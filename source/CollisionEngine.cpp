#include "CollisionEngine.h"

CollisionEngine::CollisionEngine()
{
}
/** @brief checkForCollisions
  *
  * @todo: document this function
  */
void CollisionEngine::checkForCollisions()
{
    for (std::list<CGE::Entity*>::iterator i = mEntities.begin();
         i != mEntities.end(); ++i)
    {
         std::list<CGE::Entity*>::iterator j = i;
         ++j;
         for ( ;j != mEntities.end(); ++j)
         {
          CGE::Entity* e1 = *i;
          CGE::Entity* e2 = *j;
          if (e1.isInRangeOf(e2))

         }
    }
}

/** @brief removeEntity
  *
  * @todo: document this function
  */
void CollisionEngine::removeEntity(CGE::Entity* inEntity)
{
    mEntities.remove(inEntity);
}

/** @brief AddEntity
  *
  * @todo: document this function
  */
void CollisionEngine::AddEntity(CGE::Entity* inEntity)
{
    mEntities.push_back(inEntity);
}


CollisionEngine::~CollisionEngine()
{
    //dtor
}
