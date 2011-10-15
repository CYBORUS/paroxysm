#ifndef PAROXYSMMODULE_H
#define PAROXYSMMODULE_H

#include <CGE/ManagedModule.h>
#include <CGE/Matrix4x4.h>

#include "UI/UserInterface.h"
#include "ViewNode.h"

class ParoxysmModule : public CGE::ManagedModule
{
    public:
        ParoxysmModule();
        virtual ~ParoxysmModule();

        virtual void onResize(int inWidth, int inHeight);
    protected:
        UserInterface mUI;
        ViewNode mViewNode;

        CGE::Vector<GLint, 4> mViewport;

};

#endif // PAROXYSMMODULE_H
