#ifndef FIELDMODULE_H
#define FIELDMODULE_H

#include "../Module.h"
#include "../GameCamera.h"
#include "../TSphere.h"
#include "WallField.h"

class FieldModule : public Module
{
    public:
        FieldModule();
        virtual ~FieldModule();

        /// module operation
        virtual bool onLoad();
        virtual void onOpen();
        virtual void onLoop();
        virtual void onFrame();
        virtual void onClose();
        virtual void onUnload();

    protected:
    private:
        WallField mField;
        GameCamera mCamera;
        TSphere mBall;
};

#endif
