#ifndef PAROXYSMMODULE_H
#define PAROXYSMMODULE_H

#include <CGE/UI/UserInterface.h>
#include <CGE/ManagedModule.h>
#include <CGE/MatrixNode.h>

class ParoxysmModule : public CGE::ManagedModule
{
    public:
        ParoxysmModule();
        virtual ~ParoxysmModule();

        virtual void onResize(int inWidth, int inHeight);
    protected:
        CGE::UserInterface mUI;
        mat4f mProjection;

        CGE::Vector<GLint, 4> mViewport;
};

#endif
