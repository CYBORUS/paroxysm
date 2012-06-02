#ifndef SIMPLEDISPLAYACTOR_H
#define SIMPLEDISPLAYACTOR_H

#include "GeneralActor.h"
#include <CGE/Displayable.h>

class SimpleDisplayActor : public GeneralActor
{
    public:
        SimpleDisplayActor(CGE::Displayable& inDisplayable);
        virtual ~SimpleDisplayActor();

        virtual void display();

    protected:
    private:
        CGE::Displayable& mDisplayable;
        CGE::Matrix4x4<float> mMatrix;
};

#endif
