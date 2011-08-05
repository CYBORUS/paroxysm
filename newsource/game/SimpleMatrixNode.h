#ifndef SIMPLEMODELNODE_H
#define SIMPLEMODELNODE_H

#include "ActorNode.h"

class SimpleMatrixNode : public ActorNode
{
    public:
        SimpleMatrixNode(CGE::Actor& inActor);
        virtual ~SimpleMatrixNode();

        inline CGE::Matrix4x4<float>& matrix() { return mTransform; }

    protected:
    private:
};

#endif
