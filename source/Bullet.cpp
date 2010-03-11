/**
 *  This file is part of "Paroxysm".
 *
 *  "Paroxysm" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Paroxysm" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with "Paroxysm".  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Bullet.h"

//TSphere Bullet::mSphere;

Bullet::Bullet(TerrainGrid* inTerrain, const Vector3D<float>& inPosition,
    const Vector3D<float>& inMomentum, float inRotation) : Entity(inTerrain)
{
    mWhatAmI = E_BULLET;
    mRadius = 0.5f;

    mSpeed = 1.8f;
    mMomentum = inMomentum;
    mMomentum.normalizeTo(mSpeed);
    mPosition = inPosition;
    mRotation[1] = inRotation;
    mAlive = true;
}

Bullet::~Bullet()
{
    //dtor
}

void Bullet::onCollision(Entity* inCollidedWith)
{
    if (inCollidedWith->getWhatIAm() == E_TANK)
    {
        mAlive = false;
    }
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
        glScalef(0.25f, 0.25f, 1.0f);

        mSphere.display();
    }
    glPopMatrix();

}

