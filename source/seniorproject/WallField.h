#ifndef WALLFIELD_H
#define WALLFIELD_H

#include "../PowerVBO.h"
#include "../Point2D.h"

#include <SDL.h>

class WallField
{
    public:
        WallField();
        ~WallField();

        enum Direction { NORTH, EAST, SOUTH, WEST };

        void loadFromFile(const char* inFile);
        void createRandom();
        void dump();
        void display();

        inline const Point2D<Uint32>& size() const { return mSize; }

        inline bool canMove(Uint32 inX, Uint32 inY, Direction inDirection)
        {
            return !checkBit(getWallBit(inX, inY, inDirection));
        }

    private:
        void destroy();
        void createBitList(bool inRandom = false);
        bool checkBit(Uint32 inBit);
        void setBit(Uint32 inBit, bool inSetting);
        Uint32 getWallBit(Uint32 inX, Uint32 inY, Direction inDirection);
        void buildVBO();

        static const Uint8 bits[8];

        PowerVBO* mVBO;
        Uint8* mBitList;
        size_t mBitListLength;
        Point2D<Uint32> mSize;
};

#endif
