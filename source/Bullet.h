#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"
#include "TSphere.h"
#include "OGL.h"


class Bullet : public Entity
{
    public:
        Bullet(TerrainGrid* inTerrain, Vector3D<float> inPosition, Vector3D<float> inMomentum);
        virtual ~Bullet();

        virtual void display();

        void onCollision(Entity* inCollidedWith);
        void move();

        bool isAlive();

    protected:
        TSphere mSphere;

        bool mAlive;
        float mSpeed;
};

#endif // BULLET_H
