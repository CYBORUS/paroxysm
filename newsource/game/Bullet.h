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

#ifndef BULLET_H
#define BULLET_H

#include "Entity.h"
//#include "TSphere.h"
//#include "OGL.h"
//#include "MathEngine.h"

class Bullet : public Entity
{
    public:
        Bullet(const Vector3D<float>& inPosition,
            const Vector3D<float>& inMomentum, float inRotation);
        virtual ~Bullet();

        virtual void display();
        static void setSphere(TSphere* inSphere);

        virtual void changeDirection(float inDirection);
        virtual void changeSpeed(float inSpeed);

        void onCollision(Entity* inCollidedWith);
        void move();


    protected:
        //static TSphere* mSphere;
        float mSpeed;
};

//inline void Bullet::setSphere(TSphere* inSphere)
//{
//    mSphere = inSphere;
//}

#endif
