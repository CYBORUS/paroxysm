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

#ifndef ROBOTCONTROL_H
#define ROBOTCONTROL_H

#include "Control.h"
#include "Tank.h"

#define TICK_RESET 15

class RobotControl : public Control
{
    public:
        RobotControl(Entity* inEntity);
        virtual ~RobotControl();

        virtual void update();

    private:
        Tank* mTank;

        int mTicks;
        float mTurn;
        float mSpeed;

        float mHeadRotation;
};

#endif
