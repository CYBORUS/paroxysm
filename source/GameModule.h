#ifndef GAMEMODULE_H
#define GAMEMODULE_H

#include "Module.h"
#include "TerrainGrid.h"
#include "Point2D.h"

#include <iostream>
#include <fstream>
using namespace std;

class GameModule : public Module
{
    public:
        GameModule(const char* inMapFile);
        virtual ~GameModule();

        void onLoop();
        void onFrame();
        bool onLoad();
        void onInit();
        void onCleanup();


    protected:
        TerrainGrid mTerrain;

        bool mDead;
        Module* mNextModule;

        Point2D<int> mDisplay;
        Point2D<int> mCenter;
        Point2D<int> mTerrainSize;

        Light mLight;

        Vector3D<GLfloat> mTrackball;
        Vector3D<GLfloat> mPanning;
};

#endif // GAMEMODULE_H
