#include "Tank.h"

Tank::Tank()
{
    //ctor
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
        glScalef(10.0f, 5.0f, 10.0f);
        //glTranslatef(10.0f, 0.0f, 10.0f);
        glBegin(GL_QUADS);
        {
            glVertex3f(-a, -a, a);
            glVertex3f(-a, a, a);
            glVertex3f(a, a, a);
            glVertex3f(a, -a, a);

            glVertex3f(a, -a, a);
            glVertex3f(a, -a, -a);
            glVertex3f(a, a, -a);
            glVertex3f(a, a, a);

            glVertex3f(a, -a, -a);
            glVertex3f(-a, -a, -a);
            glVertex3f(-a, a, -a);
            glVertex3f(a, a, -a);

            glVertex3f(-a, -a, -a);
            glVertex3f(-a, -a, a);
            glVertex3f(-a, a, a);
            glVertex3f(-a, a, -a);

            glVertex3f(-a, a, a);
            glVertex3f(a, a, a);
            glVertex3f(a, a, -a);
            glVertex3f(-a, a, -a);

            glVertex3f(-a, -a, a);
            glVertex3f(a, -a, a);
            glVertex3f(a, -a, -a);
            glVertex3f(-a, -a, -a);
        }
        glEnd();
    }
    glPopMatrix();
}
