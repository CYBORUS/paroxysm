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
        glPushAttrib(GL_LIGHTING_BIT);
        {
            glEnable(GL_LIGHTING);
            //glDisable(GL_COLOR_MATERIAL);
            glTranslatef(10.0f, 0.5f, 10.0f);
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
