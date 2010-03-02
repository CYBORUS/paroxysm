#include "Tank.h"

Tank::Tank()
{
    mPosition[0] = 10.0;
    mPosition[1] = 0.5;
    mPosition[2] = 10.0;

    mTankSpeed = 0.05f;
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

void Tank::move(Vector3D<float> inMoveDirection)
{
    inMoveDirection.normalize();

    inMoveDirection = inMoveDirection * mTankSpeed;

    mPosition += inMoveDirection;
}
