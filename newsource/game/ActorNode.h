#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <CGE/Actor.h>
#include "SceneGraphNode.h"

class ActorNode : public SceneGraphNode
{
    public:
        ActorNode(CGE::Actor& inActor);
        virtual ~ActorNode();

        virtual void display();
        virtual void update();

    protected:
    private:
        CGE::Actor* mActor;
};

#endif
