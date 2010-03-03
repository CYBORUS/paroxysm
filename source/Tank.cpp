#include "Tank.h"

Tank::Tank(TerrainGrid* inTerrain)
{
    mTerrain = inTerrain;
    mTerrainWidth = mTerrain->getMatrix().cols();
    mTerrainHeight = mTerrain->getMatrix().rows();
    mPosition[0] = mTerrainWidth / 2;
    mPosition[1] = 0.5;
    mPosition[2] = mTerrainHeight / 2;

    mTankSpeed = 0.1f;
    mTankTurnRate = 2.0f;

    mCurrentMoveRate = 0;
    mCurrentRotationRate = 0;

    mFrontLeftControl.set(mPosition[0] - 0.75f, 0.0f, mPosition[2] - 0.75f);
    mFrontRightControl.set(mPosition[0] + 0.75f, 0.0f, mPosition[2] - 0.75f);
    mBackLeftControl.set(mPosition[0] - 0.75f, 0.0f, mPosition[2] + 0.75f);
    mBackRightControl.set(mPosition[0] + 0.75f, 0.0f, mPosition[2] + 0.75f);
}

Tank::~Tank()
{
    //dtor
}


void Tank::display()
{
    float a = 0.5;
    glPushMatrix();
    {
        glPushAttrib(GL_LIGHTING_BIT);
        {
            glEnable(GL_LIGHTING);
            //glDisable(GL_COLOR_MATERIAL);
            glTranslatef(mPosition[0], mPosition[1], mPosition[2]);
            glRotatef(mRotation[1], 0.0f, 1.0f, 0.0f);
            glRotatef(mRotation[0], 1.0f, 0.0f, 0.0f);
            glRotatef(mRotation[2], 0.0f, 0.0f, 1.0f);
            glScalef(1.5f, 1.0f, 1.5f);

            glBegin(GL_QUADS);
            {
                glNormal3f(0.0f, 0.0f, 1.0f);
                glVertex3f(-a, -a, a);
                glVertex3f(a, -a, a);
                glVertex3f(a, a, a);
                glVertex3f(-a, a, a);

                glNormal3f(1.0f, 0.0f, 0.0f);
                glVertex3f(a, -a, a);
                glVertex3f(a, -a, -a);
                glVertex3f(a, a, -a);
                glVertex3f(a, a, a);

                glNormal3f(0.0f, 0.0f, -1.0f);
                glVertex3f(a, -a, -a);
                glVertex3f(-a, -a, -a);
                glVertex3f(-a, a, -a);
                glVertex3f(a, a, -a);

                glNormal3f(-1.0f, 0.0f, 0.0f);
                glVertex3f(-a, -a, -a);
                glVertex3f(-a, -a, a);
                glVertex3f(-a, a, a);
                glVertex3f(-a, a, -a);

                glNormal3f(0.0f, 1.0f, 0.0f);
                glVertex3f(-a, a, a);
                glVertex3f(a, a, a);
                glVertex3f(a, a, -a);
                glVertex3f(-a, a, -a);

                glNormal3f(0.0f, -1.0f, 0.0f);
                glVertex3f(-a, -a, a);
                glVertex3f(a, -a, a);
                glVertex3f(a, -a, -a);
                glVertex3f(-a, -a, -a);
            }
            glEnd();

        }
        glPopAttrib();
    }
    glPopMatrix();
}

Vector3D<float> Tank::getPosition()
{
    return mPosition;
}

void Tank::setPosition(Vector3D<float> inPosition)
{
    mPosition = inPosition;
}

void Tank::move()
{
    mRotation[1] += mCurrentRotationRate;
    changeMovementVector();
    mPosition += mMovementVector;

    if (mPosition[0] < 1)
    {
        mPosition[0] = 1;
    }
    else if (mPosition[0] > mTerrainWidth - 2)
    {
        mPosition[0] = mTerrainWidth - 2;
    }

    if (mPosition[2] < 1)
    {
        mPosition[2] = 1;
    }
    else if (mPosition[2] > mTerrainHeight - 2)
    {
        mPosition[2] = mTerrainHeight - 2;
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
    mMovementVector[0] = sin(TO_RADIANS(mRotation[1])) * mCurrentMoveRate;
    mMovementVector[2] = cos(TO_RADIANS(mRotation[1])) * mCurrentMoveRate;
}


void Tank::setupModelview()
{
    //float xSin = sin(TO_RADIANS(mRotation[0]));
    //float xCos = cos(TO_RADIANS(mRotation[0]));
    float ySin = sin(TO_RADIANS(mRotation[1]));
    float yCos = cos(TO_RADIANS(mRotation[1]));
    //float zSin =

    mRotateY(0, 0) = yCos;
    mRotateY(0, 2) = ySin;
    mRotateY(2, 0) = -ySin;
    mRotateY(2, 2) = yCos;

    mTranslate(0, 3) = mPosition[0];
}

void Tank::transformControlPoints()
{
}
