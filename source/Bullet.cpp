#include "Bullet.h"

Bullet::Bullet(TerrainGrid* inTerrain, Vector3D<float> inPosition, Vector3D<float> inMomentum) : Entity(inTerrain)
{
    mWhatAmI = E_BULLET;
    mRadius = 0.5f;

    mSpeed = 0.8f;
    mMomentum = inMomentum;
    mMomentum.normalizeTo(mSpeed);
    mPosition = inPosition;
    mAlive = true;
}

Bullet::~Bullet()
{
    //dtor
}

void Bullet::onCollision(Entity* inCollidedWith)
{
}

void Bullet::move()
{
    mPosition += mMomentum;

    if (mPosition[0] >= mTerrainWidth || mPosition[2] >= mTerrainHeight ||
        mPosition[0] <= 0 || mPosition[2] <= 0)
    {
        mAlive = false;
    }
}

void Bullet::display()
{
    //mSphere.moveSphere(mPosition[0], mPosition[1], mPosition[2]);

    glPushMatrix();
    {
        glTranslatef(mPosition[0], mPosition[1], mPosition[2]);
        glRotatef(mRotation[1], 0.0f, 1.0f, 0.0f);
        glRotatef(mRotation[0], 1.0f, 0.0f, 0.0f);
        glRotatef(mRotation[2], 0.0f, 0.0f, 1.0f);
        glScalef(0.5f, 0.5f, 0.5f);

        mSphere.display();
    }
    glPopMatrix();

}

bool Bullet::isAlive()
{
    return mAlive;
}
