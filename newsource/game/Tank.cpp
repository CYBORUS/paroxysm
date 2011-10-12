#include "Tank.h"

Tank::Tank(TerrainGrid* inTerrain, SceneGraphNode* inBaseNode, EntityRenderBins inRenderBins,
           CGE::ResourceManager<CGE::Model>* inManager) :
    ParoxysmEntity(inTerrain, inBaseNode, inRenderBins)
{
    //ctor
    mMainTank = new CGE::Actor(inManager->load("bradley.c3m"));
    inBaseNode->addChildNode(mMainTank);
    inRenderBins.general->addNode(mMainTank);
}

Tank::~Tank()
{
    //dtor
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
