#include "PlayerControl.h"

PlayerControl::PlayerControl(Tank* inTank) : Control(inTank)
{
    //ctor
}

PlayerControl::~PlayerControl()
{
    //dtor
}

void PlayerControl::update()
{
    mTank->move(mMovementVector);
}
