#include "ASField.h"

#include <cstring>
#include <iostream>
using namespace std;

ASField::ASField(const WallField& inField) : mField(&inField),
    mDestination(NULL), mPathSize(0)
{
    mWidth = mField->width();
    mHeight = mField->height();
    mSize = mWidth * mHeight;
    mNodes = new ASNode*[mSize];
    memset(mNodes, 0, mSize * sizeof(ASNode*));
}

ASField::~ASField()
{
    for (size_t i = 0; i < mSize; ++i)
        if (mNodes[i]) delete mNodes[i];
    delete [] mNodes;
}

void ASField::findPath(int inStartX, int inStartY, int inEndX,
    int inEndY)
{
    mEnd.x = inEndX;
    mEnd.y = inEndY;
    mStart.x = inStartX;
    mStart.y = inStartY;


    int h = findHeuristic(mStart.x, mStart.y, mEnd.x, mEnd.y);
    //cerr << "h is " << h << endl;
    mOpenList.push_back(new ASNode(mStart.x, mStart.y, h));

    while (!mOpenList.empty())
    {
        ASNode* lowestF = NULL;
        for (list<ASNode*>::iterator i = mOpenList.begin();
            i != mOpenList.end(); ++i)
        {
            ASNode* j = *i;
            if (!lowestF || j->getF() < lowestF->getF()) lowestF = j;
        }

        //cerr << "processing node at " << lowestF->getX() << ", "
        //    << lowestF->getY() << endl;

        mOpenList.remove(lowestF);
        mClosedList.push_back(lowestF);
        lowestF->close();

        if (!lowestF->getH())
        {
            mDestination = lowestF;
            break;
        }

        int x = lowestF->getX();
        int y = lowestF->getY();

        if (mField->canMove(x, y, WallField::NORTH))
        {
            h = findHeuristic(x, y - 1, mEnd.x, mEnd.y);
            ASNode* targetNode = mNodes[toIndex(y - 1, x)];
            if (!targetNode)
            {
                ASNode* asn = new ASNode(x, y - 1, h);
                asn->connect(lowestF, 10);
                mNodes[toIndex(y - 1, x)] = asn;
                mOpenList.push_back(asn);
            }
            else
            {
                targetNode->compare(lowestF, 10);
            }
        }

        if (mField->canMove(x, y, WallField::SOUTH))
        {
            h = findHeuristic(x, y + 1, mEnd.x, mEnd.y);
            ASNode* targetNode = mNodes[toIndex(y + 1, x)];
            if (!targetNode)
            {
                ASNode* asn = new ASNode(x, y + 1, h);
                asn->connect(lowestF, 10);
                mNodes[toIndex(y + 1, x)] = asn;
                mOpenList.push_back(asn);
            }
            else
            {
                targetNode->compare(lowestF, 10);
            }
        }

        if (mField->canMove(x, y, WallField::WEST))
        {
            h = findHeuristic(x - 1, y, mEnd.x, mEnd.y);
            ASNode* targetNode = mNodes[toIndex(y, x - 1)];
            if (!targetNode)
            {
                ASNode* asn = new ASNode(x - 1, y, h);
                asn->connect(lowestF, 10);
                mNodes[toIndex(y, x - 1)] = asn;
                mOpenList.push_back(asn);
            }
            else
            {
                targetNode->compare(lowestF, 10);
            }
        }

        if (mField->canMove(x, y, WallField::EAST))
        {
            h = findHeuristic(x + 1, y, mEnd.x, mEnd.y);
            ASNode* targetNode = mNodes[toIndex(y, x + 1)];
            if (!targetNode)
            {
                ASNode* asn = new ASNode(x + 1, y, h);
                asn->connect(lowestF, 10);
                mNodes[toIndex(y, x + 1)] = asn;
                mOpenList.push_back(asn);
            }
            else
            {
                targetNode->compare(lowestF, 10);
            }
        }
    }

    if (mDestination)
    {
        cerr << "path found! " << endl;
    }
    else
    {
        cerr << "no path found" << endl;
    }
}

WallField::Direction* ASField::getPath()
{
    if (!mDestination) return NULL;

    list<WallField::Direction> path;
    ASNode* a = mDestination;
    ASNode* b = a->getParent();

    while (b)
    {
        WallField::Direction d;

        if (a->getX() > b->getX())
            d = WallField::EAST;
        else if (a->getX() < b->getX())
            d = WallField::WEST;
        else if (a->getY() < b->getY())
            d = WallField::NORTH;
        else if (a->getY() > b->getY())
            d = WallField::SOUTH;

        path.push_front(d);

        a = b;
        b = b->getParent();
    }

    mPathSize = path.size();
    WallField::Direction* outArray = new WallField::Direction[mPathSize];
    size_t index = 0;
    for (list<WallField::Direction>::iterator i = path.begin();
        i != path.end(); ++i)
    {
        outArray[index] = *i;
        ++index;
    }

    return outArray;
}

int ASField::findHeuristic(int inStartX, int inStartY, int inEndX,
    int inEndY)
{
    int dx = abs(inEndX - inStartX);
    int dy = abs(inEndY - inStartY);
    return 10 * (dx + dy);
}
