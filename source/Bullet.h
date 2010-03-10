#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"


class Bullet : public Entity
{
    public:
        Bullet();
        virtual ~Bullet();

        void onCollision(Entity* inCollidedWith);
        void move();

    protected:
};

#endif // BULLET_H
