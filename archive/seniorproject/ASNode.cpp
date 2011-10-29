#include "ASNode.h"

#include <iostream>
using namespace std;

ASNode::ASNode(int inX, int inY, int inH) : mParent(NULL), mClosed(false),
    mX(inX), mY(inY), mF(inH), mH(inH), mG(0)
{
    //cerr << "new node created at " << inX << ", " << inY << endl;
}

void ASNode::connect(ASNode* inParent, int inOffsetG)
{
    if (!inParent) return;
    mParent = inParent;
    mG = mParent->mG + inOffsetG;
    mF = mG + mH;
}

void ASNode::compare(ASNode* inParent, int inOffsetG)
{
    if (!inParent || mClosed) return;
    int g = inParent->mG + inOffsetG;
    if (g < mG) connect(inParent, inOffsetG);
}

ASNode::~ASNode()
{
}
