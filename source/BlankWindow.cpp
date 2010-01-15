#include "BlankWindow.h"

#include <SDL_opengl.h>

bool BlankWindow::onInit()
{
    mRunning = true;
    SoundEngine::loadBackgroundMusic("portal_still_alive.wav");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    int w = SDL_GetVideoSurface()->w;
    int h = SDL_GetVideoSurface()->h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    gluPerspective(FIELD_OF_VIEW, (GLdouble)w / (GLdouble)h, NEAR_CP, FAR_CP);

    glMatrixMode(GL_MODELVIEW);

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
