#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include "Control.h"


class PlayerControl : public Control
{
    public:
        PlayerControl(Tank* inTank);
        virtual ~PlayerControl();

        void update();
    protected:
    private:
};

#endif // PLAYERCONTROL_H
