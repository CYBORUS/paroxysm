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

#include "CGE/Vectors.h"
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

        //void setGameDead();
        //void setRenderDead();

        void addReference();
        Entity* getReference();
        void removeReference();
        bool hasReferences();

        bool isGameDead();
        bool isRenderDead();

        const vec4f& getMomentum() const;
        const vec4f& getPosition() const;
        void setPosition(const vec4f& inPosition);

        EntityType getWhatIAm();
        bool isAlive();
        void die();

    protected:
        EntityType mWhatAmI;
        float mRadius;

        char mNumReferences;

        //Control* mControl;

        //bool volatile mGameDead;
        //bool volatile mRenderDead;

        static TerrainGrid* mTerrain;
        int mTerrainWidth;
        int mTerrainHeight;


        vec4f mMomentum;
        vec4f mPosition;
        vec4f mRotation;

        //for remembering where we were in case of collisions
        vec4f mPreviousPosition;

        bool mAlive;

};

inline void Entity::die()
{
    mAlive = false;
}
inline const vec4f& Entity::getPosition() const
{
    return mPosition;
}

inline const vec4f& Entity::getMomentum() const
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

//inline void Entity::setGameDead()
//{
//    mGameDead = true;
//}
//
//inline void Entity::setRenderDead()
//{
//    mRenderDead = true;
//}
//
//inline bool Entity::isGameDead()
//{
//    return mGameDead;
//}
//
//inline bool Entity::isRenderDead()
//{
//    return mRenderDead;
//}

/***********************************
*   Here for readability with a model that
*   wasn't really built for reference counting.
*   Just indicates that you are holding a reference
*   to this entity somewhere
*************************************/
inline void Entity::addReference()
{
    ++mNumReferences;
}

/**
*   Adds to the number of references held to this entity
*   and returns a reference to this entity.
**/
inline Entity* Entity::getReference()
{
    ++mNumReferences;
    return this;
}

/**
*   Removes a reference to this entity.
**/
inline void Entity::removeReference()
{
    --mNumReferences;
}

/**
*   Checks to see if there are any references being held to this entity
**/
inline bool Entity::hasReferences()
{
    return mNumReferences > 0;
}

#endif
