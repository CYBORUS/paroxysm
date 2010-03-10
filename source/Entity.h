#ifndef ENTITY_H
#define ENTITY_H

#include "Vector3D.h"

enum EntityType { E_TANK, E_BULLET };

class Entity
{
    public:
        Entity();
        virtual ~Entity();

        virtual void onCollision(Entity* inCollidedWith) = 0;

        void setRadius(float inRadius);
        float getRadius();

        Vector3D<float> getPosition();
        void setPosition(const Vector3D<float>& inPosition);

        EntityType getWhatIAm();
    protected:
        EntityType mWhatAmI;
        float mRadius;
        Vector3D<float> mPosition;

        //for remembering where we were in case of collisions
        Vector3D<float> mPreviousPosition;
};

#endif // ENTITY_H
