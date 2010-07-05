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

#ifndef ENTITY_H
#define ENTITY_H

#include "Vector3D.h"
#include "TerrainGrid.h"


class Entity
{
    public:
        Entity();
        virtual ~Entity();

        enum EntityType { TANK, BULLET };

        virtual void onCollision(Entity* inCollidedWith) = 0;
        virtual void update();
        virtual void move() = 0;
        virtual void display() = 0;
        virtual void changeDirection(float inDirection) = 0;
        virtual void changeSpeed(float inSpeed) = 0;

        static void setTerrain(TerrainGrid* inTerrain);

        void setRadius(float inRadius);
        float getRadius();

        void setGameDead();
        void setRenderDead();

        bool isGameDead();
        bool isRenderDead();

        const Vector3D<float>& getMomentum() const;
        const Vector3D<float>& getPosition() const;
        void setPosition(const Vector3D<float>& inPosition);

        EntityType getWhatIAm();
        bool isAlive();
        void die();

    protected:
        EntityType mWhatAmI;
        float mRadius;

        //Control* mControl;

        bool volatile mGameDead;
        bool volatile mRenderDead;

        static TerrainGrid* mTerrain;
        int mTerrainWidth;
        int mTerrainHeight;


        Vector3D<float> mMomentum;
        Vector3D<float> mPosition;
        Vector3D<float> mRotation;

        //for remembering where we were in case of collisions
        Vector3D<float> mPreviousPosition;

        bool mAlive;

};

inline void Entity::die()
{
    mAlive = false;
}
inline const Vector3D<float>& Entity::getPosition() const
{
    return mPosition;
}

inline const Vector3D<float>& Entity::getMomentum() const
{
    return mMomentum;
}

inline float Entity::getRadius()
{
    return mRadius;
}

inline bool Entity::isAlive()
{
    return mAlive;
}

inline void Entity::setGameDead()
{
    mGameDead = true;
}

inline void Entity::setRenderDead()
{
    mRenderDead = true;
}

inline bool Entity::isGameDead()
{
    return mGameDead;
}

inline bool Entity::isRenderDead()
{
    return mRenderDead;
}

#endif
