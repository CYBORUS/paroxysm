#include "BlankWindow.h"

bool BlankWindow::onInit()
{
    mRunning = true;

    return true;
}

void BlankWindow::onCleanup()
{
}


void BlankWindow::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
   cerr << "keydown" << endl;
   SoundEngine::playBackgroundMusic();
}