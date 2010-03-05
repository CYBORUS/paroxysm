#ifndef TANK_H
#define TANK_H

#include <SDL_opengl.h>

#include "Vector3D.h"
#include "TerrainGrid.h"
#include "Matrix.h"
#include "TSphere.h"

#include <iostream>
#include <cstdlib>
using namespace std;

class Tank
{
    public:
        Tank(TerrainGrid* inTerrain);
        virtual ~Tank();

        virtual void display();
        Vector3D<float> getPosition();
        void setPosition(Vector3D<float> inPosition);
        void rotateTurret(float inRotation);
        void setTurretDirection(float inRotation);
        void modifyTurretRotation(bool inRotate, float inRotation);
        void changeDirection(float inDirection);
        void changeSpeed(float inSpeed);

        Vector3D<float> getControlPoint();

        void move();

    protected:
        void changeMovementVector();
        //void setupModelview();
        void transformControlPoints();

        TerrainGrid* mTerrain;
        int mTerrainWidth;
        int mTerrainHeight;
        Vector3D<float> mPosition;
        Vector3D<float> mRotation;
        Vector3D<float> mMovementVector;

        //these four points control collision with the terrain
        Vector3D<float> mFrontLeftControl;
        Vector3D<float> mFrontRightControl;
        Vector3D<float> mBackLeftControl;
        Vector3D<float> mBackRightControl;

        Vector3D<float> mTransformedFrontLeftControl;
        Vector3D<float> mTransformedFrontRightControl;
        Vector3D<float> mTransformedBackLeftControl;
        Vector3D<float> mTransformedBackRightControl;

/*
        Matrix<float> mModelview;
        Matrix<float> mRotateX;
        Matrix<float> mRotateY;
        Matrix<float> mRotateZ;
        Matrix<float> mTranslate;
*/

        float mTurretAngle;

        Vector3D<float> mTankSize;

        Vector3D<float> mHeadCenter;
        float mHeadRotation;
        float mHeadRotationRate;
        float mHeadTargetDirection;
        float mHeadRotationDirection; //whether to turn left or right
        Vector3D<float> mHeadSize;

        bool mTurretConstantRotate;
        bool mAIRotateCalled;

        Vector3D<float> mTurretCenter;
        Vector3D<float> mTurretSize;

        float mTankSpeed;
        float mTankTurnRate;

        float mCurrentMoveRate;
        float mCurrentRotationRate;

        TSphere mSphere;
};

#endif // TANK_H
