#ifndef PAROXYSMENTITY_H
#define PAROXYSMENTITY_H

#include <CGE/Entity.h>
#include "TerrainGrid.h"


class ParoxysmEntity : public CGE::Entity
{
    public:
        ParoxysmEntity(TerrainGrid* inTerrain);
        virtual ~ParoxysmEntity();

    protected:
        TerrainGrid* mTerrain;

};

#endif
