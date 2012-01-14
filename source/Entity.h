#ifndef ENTITY_H
#define ENTITY_H

#include <CGE/Actor.h>
#include <CGE/Vectors.h>
#include <CGE/LuaReference.h>
#include <lua.hpp>
#include <vector>

#include <iostream>
class Entity : public CGE::SceneGraphNode
{
    public:
        Entity(const CGE::LuaReference& inLuaTable);
        virtual ~Entity();

        void update();
        void impact(const vec3d& inMomentum);
        bool isInRangeOf(Entity* inEntity);

        inline double getMass() const           { return mMass;         }
        inline double getRadius() const         { return mRadius;       }
        inline const vec3d& getPosition() const { return mPosition;     }
        inline const vec3d& getVelocity() const { return mVelocity;     }
        inline double getMaxSpeed() const       { return mMaxSpeed;     }
        inline double getCurrentSpeed() const   { return mCurrentSpeed; }
        inline bool getIsBeingDeleted() const   { return mIsBeingDeleted; }

        inline void setMaxSpeed(double inSpeed) { mMaxSpeed = inSpeed;  }
        inline void setRadius(double inRadius)  { mRadius = inRadius;   }
        inline void setMass(double inMass)      { mMass = inMass;       }
        inline void setIsBeingDeleted()         { mIsBeingDeleted = true; }

        void addLocalMomentumVector(vec3d inMomentum);
        void addGlobalMomentumVector(const vec3d& inMomentum);
        void addLocalVelocityVector(vec3d inVelocity);
        void addGlobalVelocityVector(const vec3d& inVelocity);

        inline const vec3f& getTranslation() const
        {
            return mTranslation;
        }

        void calculateLocalOrientation()
        {
            // For now, the first actor added is assumed to be the
            // base direction actor.

            vec3d rotation = mActors[0]->getRotation();

            CGE::Matrix4x4<double> transformation;
            transformation.rotateY(rotation[1]);
            transformation.rotateX(rotation[0]);
            transformation.rotateZ(rotation[2]);

            vec4d initialForward;
            initialForward[2] = -1.0;
            initialForward[3] = 1.0;

            vec4d initialUp;
            initialUp[1] = 1.0; //not really sure this is correct, but testing it
            initialUp[3] = 1.0;

            vec4d result;
            transformation.transform(initialForward, result);
            mForwardDirection = result;
            transformation.transform(initialUp, result);
            mUpDirection = result;
        }


        void calculateLocalOrientation(vec3d& inMomentum)
        {
            vec3d rotation = mActors[0]->getRotation();

            CGE::Matrix4x4<double> transformation;
            transformation.rotateY(rotation[1]);
            transformation.rotateX(rotation[0]);
            transformation.rotateZ(rotation[2]);

            vec4d initial;
            initial[0] = inMomentum[0];
            initial[1] = inMomentum[1];
            initial[2] = inMomentum[2];
            initial[3] = 1.0;
            vec4d result;

            transformation.transform(initial, result);

            inMomentum[0] = result[0];
            inMomentum[1] = result[1];
            inMomentum[2] = result[2];
        }


        inline CGE::Actor* getActor(size_t inIndex)
        {
            return inIndex < mActors.size() ? mActors[inIndex] : NULL;
        }

        void setCollisionCallback(lua_State* inState);

        void onCollision(lua_State* inState, Entity* inEntity);

        inline void setVelocity(const vec3d& inVelocity)
        {
            mVelocity = inVelocity;
        }

        inline void setVelocity(double inX, double inY, double inZ)
        {
            mVelocity[0] = inX;
            mVelocity[1] = inY;
            mVelocity[2] = inZ;
        }

        void rotateActor(size_t inIndex, double inXRotation,
            double inYRotation, double inZRotation);

        inline void resetActorMatrix(int inActor)
        {
            //mActors[inActor]->matrix().loadIdentity();
            mActors[inActor]->resetMatrix();
            //std::cerr << "actor: " << inActor << std::endl;
        }

        inline size_t numActors()
        {
            return mActors.size();
        }

        /*******************************************
        * Makes the entity turn at a percentage of
        * mMaxTurnSpeed on any of the axes
        *
        * Values passed in should be between -1.0 and 1.0, although
        * greater values can be used
        *******************************************/
        inline void setTurnRate(double inX, double inY, double inZ)
        {
            mTurn[0] = inX;
            mTurn[1] = inY;
            mTurn[2] = inZ;
        }

        /**********************************************
        * Sets the desired maximum rotation speed for each
        * axis.
        **********************************************/
        inline void setMaxTurnSpeed(double inX, double inY, double inZ)
        {
            mMaxTurnSpeed[0] = inX;
            mMaxTurnSpeed[1] = inY;
            mMaxTurnSpeed[2] = inZ;
        }

        inline const vec3d& getMomentum()
        {
            mMomentum[0] = mVelocity[0] * mMass;
            mMomentum[1] = mVelocity[1] * mMass;
            mMomentum[2] = mVelocity[2] * mMass;

            return mMomentum;
        }

        inline void setDefaultRotation(const vec3d& inDefault)
        {
            mDefaultRotation = inDefault;
        }

        inline void setCurrentSpeed(double inSpeed)
        {
            mCurrentSpeed = inSpeed;
        }

        inline void setPosition(const vec3d& inPosition)
        {
            mPosition = inPosition;
            for (size_t i = 0; i < 3; ++i) mTranslation[i] = mPosition[i];
        }

        size_t addActor(CGE::Actor* inActor);
        size_t addActor(CGE::Actor* inActor, size_t inIndex);

    protected:
        vec3d mVelocity;
        vec3d mMomentum;
        vec3d mPosition;
        vec3f mTranslation; // pre-converted values for camera following
        vec3d mRotation;
        vec3d mDefaultRotation;
        vec3d mMaxTurnSpeed;

        double mMass;
        double mRadius;
        double mMaxSpeed;
        double mCurrentSpeed;

        CGE::LuaReference mLuaTable;
        CGE::LuaReference mLuaCallback;

        // Value should be between -1.0 and 1.0, will cause the entity to
        // rotate a percentage of mMaxTurnSpeed.
        vec3d mTurn;

        std::vector<CGE::Actor*> mActors;

        //used to determine the entities local orientation
        vec3d mForwardDirection;
        vec3d mUpDirection;

        bool mIsBeingDeleted;
};

#endif
