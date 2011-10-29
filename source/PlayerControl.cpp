#include "PlayerControl.h"

PlayerControl::PlayerControl(CGE::Entity* inEntity) : Control(inEntity)
{
    //ctor
}

PlayerControl::~PlayerControl()
{
    //dtor
}

void PlayerControl::update()
{
    vec4f position = mEntity->getPosition();
    vec4f momentum = mEntity->getMomentum();
}
