#ifndef TANK_H
#define TANK_H

#include <SDL_opengl.h>

#include "Vector3D.h"

#include <iostream>
#include <cstdlib>
using namespace std;

class Tank
{
    public:
        Tank();
        virtual ~Tank();

        virtual void display();
        Vector3D<float> getPosition();
        void changeDirection(float inDirection);
        void changeSpeed(float inSpeed);

        void move();

    protected:
        void changeMovementVector();

        Vector3D<float> mPosition;
        Vector3D<float> mRotation;
        Vector3D<float> mMovementVector;

        float mTurretAngle;

        float mTankSpeed;
        float mTankTurnRate;

        float mCurrentMoveRate;
        float mCurrentRotationRate;
};

#endif // TANK_H
