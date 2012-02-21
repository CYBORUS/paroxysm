#ifndef DUMMYBULLETCONTROL_H
#define DUMMYBULLETCONTROL_H

#include "Control.h"
#include "Bullet.h"
#include "MathEngine.h"

class DummyBulletControl : public Control
{
    public:
        DummyBulletControl(Bullet* inBullet);
        virtual ~DummyBulletControl();

        virtual void update();
    private:
            float mTurn;

};

#endif // DUMMYBULLETCONTROL_H
