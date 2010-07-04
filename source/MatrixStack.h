#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include "Matrix3D.h"

class MatrixStack
{
    public:
        MatrixStack(size_t inCapacity = 16);
        ~MatrixStack();

        inline Matrix3D& matrix() { return mMatrices[mSize - 1]; }
        void push();
        void pop();

    private:
        Matrix3D* mMatrices;
        size_t mCapacity;
        size_t mSize;
};

#endif
