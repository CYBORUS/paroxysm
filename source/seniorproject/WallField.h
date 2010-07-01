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

        void loadFromFile(const char* inFile);
        void createRandom();
        void dump();

    private:
        void destroy();
        void createBitList(bool inRandom = false);
        bool checkBit(Uint32 inBit);

        static const Uint8 bits[8];

        PowerVBO* mVBO;
        Uint8* mBitList;
        size_t mBitListLength;
        Point2D<Uint32> mSize;
};

#endif
