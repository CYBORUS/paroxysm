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
        virtual void changeDirection(float inDirection);
        virtual void changeSpeed(float inSpeed);

    protected:
        Tank* mTank;
};

#endif // CONTROL_H
