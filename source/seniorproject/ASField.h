#ifndef ASFIELD_H
#define ASFIELD_H

#include "WallField.h"
#include "ASNode.h"

class ASField
{
    public:
        ASField(const WallField& inField);
        ~ASField();

        void findPath(Uint32 inStartX, Uint32 inStartY, Uint32 inEndX,
            Uint32 inEndY);

    private:
        void clear();

        inline size_t toIndex(Uint32 inRow, Uint32 inCol)
        {
            return inRow * mWidth + inCol;
        }

        const WallField* mField;
        ASNode** mOpenList;
        ASNode** mClosedList;
        size_t mSize;
        Uint32 mWidth;
        Uint32 mHeight;
};

#endif
