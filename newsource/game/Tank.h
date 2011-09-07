#ifndef TANK_H
#define TANK_H

#include "ParoxysmEntity.h"


class Tank : public ParoxysmEntity
{
    public:
        Tank(TerrainGrid* inTerrain);
        virtual ~Tank();

        virtual void move();
        virtual void display();
        virtual void changeDirection(float inDirection);
        virtual void changeSpeed(float inSpeed);

    protected:
};

#endif // TANK_H
