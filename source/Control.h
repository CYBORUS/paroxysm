#ifndef CONTROL_H
#define CONTROL_H

#include "Vector3D.h"
#include "Tank.h"

class Control
{
    public:
        Control(Tank* inTank);
        virtual ~Control();

        virtual void update() = 0;

    protected:
        Tank* mTank;
        Vector3D<float> mMovementVector;
        Vector3D<float> mRotationVector;
};

#endif // CONTROL_H
