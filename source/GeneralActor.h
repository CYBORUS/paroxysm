#ifndef GENERALACTOR_H
#define GENERALACTOR_H

#include <CGE/MatrixNode.h>

class GeneralActor
{
    public:
        GeneralActor(const CGE::Matrix4x4<float>& inMatrix);
        virtual ~GeneralActor();

        virtual void display() = 0;

        inline CGE::MatrixNode<float>& modelViewProjectionNode()
        {
            return mModelViewProjection;
        }

    protected:
    private:
        CGE::MatrixNode<float> mModelViewProjection;
};

#endif
