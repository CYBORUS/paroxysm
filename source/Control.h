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

enum ControlType { NO_CONTROLS, PLAYER_TANK, NETWORK_TANK, ROBOT_TANK };

class Control
{
    public:
        Control(Tank* inTank);
        virtual ~Control();

        virtual void update() = 0;
        virtual void setPosition(const Vector3D<float>& inPosition);
        virtual void changeDirection(float inDirection);
        virtual void changeSpeed(float inSpeed);

        virtual void setGameDead();
        virtual bool isGameDead();

    protected:
        Tank* mTank;

        bool mGameDead;

        //AI turrets have to move differently than player turrets,
        //these controls are meant to provide that functionality


};

inline void Control::setGameDead()
{
    mGameDead = true;
}

inline bool Control::isGameDead()
{
    return mGameDead;
}

#endif
