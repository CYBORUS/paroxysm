#include "TestActorNode.h"

TestActorNode::TestActorNode(CGE::Actor& inActor) : ActorNode(inActor)
{
    mTransform.rotateX(90.0f);
}

TestActorNode::~TestActorNode()
{
}

void TestActorNode::update()
{
    mTransform.rotateY(0.5f);
}
