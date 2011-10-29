#include "MatrixStack.h"

MatrixStack::MatrixStack(size_t inCapacity) : mCapacity(inCapacity), mSize(1)
{
    if (mCapacity < 2) mCapacity = 2;
    mMatrices = new Matrix3D[mCapacity];
}

MatrixStack::~MatrixStack()
{
    delete [] mMatrices;
}

void MatrixStack::push()
{
    if (mSize < mCapacity)
    {
        mMatrices[mSize] = mMatrices[mSize - 1];
        ++mSize;
    }
}

void MatrixStack::pop()
{
    if (mSize > 1) --mSize;
}

void MatrixStack::reset()
{
    mSize = 1;
    mMatrices[0].loadIdentity();
}
