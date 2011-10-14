#include "RenderBin.h"

RenderBin::RenderBin()
{
}

RenderBin::~RenderBin()
{
}

void RenderBin::addNode(CGE::SceneGraphNode* inNode)
{
    mNodes.push_back(inNode);
}

void RenderBin::removeNode(CGE::SceneGraphNode* inNode)
{
    mNodes.remove(inNode);
}

void RenderBin::displayAll()
{
    for (std::list<CGE::SceneGraphNode*>::iterator i = mNodes.begin();
        i != mNodes.end(); ++i)
    {
        CGE::SceneGraphNode* sgn = *i;
        displayNode(sgn);
    }
}
