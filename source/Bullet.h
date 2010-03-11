#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"
#include "TSphere.h"
#include "OGL.h"


class Bullet : public Entity
{
    public:
        Bullet(TerrainGrid* inTerrain, Vector3D<float> inPosition, Vector3D<float> inMomentum, float inRotation);
        virtual ~Bullet();

        virtual void display();

        void onCollision(Entity* inCollidedWith);
        void move();


    protected:
        TSphere mSphere;

        float mSpeed;
};

#endif // BULLET_H
