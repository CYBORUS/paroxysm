#ifndef PAROXYSMENTITY_H
#define PAROXYSMENTITY_H

#include "Entity.h"
#include "TerrainGrid.h"


class ParoxysmEntity : public Entity
{
    public:
        ParoxysmEntity(TerrainGrid* inTerrain);
        virtual ~ParoxysmEntity();

    protected:
        TerrainGrid* mTerrain;

};

#endif // PAROXYSMENTITY_H
