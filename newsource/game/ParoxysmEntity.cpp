#include "ParoxysmEntity.h"

ParoxysmEntity::ParoxysmEntity(TerrainGrid* inTerrain,
    CGE::SceneGraphNode* inBaseNode, EntityRenderBins inRenderBins)
{
    mTerrain = inTerrain;
    mBaseNode = inBaseNode;
    mRenderBins = inRenderBins;
}

ParoxysmEntity::~ParoxysmEntity()
{
}
