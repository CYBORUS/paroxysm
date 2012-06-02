#include "SimpleDisplayActor.h"

SimpleDisplayActor::SimpleDisplayActor(CGE::Displayable& inDisplayable)
    : GeneralActor(mMatrix), mDisplayable(inDisplayable)
{
}

SimpleDisplayActor::~SimpleDisplayActor()
{
}

void SimpleDisplayActor::display()
{
    mDisplayable.display();
}
