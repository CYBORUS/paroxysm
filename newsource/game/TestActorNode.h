#ifndef TESTACTORNODE_H
#define TESTACTORNODE_H

#include "ActorNode.h"

class TestActorNode : public ActorNode
{
    public:
        TestActorNode(CGE::Actor& inActor);
        virtual ~TestActorNode();

        virtual void update();

    protected:
    private:
};

#endif
