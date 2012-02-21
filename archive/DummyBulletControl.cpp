#include "DummyBulletControl.h"

DummyBulletControl::DummyBulletControl(Bullet* inBullet) : Control(inBullet)
{
}

DummyBulletControl::~DummyBulletControl()
{
    //dtor
}

void DummyBulletControl::update()
{
    mTurn = MathEngine::supremeRandom<float>(-1.0f, 1.0f);
    mEntity->changeDirection(mTurn);
}
