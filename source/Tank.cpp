#include "Tank.h"

Tank::Tank(TerrainGrid& inTerrain, CGE::SceneGraphNode* inBaseNode,
    EntityRenderBins inRenderBins,
    CGE::ResourceManager<CGE::ModelFromFile>& inManager)
    : ParoxysmEntity(inTerrain, inBaseNode, inRenderBins)
{
    mMainTank = new CGE::Actor(inManager.load("bradley.c3m"));
    inBaseNode->addChildNode(mMainTank);
    mBasePosition.rotate(90, 1, 0, 0);
    mMainTank->matrix() *= mBasePosition;
    inRenderBins.general->addActor(mMainTank);
}

Tank::~Tank()
{
}

void Tank::changeDirection(double inDirection)
{
    mMainTank->matrix().loadIdentity();
    mMainTank->matrix().rotate(90, 0, 0, 1);
}

void Tank::changeSpeed(double inSpeed)
{
}
