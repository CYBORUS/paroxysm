#include "Tank.h"

Tank::Tank(TerrainGrid* inTerrain) : mTankSize(1.5, 1.0, 1.5), mHeadCenter(0.0, 0.75, 0.0),
    mHeadSize(0.75, 0.5, 0.75), mTurretCenter(0.0, 0.0, 0.75), mTurretSize(0.25, 0.25, 0.75)
{
    mTerrain = inTerrain;
    mTerrainWidth = mTerrain->getMatrix().cols();
    mTerrainHeight = mTerrain->getMatrix().rows();
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

    mSphere.setScale(0.1, 0.1, 0.1);
}

Tank::~Tank()
{
    //dtor
}


void Tank::display()
{
    float a = 0.5;
    glPushAttrib(GL_LIGHTING_BIT);
    {
        glPushAttrib(GL_POLYGON_BIT);
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            //glEnable(GL_LIGHTING);
            glDisable(GL_LIGHTING);

            glPushMatrix();
            {
                //glDisable(GL_COLOR_MATERIAL);
                glTranslatef(mPosition[0], mPosition[1], mPosition[2]);
                glRotatef(mRotation[1], 0.0f, 1.0f, 0.0f);
                glRotatef(mRotation[0], 1.0f, 0.0f, 0.0f);
                glRotatef(mRotation[2], 0.0f, 0.0f, 1.0f);

                glPushMatrix();
                {
                    glScalef(mTankSize[0], mTankSize[1], mTankSize[2]);

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
                        glVertex3f(-a, -a, -a);
                        glVertex3f(a, -a, -a);
                        glVertex3f(a, -a, a);
                    }
                    glEnd();
                }
                glPopMatrix();

                glTranslatef(mHeadCenter[0], mHeadCenter[1], mHeadCenter[2]);
                glRotatef(mHeadRotation - mRotation[1], 0.0f, 1.0f, 0.0f);

                glPushMatrix();
                {
                    glScalef(mHeadSize[0], mHeadSize[1], mHeadSize[2]);
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
                        glVertex3f(-a, -a, -a);
                        glVertex3f(a, -a, -a);
                        glVertex3f(a, -a, a);
                    }
                    glEnd();
                }
                glPopMatrix();

                glTranslatef(mTurretCenter[0], mTurretCenter[1], mTurretCenter[2]);

                glPushMatrix();
                {
                    glScalef(mTurretSize[0], mTurretSize[1], mTurretSize[2]);
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
                        glVertex3f(-a, -a, -a);
                        glVertex3f(a, -a, -a);
                        glVertex3f(a, -a, a);
                    }
                    glEnd();
                }
                glPopMatrix();
            }
            glPopMatrix();

/*
            mSphere.moveSphere(mTransformedFrontLeftControl[0], mTransformedFrontLeftControl[1], mTransformedFrontLeftControl[2]);
            //glTranslatef(mTransformedFrontLeftControl[0], mTransformedFrontLeftControl[1], mTransformedFrontLeftControl[2]);
            //glRotatef(mRotation[1], 0.0f, 1.0f, 0.0f);
            //glScalef(10.0f, 10.0f, 10.0f);
            mSphere.setColor(1.0, 1.0, 1.0);
            mSphere.display();

            mSphere.moveSphere(mTransformedFrontRightControl[0], mTransformedFrontRightControl[1], mTransformedFrontRightControl[2]);
            //glTranslatef(mTransformedFrontRightControl[0], mTransformedFrontRightControl[1], mTransformedFrontRightControl[2]);
            //glRotatef(mRotation[1], 0.0f, 1.0f, 0.0f);
            //glScalef(10.0f, 10.0f, 10.0f);
            mSphere.setColor(1.0f, 0.0f, 0.0f);
            mSphere.display();

            mSphere.moveSphere(mTransformedBackLeftControl[0], mTransformedBackLeftControl[1], mTransformedBackLeftControl[2]);
            //glTranslatef(mTransformedBackLeftControl[0], mTransformedBackLeftControl[1], mTransformedBackLeftControl[2]);
            //glRotatef(mRotation[1], 0.0f, 1.0f, 0.0f);
            //glScalef(10.0f, 10.0f, 10.0f);
            mSphere.setColor(0.0, 1.0, 0.0);
            mSphere.display();

            mSphere.moveSphere(mTransformedBackRightControl[0], mTransformedBackRightControl[1], mTransformedBackRightControl[2]);
            //glTranslatef(mTransformedBackRightControl[0], mTransformedBackRightControl[1], mTransformedBackRightControl[2]);
            //glRotatef(mRotation[1], 0.0f, 1.0f, 0.0f);
            //glScalef(10.0f, 10.0f, 10.0f);
            mSphere.setColor(0.0, 0.0, 1.0);
            mSphere.display();
*/
        }
        glPopAttrib();
    }
    glPopAttrib();
    //cerr << "transformed front left: " << mTransformedFrontLeftControl << endl;
}

Vector3D<float> Tank::getPosition()
{
    return mPosition;
}

void Tank::setPosition(Vector3D<float> inPosition)
{
    mPosition = inPosition;
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

    mMovementVector[0] = ySin * mCurrentMoveRate;
    mMovementVector[2] = yCos * mCurrentMoveRate;

    Vector3D<float> newPosition = mPosition + mMovementVector;


    //changeMovementVector();

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


    //transformControlPoints();

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
    mTransformedFrontLeftControl[1] += mTerrain->findHeight(mTransformedFrontLeftControl[0], mTransformedFrontLeftControl[2]) + mTankSize[1] / 2.0;
    float highestControlPoint = mTransformedFrontLeftControl[1];
    float lowestControlPoint = mTransformedFrontLeftControl[1];


    mTransformedFrontRightControl[0] = mFrontRightControl[0] * yCos + mFrontRightControl[2] * ySin;
    mTransformedFrontRightControl[2] = mFrontRightControl[0] * -ySin + mFrontRightControl[2] * yCos;
    mTransformedFrontRightControl[0] += newPosition[0];
    mTransformedFrontRightControl[2] += newPosition[2];
    mTransformedFrontRightControl[1] += mTerrain->findHeight(mTransformedFrontRightControl[0], mTransformedFrontRightControl[2]) + mTankSize[1] / 2.0;
    if (mTransformedFrontRightControl[1] > highestControlPoint)
    {
        highestControlPoint = mTransformedFrontRightControl[1];
    }
    else if (mTransformedFrontRightControl[1] < lowestControlPoint)
    {
        lowestControlPoint = mTransformedFrontRightControl[1];
    }

    mTransformedBackLeftControl[0] = mBackLeftControl[0] * yCos + mBackLeftControl[2] * ySin;
    mTransformedBackLeftControl[2] = mBackLeftControl[0] * -ySin + mBackLeftControl[2] * yCos;
    mTransformedBackLeftControl[0] += newPosition[0];
    mTransformedBackLeftControl[2] += newPosition[2];
    mTransformedBackLeftControl[1] += mTerrain->findHeight(mTransformedBackLeftControl[0], mTransformedBackLeftControl[2]) + mTankSize[1] / 2.0;
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
    mTransformedBackRightControl[1] += mTerrain->findHeight(mTransformedBackRightControl[0], mTransformedBackRightControl[2]) + mTankSize[1] / 2.0;
    if ( mTransformedBackRightControl[1] > highestControlPoint)
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
    mRotation[2] = (abs(zFront) > abs(zBack)) ? TO_DEGREES(zFront) : TO_DEGREES(zBack);

    float xLeft = atan(( mTransformedBackLeftControl[1] - mTransformedFrontLeftControl[1]) / mTankSize[0]);
    float xRight = atan(( mTransformedBackRightControl[1] - mTransformedFrontRightControl[1]) / mTankSize[0]);
    mRotation[0] = (abs(xLeft) > abs(xRight)) ? TO_DEGREES(xLeft) : TO_DEGREES(xRight);
    //cerr << "mRotation: " << mRotation << endl;

    //set the position to the highest of the four control points
    //mPosition[1] = mTerrain->findHeight(mPosition[0], mPosition[2]) + mTankSize.y / 2.0;
    mPosition[1] = (highestControlPoint + lowestControlPoint) / 2.0 + mTankSize[1] / 2.0;



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

/*
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
    //mTranslate(1, 3) = 0.5;
    mTranslate(1, 3) = 0.5;//mTerrain->findHeight(mPosition[0], mPosition[2]) + 0.5;
    mTranslate(2, 3) = mPosition[2];

    //cerr << "mTranslate: \n" << mTranslate << endl;
    //cerr << "mRotateY: \n" << mRotateY << endl;
    //exit(4);

    mModelview = mTranslate * mRotateY;
}
*/

void Tank::transformControlPoints()
{
    //setupModelview();
    //Vector3D<float> mDefault;

    mTransformedBackLeftControl = mBackLeftControl;
    mTransformedBackRightControl = mBackRightControl;
    mTransformedFrontLeftControl = mFrontLeftControl;
    mTransformedFrontRightControl = mFrontRightControl;

    float ySin = sin(TO_RADIANS(mRotation[1]));
    float yCos = cos(TO_RADIANS(mRotation[1]));

    mTransformedFrontLeftControl[0] = mFrontLeftControl[0] * yCos + mFrontLeftControl[2] * ySin;
    mTransformedFrontLeftControl[2] = mFrontLeftControl[0] * -ySin + mFrontLeftControl[2] * yCos;
    mTransformedFrontLeftControl[0] += mPosition[0];
    mTransformedFrontLeftControl[2] += mPosition[2];
    mTransformedFrontLeftControl[1] += mTerrain->findHeight(mTransformedFrontLeftControl[0], mTransformedFrontLeftControl[2]) + 0.5;

    mTransformedFrontRightControl[0] = mFrontRightControl[0] * yCos + mFrontRightControl[2] * ySin;
    mTransformedFrontRightControl[2] = mFrontRightControl[0] * -ySin + mFrontRightControl[2] * yCos;
    mTransformedFrontRightControl[0] += mPosition[0];
    mTransformedFrontRightControl[2] += mPosition[2];
    mTransformedFrontRightControl[1] += mTerrain->findHeight(mTransformedFrontRightControl[0], mTransformedFrontRightControl[2]) + 0.5;

    mTransformedBackLeftControl[0] = mBackLeftControl[0] * yCos + mBackLeftControl[2] * ySin;
    mTransformedBackLeftControl[2] = mBackLeftControl[0] * -ySin + mBackLeftControl[2] * yCos;
    mTransformedBackLeftControl[0] += mPosition[0];
    mTransformedBackLeftControl[2] += mPosition[2];
    mTransformedBackLeftControl[1] += mTerrain->findHeight(mTransformedBackLeftControl[0], mTransformedBackLeftControl[2]) + 0.5;

    mTransformedBackRightControl[0] = mBackRightControl[0] * yCos + mBackRightControl[2] * ySin;
    mTransformedBackRightControl[2] = mBackRightControl[0] * -ySin + mBackRightControl[2] * yCos;
    mTransformedBackRightControl[0] += mPosition[0];
    mTransformedBackRightControl[2] += mPosition[2];
    mTransformedBackRightControl[1] += mTerrain->findHeight(mTransformedBackRightControl[0], mTransformedBackRightControl[2]) + 0.5;


}
