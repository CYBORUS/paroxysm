#ifndef GENERALBIN_H
#define GENERALBIN_H

#include <CGE/OpenGL/Program.h>
#include <CGE/OpenGL/VertexShader.h>
#include <CGE/OpenGL/FragmentShader.h>
#include <CGE/RenderBin.h>

class GeneralBin : public CGE::RenderBin
{
    public:
        GeneralBin();
        virtual ~GeneralBin();

    protected:
        virtual void beforeRender();
        //virtual void afterRender();
        virtual void displayActor(CGE::Actor* inActor);

    private:
        CGE::Program<2> mProgram;
        CGE::VertexShader mVS;
        CGE::FragmentShader mFS;

        GLuniform mUniMVPM;
        GLuniform mUniTexture;
};

#endif
