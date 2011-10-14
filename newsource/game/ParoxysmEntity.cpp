#include "ParoxysmEntity.h"

ParoxysmEntity::ParoxysmEntity(TerrainGrid* inTerrain, SceneGraphNode* inBaseNode, EntityRenderBins inRenderBins)
{
    mTerrain = inTerrain;
    mBaseNode = inBaseNode;
    mRenderBins = inRenderBins;
}

ParoxysmEntity::~ParoxysmEntity()
{
    //dtor
}
