#include "Tank.h"

Tank::Tank()
{
    mPosition[0] = 10.0;
    mPosition[1] = 0.5;
    mPosition[2] = 10.0;

    mTankSpeed = 0.1f;
    mTankTurnRate = 2.0f;
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

void Tank::move()
{
    mRotation[1] += mCurrentRotationRate;
    changeMovementVector();
    mPosition += mMovementVector;
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
