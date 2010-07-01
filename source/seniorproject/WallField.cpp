#include "WallField.h"

#include <iostream>
#include <cstring>
using namespace std;

const Uint8 WallField::bits[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02,
    0x01};

WallField::WallField() : mVBO(NULL), mBitList(NULL)
{
}

WallField::~WallField()
{
    destroy();
}

void WallField::destroy()
{
    if (mVBO)
    {
        delete mVBO;
        mVBO = NULL;
    }

    if (mBitList)
    {
        delete [] mBitList;
        mBitList = NULL;
    }
}

void WallField::createBitList(bool inRandom)
{
    Uint32 numberOfWalls = mSize.x * mSize.y * 2;
    mBitListLength = numberOfWalls / 8;
    if (numberOfWalls % 8) ++mBitListLength;
    mBitList = new Uint8[mBitListLength];
    if (inRandom)
    {
        for (size_t i = 0; i < mBitListLength; ++i)
            mBitList[i] = rand() % 256;
    }
    else
    {
        memset(mBitList, 0, mBitListLength);
    }
}

bool WallField::checkBit(Uint32 inBit)
{
    return mBitList[inBit / 8] & bits[inBit % 8];
}

void WallField::loadFromFile(const char* inFile)
{
    destroy();
}

void WallField::createRandom()
{
    destroy();
    mSize.x = rand() % 32 + 1;
    mSize.y = rand() % 32 + 1;
    createBitList(true);
}

void WallField::dump()
{
    cout << mSize.x << " x " << mSize.y << " : ";
    Uint32 numBits = mSize.x * mSize.y * 2;
    for (Uint32 i = 0; i < numBits; ++i)
    {
        cout << (checkBit(i) ? '1' : '0');
    }
    cout << endl;
}
