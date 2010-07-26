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
        for (Uint32 i = 0; i < numberOfWalls; ++i)
        {
            int roll = rand() % 5;
            setBit(i, !roll);
        }
    }
    else
    {
        memset(mBitList, -1, mBitListLength);
    }
}

bool WallField::checkBit(Uint32 inBit) const
{
    return !!(mBitList[inBit / 8] & bits[inBit % 8]);
}

void WallField::setBit(Uint32 inBit, bool inSetting)
{
    if (inSetting)
    {
        mBitList[inBit / 8] |= bits[inBit % 8];
    }
    else
    {
        mBitList[inBit / 8] &= ~bits[inBit % 8];
    }
}

Uint32 WallField::getWallBit(Uint32 inX, Uint32 inY, Direction inDirection)
    const
{
    switch (inDirection)
    {
        case NORTH: return (2 * mSize.x * inY) + (2 * inX);
        case WEST: return (2 * mSize.x * inY) + (2 * inX) + 1;
        case EAST: return getWallBit(inX + 1, inY, WEST);
        case SOUTH: return getWallBit(inX, inY + 1, NORTH);
    }

    return 0;
}

void WallField::buildVBO()
{
    mVBO = new PowerVBO;

    vector<float> vertices;
    vertices.reserve((mSize.x + 1) * (mSize.y + 1) * 2);

    for (Uint32 i = 0; i <= mSize.y; ++i)
    {
        for (Uint32 j = 0; j <= mSize.x; ++j)
        {
            float x = float(j);
            float y = float(i);

            vertices.push_back(x);
            vertices.push_back(0.0f);
            vertices.push_back(y);

            vertices.push_back(x);
            vertices.push_back(1.0f);
            vertices.push_back(y);
        }
    }

    mVBO->loadVertexArray(PVBO_VERTEX, 3, vertices.size(), &vertices[0]);

    vector<Uint32> indices;

    for (Uint32 i = 0; i < mSize.y; ++i)
    {
        for (Uint32 j = 0; j < mSize.x; ++j)
        {
            Uint32 b = (2 * mSize.x * i) + (2 * j);
            Uint32 firstVertex = ((mSize.x + 1) * i + j) * 2;
            if (checkBit(b))
            {
                indices.push_back(firstVertex);
                indices.push_back(firstVertex + 1);
                indices.push_back(firstVertex + 3);
                indices.push_back(firstVertex + 2);
            }

            if (checkBit(b + 1))
            {
                indices.push_back(firstVertex);
                indices.push_back(firstVertex + 1);
                indices.push_back(firstVertex + ((mSize.x + 1) * 2) + 1);
                indices.push_back(firstVertex + ((mSize.x + 1) * 2));
            }
        }
    }

    mVBO->loadIndexArray(GL_QUADS, indices.size(), &indices[0]);
}

void WallField::loadFromFile(const char* inFile)
{
    destroy();
}

void WallField::createRandom()
{
    destroy();
    mSize.x = rand() % 16 + 16;
    mSize.y = rand() % 16 + 16;
    //mSize.x = 4;
    //mSize.y = 4;
    createBitList(true);
    for (Uint32 i = 0; i < mSize.x; ++i)
    {
        setBit(getWallBit(i, 0, NORTH), true);
        setBit(getWallBit(i, mSize.y - 1, NORTH), true);
        setBit(getWallBit(i, mSize.y - 1, WEST), false);
    }

    for (Uint32 i = 0; i < mSize.y; ++i)
    {
        setBit(getWallBit(0, i, WEST), true);
        setBit(getWallBit(mSize.x - 1, i, WEST), true);
        setBit(getWallBit(mSize.x - 1, i, NORTH), false);
    }

    setBit(getWallBit(0, mSize.y - 1, WEST), false);
    setBit(getWallBit(mSize.x - 1, mSize.y - 1, WEST), false);

    buildVBO();
}

void WallField::dump()
{
    cout << mSize.x << " x " << mSize.y << " : ";
    Uint32 numBits = mSize.x * mSize.y * 2;
    for (Uint32 i = 0; i < numBits; ++i) cout << (checkBit(i) ? '1' : '0');
    cout << endl;
}

void WallField::display()
{
    if (mVBO) mVBO->display();
}
