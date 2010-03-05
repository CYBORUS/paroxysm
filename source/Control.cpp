/**
 *  This file is part of "Paroxysm".
 *
 *  "Paroxysm" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Paroxysm" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with "Paroxysm".  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Control.h"

Control::Control(Tank* inTank)
{
    mTank = inTank;

    mHeadRotationRate = 4.0f;
    mHeadRotationDirection = 0;
    mHeadTargetDirection = 0;
}

Control::~Control()
{
    //dtor
}

void Control::setTurretRotation(float inRotation)
{
    mHeadTargetDirection = inRotation;

    if (mHeadTargetDirection >= mHeadRotation)
    {
        mHeadRotationDirection = mHeadRotationRate;
    }
    else
    {
        mHeadRotationDirection = -mHeadRotationRate;
    }
}

void Control::stepTurretRotation()
{
    if ( abs(mHeadRotation - mHeadTargetDirection) < mHeadRotationRate)
    {
        mHeadRotation = mHeadTargetDirection;
        mHeadTargetDirection = 0;
    }
    else
    {
        mHeadRotation += mHeadRotationDirection;
        mHeadTargetDirection -= mHeadRotationRate;
    }

    mTank->rotateTurret(mHeadRotation);
}

void Control::setPosition(Vector3D<float> inPosition)
{
    mTank->setPosition(inPosition);
}

void Control::changeDirection(float inDirection)
{
    mTank->changeDirection(inDirection);
}

void Control::changeSpeed(float inSpeed)
{
    mTank->changeSpeed(inSpeed);
}
