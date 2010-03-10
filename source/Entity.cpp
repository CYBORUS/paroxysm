#include "Entity.h"

Entity::Entity()
{
    mRadius = 1;
}

Entity::~Entity()
{
}


void Entity::setRadius(float inRadius)
{
    mRadius = inRadius;
}

float Entity::getRadius()
{
    return mRadius;
}

Vector3D<float> Entity::getPosition()
{
    return mPosition;
}

void Entity::setPosition(const Vector3D<float>& inPosition)
{
    mPosition = inPosition;
}

EntityType Entity::getWhatIAm()
{
    return mWhatAmI;
}
