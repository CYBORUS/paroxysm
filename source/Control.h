#ifndef CONTROL_H
#define CONTROL_H

#include "Vector3D.h"
#include "Tank.h"

enum ControlType { PLAYER_TANK, NETWORK_TANK };

class Control
{
    public:
        Control(Tank* inTank);
        virtual ~Control();

        virtual void update() = 0;
        virtual void move(Vector3D<float> inMoveDirection);

    protected:
        Tank* mTank;
        Vector3D<float> mMovementVector;
        Vector3D<float> mRotationVector;
};

#endif // CONTROL_H
