#ifndef GENERALBIN_H
#define GENERALBIN_H

#include <CGE/OpenGL/Program.h>
#include <CGE/OpenGL/VertexShader.h>
#include <CGE/OpenGL/FragmentShader.h>

#include "RenderBin.h"

class GeneralBin : public RenderBin
{
    public:
        GeneralBin();
        virtual ~GeneralBin();

        void displayAll();

    protected:
        virtual void displayNode(CGE::SceneGraphNode* inNode);

    private:
        CGE::Program<2> mProgram;
        CGE::VertexShader mVS;
        CGE::FragmentShader mFS;

        GLuniform mUniMVPM;
        GLuniform mUniTexture;
};

#endif
