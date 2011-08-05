#ifndef RENDERBIN_H
#define RENDERBIN_H

#include "SceneGraphNode.h"

#include <list>

class RenderBin
{
    public:
        RenderBin();
        virtual ~RenderBin();

        void addNode(SceneGraphNode& inNode);
        void removeNode(SceneGraphNode& inNode);

        void displayAll();

    protected:
        virtual void displayNode(SceneGraphNode* inNode) = 0;

    private:
        std::list<SceneGraphNode*> mNodes;
};

#endif
