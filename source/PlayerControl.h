#ifndef PLAYERCONTROL_H
#define PLAYERCONTROL_H

#include <CGE/Control.h>
#include <CGE/Entity.h>


class PlayerControl : public CGE::Control
{
    public:
        PlayerControl(CGE::Entity* inEntity);
        virtual ~PlayerControl();

        virtual void update();
    protected:
    private:
};

#endif // PLAYERCONTROL_H
