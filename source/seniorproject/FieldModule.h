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
        virtual void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);

    private:
        WallField mField;
        WallField::Direction* mPath;
        size_t mPathSize;
        size_t mCurrentStep;
        Uint32 mTimer;
        GameCamera mCamera;
        TSphere mBall;
        float mSpin;
        float mRise;
        float mZoom;
        Point2D<Uint32> mPosition;
};

#endif
