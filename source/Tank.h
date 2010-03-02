#ifndef TANK_H
#define TANK_H

#include <SDL_opengl.h>

#include "Vector3D.h"

class Tank
{
    public:
        Tank();
        virtual ~Tank();

        virtual void display();


    protected:


        float mRotateX;
        float mRotateY;
        float mRotateZ;

        float turretAngle;
};

#endif // TANK_H
