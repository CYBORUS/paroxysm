#ifndef RENDERBIN_H
#define RENDERBIN_H

#include <CGE/SceneGraphNode.h>

#include <list>

class RenderBin
{
    public:
        RenderBin();
        virtual ~RenderBin();

        void addNode(CGE::SceneGraphNode* inNode);
        void removeNode(CGE::SceneGraphNode* inNode);

        void displayAll();

    protected:
        virtual void displayNode(CGE::SceneGraphNode* inNode) = 0;

    private:
        std::list<CGE::SceneGraphNode*> mNodes;
};

#endif
