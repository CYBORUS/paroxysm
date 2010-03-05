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

#include "RobotControl.h"

#include <ctime>

boost::mt19937 RobotControl::mRng(time(NULL));

RobotControl::RobotControl(Tank* inTank) : Control(inTank), mTicks(1),
    mTurn(0.0f), mSpeed(0.0f)
{
}

RobotControl::~RobotControl()
{
}

void RobotControl::update()
{
    --mTicks;

    if (mTicks < 1)
    {
        mTicks = TICK_RESET;
        mSpeed = randFloat(0.1f, 1.0f);
        mTurn = randFloat(-1.0f, 1.0f);
        headRotation = randFloat(0.0f, 360.0f);

        mTank->rotateTurret(headRotation);
        mTank->changeSpeed(mSpeed);
        mTank->changeDirection(mTurn);
    }

    mTank->move();
}

float RobotControl::randFloat(float min, float max)
{
    boost::uniform_real<float> u(min, max);
    boost::variate_generator<boost::mt19937&, boost::uniform_real<float> >
        gen(mRng, u);
    return gen();
}
