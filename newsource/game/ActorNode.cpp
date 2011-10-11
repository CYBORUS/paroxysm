#include "ActorNode.h"

ActorNode::ActorNode(CGE::Actor& inActor) : mActor(&inActor)
{
}

ActorNode::~ActorNode()
{
}

void ActorNode::display()
{
    mActor->display();
}

void ActorNode::update()
{
}
