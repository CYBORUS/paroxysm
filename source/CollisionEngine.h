#ifndef COLLISIONENGINE_H
#define COLLISIONENGINE_H

#include "CGE/Entity.h"


class CollisionEngine
{
    public:
        CollisionEngine();
        virtual ~CollisionEngine();
        void AddEntity(CGE::Entity* inEntity);
        void removeEntity(CGE::Entity* inEntity);
        void checkForCollisions();

    protected:
    private:
        std::list<CGE::Entity*> mEntities;
};

#endif // COLLISIONENGINE_H
