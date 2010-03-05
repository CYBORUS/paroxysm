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

#ifndef CONTROL_H
#define CONTROL_H

#include "Vector3D.h"
#include "Tank.h"

enum ControlType { PLAYER_TANK, NETWORK_TANK, ROBOT_TANK };

class Control
{
    public:
        Control(Tank* inTank);
        virtual ~Control();

        virtual void update() = 0;
        virtual void setPosition(Vector3D<float> inPosition);
        virtual void changeDirection(float inDirection);
        virtual void changeSpeed(float inSpeed);
        virtual void stepTurretRotation();
        virtual void setTurretRotation(float inRotation);

    protected:
        Tank* mTank;

        //AI turrets have to move differently than player turrets,
        //these controls are meant to provide that functionality
        float mHeadRotation;
        float mHeadRotationRate;
        float mHeadTargetDirection;
        float mHeadRotationDirection; //whether to turn left or right

};

#endif
