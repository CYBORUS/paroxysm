#include "Control.h"

Control::Control(Tank* inTank)
{
    mTank = inTank;
}

Control::~Control()
{
    //dtor
}

void Control::move(Vector3D<float> inMoveDirection)
{
    mMovementVector = inMoveDirection;
}
