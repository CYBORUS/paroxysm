#ifndef TANK_H
#define TANK_H

#include "ParoxysmEntity.h"
#include <CGE/ResourceManager.h>
#include <CGE/ModelFromFile.h>
#include <CGE/Actor.h>
#include <CGE/Matrix4x4.h>

class Tank : public ParoxysmEntity
{
    public:
        Tank(TerrainGrid& inTerrain, CGE::SceneGraphNode* inBaseNode,
            EntityRenderBins inRenderBins,
            CGE::ResourceManager<CGE::ModelFromFile>& inManager);
        virtual ~Tank();

        virtual void changeDirection(double inDirection);
        virtual void changeSpeed(double inSpeed);

    protected:
        CGE::Actor* mMainTank;
        mat4f mBasePosition;
};

#endif
