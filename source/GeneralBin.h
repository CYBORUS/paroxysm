#ifndef GENERALBIN_H
#define GENERALBIN_H

#include "GeneralActor.h"
#include <CGE/OpenGL/Program.h>
#include <set>

class GeneralBin
{
    public:
        GeneralBin();
        ~GeneralBin();

        void addActor(GeneralActor& inActor);
        void renderAll();

    private:
        std::set<GeneralActor*> mActors;
        CGE::Program<2> mProgram;

        GLuniform mUniMVPM;
        GLuniform mUniTexture;
};

#endif
