#ifndef SKYBOXBIN_H
#define SKYBOXBIN_H

#include "SkyBoxActor.h"
#include <CGE/OpenGL/Program.h>
#include <set>

class SkyBoxBin
{
    public:
        SkyBoxBin();
        ~SkyBoxBin();

        void addActor(SkyBoxActor& inActor);
        void renderAll();

    private:
        std::set<SkyBoxActor*> mActors;
        CGE::Program<2> mProgram;

        GLuniform mUniMVPM;
        GLuniform mUniTexture;
};

#endif
