#include "ASField.h"

#include <cstring>

#include <SDL.h>

ASField::ASField(const WallField& inField) : mField(&inField)
{
    mWidth = mField->width();
    mHeight = mField->height();
    mSize = mWidth * mHeight;
    mOpenList = new ASNode*[mSize];
    memset(mOpenList, 0, mSize * sizeof(ASNode*));
    mClosedList = new ASNode*[mSize];
    memset(mClosedList, 0, mSize * sizeof(ASNode*));
}

ASField::~ASField()
{
    for (size_t i = 0; i < mSize; ++i)
    {
        if (mOpenList[i])
        {
            delete mOpenList[i];
            mClosedList[i] = NULL;
        }

        if (mClosedList[i]) delete mClosedList[i];
    }

    delete [] mClosedList;
    delete [] mOpenList;
}

void ASField::findPath(Uint32 inStartX, Uint32 inStartY, Uint32 inEndX,
    Uint32 inEndY)
{

}
