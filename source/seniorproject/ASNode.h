#ifndef ASNODE_H
#define ASNODE_H

#include <cstdlib>

class ASNode
{
    public:
        ASNode(int inX, int inY, int inH);
        ~ASNode();

        void connect(ASNode* inParent, int inOffsetG);
        void compare(ASNode* inParent, int inOffsetG);
        inline void close() { mClosed = true; }

        inline bool isClosed() { return mClosed; }
        inline int getF() { return mF; }
        inline int getG() { return mG; }
        inline int getH() { return mH; }
        inline int getX() { return mX; }
        inline int getY() { return mY; }

    private:
        ASNode* mParent;
        bool mClosed;

        int mX;
        int mY;

        int mF;
        int mH;
        int mG;
};

#endif
