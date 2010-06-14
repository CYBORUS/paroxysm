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

#include "Tank.h"
#include "ModelStack.h"

Tank::Tank() : mTankSize(1.5, 1.0, 1.5),
    mHeadCenter(0.0, 0.75, 0.0), mHeadSize(0.75, 0.5, 0.75),
    mTurretCenter(0.0, 0.0, 0.75), mTurretSize(0.25, 0.25, 0.75)
{
    //setRadius(0.5);
    mRadius = 0.75;
    mWhatAmI = E_TANK;
    mAlive = true;
    mPosition[0] = mTerrainWidth / 2;
    mPosition[1] = 0.5;
    mPosition[2] = mTerrainHeight / 2;

    mTankSpeed = 0.15f;
    mTankTurnRate = 4.0f;
    mHeadRotationRate = 4.0f;

    mTurretConstantRotate = false;
    mAIRotateCalled = false;

    mCurrentMoveRate = 0;
    mCurrentRotationRate = 0;
    mHeadRotationDirection = 0;
    mHeadTargetDirection = 0;
    mHeadRotation = 0;

    mFrontLeftControl.set(0 + 0.75f, -0.5f, 0 + 0.75f);
    mFrontRightControl.set(0 - 0.75f, -0.5f, 0 + 0.75f);
    mBackLeftControl.set(0 + 0.75f, -0.5f, 0 - 0.75f);
    mBackRightControl.set(0 - 0.75f, -0.5f, 0 - 0.75f);

    float a = 0.5;
    GLfloat baseRect[24] = {-a, a, a, //0 0
                            a, a, a, //1 3
                            a, a, -a, //2 6
                            -a, a, -a, //3 9
                            -a, -a, a, //4 12
                            a, -a, a, //5 15
                            a, -a, -a, //6 18
                            -a, -a, -a}; //7 21

    for (int i = 0; i < 24; ++i)
    {
        mBaseRect[i] = baseRect[i];
    }

    Vector3D<GLfloat> nextNormal;
    GLfloat normals[24];

    //mBaseRectNormals = new GLfloat[24];
    for (int i = 0; i < 24; i += 3)
    {
        nextNormal.set(baseRect[i], baseRect[i + 1], baseRect[i + 2]);
        nextNormal.normalize();
        mBaseRectNormals[i] = nextNormal[0];
        mBaseRectNormals[i + 1] = nextNormal[1];
        mBaseRectNormals[i + 2] = nextNormal[2];

        normals[i] = nextNormal[0];
        normals[i + 1] = nextNormal[1];
        normals[i + 2] = nextNormal[2];

    }

    //mBaseRectNormals = normals;

    mNumIndices = 24;

    GLuint indices[24] = {4, 5, 1, 0,
                         5, 6, 2, 1,
                         6, 7, 3, 2,
                         7, 4, 0, 3,
                         0, 1, 2, 3,
                         4, 7, 6, 5};

    //mBaseRectIndices = new GLuint[mNumIndices];

    for (int i = 0; i < mNumIndices; ++i)
    {
        mBaseRectIndices[i] = indices[i];
    }

    mBody = ModelStack::load("bradley_body.c3m");
    mHead = ModelStack::load("bradley_head.c3m");
    mTurret = ModelStack::load("bradley_turret.c3m");
}

Tank::~Tank()
{
}

void Tank::onCollision(Entity* inCollidedWith)
{
    switch (inCollidedWith->getWhatIAm())
    {
        case E_TANK:
        {
            //Tank* t = (Tank*)inCollidedWith;
            mPosition = mPreviousPosition;

            break;
        }
        case E_BULLET:
        {
            mAlive = false;
            break;
        }

        default:
        {
        }
    }
}


void Tank::display()
{
    //float a = 0.5;
    glPushMatrix();
    {
        //glDisable(GL_COLOR_MATERIAL);
        glTranslatef(mPosition[0], mPosition[1], mPosition[2]);
        glRotatef(mRotation[1], 0.0f, 1.0f, 0.0f);
        glRotatef(mRotation[0], 1.0f, 0.0f, 0.0f);
        glRotatef(mRotation[2], 0.0f, 0.0f, 1.0f);

        glPushMatrix();
        {
            mBody->display();
        }
        glPopMatrix();

        //glTranslatef(mHeadCenter[0], mHeadCenter[1], mHeadCenter[2]);
        glRotatef(mHeadRotation - mRotation[1], 0.0f, 1.0f, 0.0f);

        glPushMatrix();
        {
            mHead->display();
        }
        glPopMatrix();

        //glTranslatef(mTurretCenter[0], mTurretCenter[1], mTurretCenter[2]);

        glPushMatrix();
        {
            mTurret->display();
        }
        glPopMatrix();
    }
    glPopMatrix();

}

void Tank::rotateTurret(float inRotation)
{
    mAIRotateCalled = true;
    mHeadTargetDirection = inRotation;

    if (mHeadTargetDirection >= mHeadRotation)
    {
        mHeadRotationDirection = mHeadRotationRate;
    }
    else
    {
        mHeadRotationDirection = -mHeadRotationRate;
    }
}

void Tank::setTurretDirection(float inRotation)
{
    mHeadRotation = inRotation;
}

void Tank::modifyTurretRotation(bool inRotate, float inRotation)
{
    mTurretConstantRotate = inRotate;

    mHeadRotationDirection = mHeadRotationRate;

    if (inRotation < 0)
    {
        mHeadRotationDirection = -mHeadRotationRate;
    }
}

Vector3D<float> Tank::getControlPoint()
{
    return mTransformedFrontLeftControl;
}


/**************************************************
*   Calculates one step for the tank based on its
*   movement and rotation vectors.  It then works out
*   how that movement affects its orientation on the
*   terrain and runs it against the collision engine.
*
*   This function should be written as procedurally as possible
*   because it is called every frame for every tank.
**************************************************/
void Tank::move()
{
    mRotation[1] += mCurrentRotationRate;

    float ySin = sin(TO_RADIANS(mRotation[1]));
    float yCos = cos(TO_RADIANS(mRotation[1]));

    Vector3D<float> driveMomentum;

    driveMomentum[0] = ySin * mCurrentMoveRate;
    driveMomentum[2] = yCos * mCurrentMoveRate;

    Vector3D<float> newPosition = mPosition + driveMomentum;


    if (mTurretConstantRotate)
    {
        mHeadRotation += mHeadRotationDirection;
    }

    if (mAIRotateCalled)
    {
        if (abs(mHeadRotation - mHeadTargetDirection) < mHeadRotationRate)
        {
            mHeadRotation = mHeadTargetDirection;
            mHeadTargetDirection = mHeadRotation;
        }
        else
        {
            mHeadRotation += mHeadRotationDirection;
            mHeadTargetDirection -= mHeadRotationRate;
        }
    }


    //transform the four control points to determine how the tank should sit
    //on the terrrain
    mTransformedBackLeftControl = mBackLeftControl;
    mTransformedBackRightControl = mBackRightControl;
    mTransformedFrontLeftControl = mFrontLeftControl;
    mTransformedFrontRightControl = mFrontRightControl;


    mTransformedFrontLeftControl[0] = mFrontLeftControl[0] * yCos + mFrontLeftControl[2] * ySin;
    mTransformedFrontLeftControl[2] = mFrontLeftControl[0] * -ySin + mFrontLeftControl[2] * yCos;
    mTransformedFrontLeftControl[0] += newPosition[0];
    mTransformedFrontLeftControl[2] += newPosition[2];
    mTransformedFrontLeftControl[1]
        += mTerrain->findHeight(mTransformedFrontLeftControl[0],
        mTransformedFrontLeftControl[2]) + mTankSize[1] / 2.0;
    float highestControlPoint = mTransformedFrontLeftControl[1];
    float lowestControlPoint = mTransformedFrontLeftControl[1];

    mTransformedFrontRightControl[0] = mFrontRightControl[0] * yCos + mFrontRightControl[2] * ySin;
    mTransformedFrontRightControl[2] = mFrontRightControl[0] * -ySin + mFrontRightControl[2] * yCos;
    mTransformedFrontRightControl[0] += newPosition[0];
    mTransformedFrontRightControl[2] += newPosition[2];
    mTransformedFrontRightControl[1]
        += mTerrain->findHeight(mTransformedFrontRightControl[0],
        mTransformedFrontRightControl[2]) + mTankSize[1] / 2.0;

    if (mTransformedFrontRightControl[1] > highestControlPoint)
    {
        highestControlPoint = mTransformedFrontRightControl[1];
    }
    else if (mTransformedFrontRightControl[1] < lowestControlPoint)
    {
        lowestControlPoint = mTransformedFrontRightControl[1];
    }

    mTransformedBackLeftControl[0] = mBackLeftControl[0] * yCos
        + mBackLeftControl[2] * ySin;
    mTransformedBackLeftControl[2] = mBackLeftControl[0] * -ySin
        + mBackLeftControl[2] * yCos;
    mTransformedBackLeftControl[0] += newPosition[0];
    mTransformedBackLeftControl[2] += newPosition[2];
    mTransformedBackLeftControl[1]
        += mTerrain->findHeight(mTransformedBackLeftControl[0],
        mTransformedBackLeftControl[2]) + mTankSize[1] / 2.0;

    if ( mTransformedBackLeftControl[1] > highestControlPoint)
    {
        highestControlPoint = mTransformedBackLeftControl[1];
    }
    else if (mTransformedBackLeftControl[1] < lowestControlPoint)
    {
        lowestControlPoint = mTransformedBackLeftControl[1];
    }

    mTransformedBackRightControl[0] = mBackRightControl[0] * yCos + mBackRightControl[2] * ySin;
    mTransformedBackRightControl[2] = mBackRightControl[0] * -ySin + mBackRightControl[2] * yCos;
    mTransformedBackRightControl[0] += newPosition[0];
    mTransformedBackRightControl[2] += newPosition[2];
    mTransformedBackRightControl[1]
        += mTerrain->findHeight(mTransformedBackRightControl[0],
        mTransformedBackRightControl[2]) + mTankSize[1] / 2.0;

    if (mTransformedBackRightControl[1] > highestControlPoint)
    {
        highestControlPoint = mTransformedBackRightControl[1];
    }
    else if (mTransformedBackRightControl[1] < lowestControlPoint)
    {
        lowestControlPoint = mTransformedBackRightControl[1];
    }

    //use the transformed control points to determine what angle the tank should sit at
    float zFront = atan((mTransformedFrontLeftControl[1] - mTransformedFrontRightControl[1]) / mTankSize[0]);
    float zBack = atan((mTransformedBackLeftControl[1] - mTransformedBackRightControl[1]) / mTankSize[0]);
    mRotation[2] = (abs(zFront) > abs(zBack)) ? TO_DEGREES(zFront)
        : TO_DEGREES(zBack);

    float xLeft = atan(( mTransformedBackLeftControl[1] - mTransformedFrontLeftControl[1]) / mTankSize[0]);
    float xRight = atan(( mTransformedBackRightControl[1] - mTransformedFrontRightControl[1]) / mTankSize[0]);
    mRotation[0] = (abs(xLeft) > abs(xRight)) ? TO_DEGREES(xLeft)
        : TO_DEGREES(xRight);

    //set the position to the highest of the four control points
    mPosition[1] = (highestControlPoint + lowestControlPoint) / 2.0
        + mTankSize[1] / 2.0;

    mPreviousPosition = mPosition;

    float friction = mTerrain->getFriction(mPosition[0], mPosition[2]);
    float cmr = fabs(mCurrentMoveRate);

    driveMomentum *= friction;
    mMomentum *= (1.0f - friction);
    if (mMomentum.isZero()) mMomentum.set(0.0f);

    float magnitude = mMomentum.length();
    mMomentum += driveMomentum;
    if (mMomentum.length() > cmr)
    {
        mMomentum.normalizeTo(magnitude > cmr ? magnitude : cmr);
    }

    mPosition += mMomentum;

    if (mPosition[0] < 1.25)
    {
        mPosition[0] = 1.25;
    }
    else if (mPosition[0] > mTerrainWidth - 2.25)
    {
        mPosition[0] = mTerrainWidth - 2.25;
    }

    if (mPosition[2] < 1.25)
    {
        mPosition[2] = 1.25;
    }
    else if (mPosition[2] > mTerrainHeight - 2.25)
    {
        mPosition[2] = mTerrainHeight - 2.25;
    }

}

/******************************************************
*   Tells the tank which way to turn
*
*   inDirection: a float between -1.0 and 1.0 indicating
*       turn direction and magnitude relative to the
*       tanks maximum turn rate.
*******************************************************/
void Tank::changeDirection(float inDirection)
{
    mCurrentRotationRate = inDirection * mTankTurnRate;
    //changeMovementVector();
}


void Tank::changeSpeed(float inSpeed)
{
    mCurrentMoveRate = inSpeed * mTankSpeed;
    //changeMovementVector();
}

void Tank::changeMovementVector()
{
    mMomentum[0] = sin(TO_RADIANS(mRotation[1])) * mCurrentMoveRate;
    mMomentum[2] = cos(TO_RADIANS(mRotation[1])) * mCurrentMoveRate;
}

Vector3D<float> Tank::getBulletDirection()
{
    float ySin = sin(TO_RADIANS(mHeadRotation));
    float yCos = cos(TO_RADIANS(mHeadRotation));

    Vector3D<float> direction(ySin, 0.0f, yCos);
    return direction;
}

Vector3D<float> Tank::getBulletStart()
{
    float ySin = sin(TO_RADIANS(mHeadRotation));
    float yCos = cos(TO_RADIANS(mHeadRotation));

    float oldZ = mTurretCenter[2] + mTurretSize[2];
    float x = oldZ * ySin;
    float z = oldZ * yCos;

    x += mPosition[0];
    z += mPosition[2];

    Vector3D<float> position(x, mPosition[1] + mHeadCenter[1], z);

    return position;
}

float Tank::getBulletRotation()
{
    return mHeadRotation;
}
