#ifndef ASFIELD_H
#define ASFIELD_H

#include "ASNode.h"
#include "WallField.h"
#include "../Point2D.h"

#include <list>

class ASField
{
    public:
        ASField(const WallField& inField);
        ~ASField();

        void findPath(int inStartX, int inStartY, int inEndX,
            int inEndY);

    private:
        void clear();
        static int findHeuristic(int inStartX, int inStartY,
            int inEndX, int inEndY);

        inline size_t toIndex(int inRow, int inCol)
        {
            return inRow * mWidth + inCol;
        }

        const WallField* mField;
        Point2D<int> mStart;
        Point2D<int> mEnd;
        ASNode** mNodes;
        std::list<ASNode*> mOpenList;
        std::list<ASNode*> mClosedList;
        size_t mSize;
        int mWidth;
        int mHeight;
};

#endif
