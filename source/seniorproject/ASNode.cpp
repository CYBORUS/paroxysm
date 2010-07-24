#include "ASNode.h"

ASNode::ASNode(Uint32 inH) : mParent(NULL), mF(inH), mH(inH), mG(0)
{
}

ASNode::ASNode(Uint32 inH, Uint32 inOffsetG, ASNode* inParent)
    : mParent(inParent), mH(inH)
{
    mG = mParent->mG + inOffsetG;
    mF = mG + mH;
}

ASNode::~ASNode()
{
}
