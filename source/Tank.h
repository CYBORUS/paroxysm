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
        Vector3D<float> getPosition();

        void move(Vector3D<float> inMoveDirection);

    protected:

        Vector3D<float> mPosition;
        Vector3D<float> mRotation;

        float mTurretAngle;

        float mTankSpeed;
};

#endif // TANK_H
