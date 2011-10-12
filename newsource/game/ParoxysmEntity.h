#ifndef PAROXYSMENTITY_H
#define PAROXYSMENTITY_H

#include <CGE/Entity.h>
#include "TerrainGrid.h"
#include "GeneralBin.h"
#include "SceneGraphNode.h"

struct EntityRenderBins
{
    GeneralBin* general;
};

class ParoxysmEntity : public CGE::Entity
{
    public:
        ParoxysmEntity(TerrainGrid* inTerrain, SceneGraphNode* inBaseNode, EntityRenderBins inRenderBins);
        virtual ~ParoxysmEntity();

    protected:
        TerrainGrid* mTerrain;
        SceneGraphNode* mBaseNode;
        EntityRenderBins mRenderBins;

};

#endif
