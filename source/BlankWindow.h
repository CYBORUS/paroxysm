#ifndef _BLANKWINDOW_H_
#define _BLANKWINDOW_H_

#include "Module.h"
#include "SoundEngine.h"
#include "DisplayEngine.h"
#include "TSphere.h"

class BlankWindow : public Module
{
    public:
        /// module operation
        virtual bool onInit();
        virtual void onCleanup();

        void onLoop();

        void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
    private:
        TSphere mSphere;
};

#endif
