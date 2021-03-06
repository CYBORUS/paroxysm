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
#include "MathEngine.h"

#include <ctime>

RobotControl::RobotControl(Entity* inEntity) : Control(inEntity), mTicks(1),
    mTurn(0.0f), mSpeed(0.0f)
{
    mTank = (Tank*)inEntity;
    mWhatAmI = Control::ROBOT_TANK;
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
        mSpeed = MathEngine::supremeRandom<float>(0.1f, 1.0f);
        mTurn = MathEngine::supremeRandom<float>(-1.0f, 1.0f);
        mHeadRotation = MathEngine::supremeRandom<float>(0.0f, 360.0f);

        mTank->rotateTurret(mHeadRotation);
        mEntity->changeSpeed(mSpeed);
        mEntity->changeDirection(mTurn);
    }
}
