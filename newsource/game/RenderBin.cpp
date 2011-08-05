#include "RenderBin.h"

RenderBin::RenderBin()
{
}

RenderBin::~RenderBin()
{
}

void RenderBin::addNode(SceneGraphNode& inNode)
{
    mNodes.push_back(&inNode);
}

void RenderBin::removeNode(SceneGraphNode& inNode)
{
    mNodes.remove(&inNode);
}

void RenderBin::displayAll()
{
    for (std::list<SceneGraphNode*>::iterator i = mNodes.begin();
        i != mNodes.end(); ++i)
    {
        SceneGraphNode* sgn = *i;
        displayNode(sgn);
    }
}
