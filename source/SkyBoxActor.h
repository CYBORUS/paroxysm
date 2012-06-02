#ifndef SKYBOXACTOR_H
#define SKYBOXACTOR_H

#include "SkyBox.h"
#include <CGE/MatrixNode.h>

class SkyBoxActor
{
    public:
        SkyBoxActor(SkyBox& inSkyBox);
        virtual ~SkyBoxActor();

        void display();

        inline CGE::MatrixNode<float>& modelViewProjectionNode()
        {
            return mModelViewProjection;
        }

        inline CGE::Matrix4x4<float>& matrix() { return mMatrix; }

    protected:
    private:
        SkyBox& mSkyBox;

        CGE::Matrix4x4<float> mMatrix;
        CGE::MatrixNode<float> mModelViewProjection;

};

#endif
