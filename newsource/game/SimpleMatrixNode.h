#ifndef SIMPLEMODELNODE_H
#define SIMPLEMODELNODE_H

#include "SceneGraphNode.h"

class SimpleMatrixNode : public SceneGraphNode
{
    public:
        SimpleMatrixNode();
        virtual ~SimpleMatrixNode();

        inline CGE::Matrix4x4<float>& matrix() { return mTransform; }

    protected:
    private:
};

#endif
