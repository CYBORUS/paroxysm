#include "RenderBin.h"

namespace CGE
{
    RenderBin::RenderBin() : mHead(NULL)
    {
    }

    RenderBin::~RenderBin()
    {
    }

    void RenderBin::addActor(Actor* inActor)
    {
        if (!inActor) return;

        inActor->remove();

        if (mHead)
        {
            mHead->mHead = NULL;
            inActor->mNextActor = mHead;
        }

        mHead = inActor;
        inActor->mHead = &mHead;
    }

    void RenderBin::renderAll()
    {
        for (Actor* a = mHead; a; a = a->mNextActor)
        {
            displayActor(a);
        }
    }

    void RenderBin::displayActor(Actor* inActor)
    {
        inActor->display();
    }
}
