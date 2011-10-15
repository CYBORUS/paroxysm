#include "Tank.h"

Tank::Tank(TerrainGrid* inTerrain, CGE::SceneGraphNode* inBaseNode,
    EntityRenderBins inRenderBins, CGE::ResourceManager<CGE::Model>* inManager)
    : ParoxysmEntity(inTerrain, inBaseNode, inRenderBins)
{
    mMainTank = new CGE::Actor(inManager->load("bradley.c3m"));
    inBaseNode->addChildNode(mMainTank);
    inRenderBins.general->addNode(mMainTank);

    mMainTank->matrix().rotate(90, 1, 0, 0);
}

Tank::~Tank()
{
}

void Tank::move()
{
}

void Tank::changeDirection(float inDirection)
{
}

void Tank::changeSpeed(float inSpeed)
{
}
