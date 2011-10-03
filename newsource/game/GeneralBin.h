#ifndef GENERALBIN_H
#define GENERALBIN_H

#include <CGE/Program.h>
#include <CGE/VertexShader.h>
#include <CGE/FragmentShader.h>

#include "RenderBin.h"

class GeneralBin : public RenderBin
{
    public:
        GeneralBin();
        virtual ~GeneralBin();

        void displayAll();

    protected:
        virtual void displayNode(SceneGraphNode* inNode);

    private:
        CGE::Program<2> mProgram;
        CGE::VertexShader mVS;
        CGE::FragmentShader mFS;

        GLuniform mUniMVPM;
        GLuniform mUniTexture;
};

#endif
