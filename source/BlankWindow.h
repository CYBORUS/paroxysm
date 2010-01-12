#ifndef _BLANKWINDOW_H_
#define _BLANKWINDOW_H_

#include "Module.h"
#include "SoundEngine.h"

class BlankWindow : public Module
{
    public:
        /// module operation
        virtual bool onInit();
        virtual void onCleanup();

        void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
    private:
};

#endif // _BLANKWINDOW_H_
