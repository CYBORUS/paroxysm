#ifndef SKYBOXBIN_H
#define SKYBOXBIN_H

#include <CGE/OpenGL/Program.h>
#include <CGE/RenderBin.h>

class SkyBoxBin : public CGE::RenderBin
{
    public:
        SkyBoxBin();
        virtual ~SkyBoxBin();
    protected:
        virtual void beforeRender();
        virtual void afterRender();
        virtual void displayActor(CGE::Actor* inActor);

    private:
        CGE::Program<2> mProgram;

        GLuniform mUniMVPM;
        GLuniform mUniTexture;
};

#endif
