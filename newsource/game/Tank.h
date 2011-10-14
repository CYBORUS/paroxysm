#ifndef TANK_H
#define TANK_H

#include "ParoxysmEntity.h"
#include <CGE/ResourceManager.h>
#include <CGE/Model.h>
#include <CGE/Actor.h>


class Tank : public ParoxysmEntity
{
    public:
        Tank(TerrainGrid* inTerrain, SceneGraphNode* inBaseNode,
            EntityRenderBins inRenderBins,
            CGE::ResourceManager<CGE::Model>* inManager);
        virtual ~Tank();

        virtual void move();
        virtual void changeDirection(float inDirection);
        virtual void changeSpeed(float inSpeed);

    protected:
        CGE::Actor* mMainTank;
};

#endif
