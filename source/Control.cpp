#include "Control.h"

Control::Control(Tank* inTank)
{
    mTank = inTank;
}

Control::~Control()
{
    //dtor
}

void Control::changeDirection(float inDirection)
{
    mTank->changeDirection(inDirection);
}

void Control::changeSpeed(float inSpeed)
{
    mTank->changeSpeed(inSpeed);
}
