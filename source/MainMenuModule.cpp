#include "MainMenuModule.h"

bool MainMenuModule::onInit()
{
    mRunning = true;
    mNextModule = NULL;


    GLdouble ratio = 0;
    int w = SDL_GetVideoSurface()->w;

    int h = SDL_GetVideoSurface()->h;


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    //gluPerspective(FIELD_OF_VIEW, (GLdouble)w / (GLdouble)h, NEAR_CP, FAR_CP);

    //we need to deal with the possibility of a monitor on it's side
    if (w >= h)
    {
        ratio = (GLdouble)w / (GLdouble)h;
        glOrtho(-RANGE * ratio, RANGE * ratio, -RANGE, RANGE, 1.0, 10.0);
    }
    else
    {
        ratio = (GLdouble)h / (GLdouble)w;
        glOrtho(-RANGE, RANGE, -RANGE * ratio, RANGE * ratio, 1.0, 10.0);
    }

    glMatrixMode(GL_MODELVIEW);


}

void MainMenuModule::onLoop()
{
}

void MainMenuModule::onFrame()
{
}

void MainMenuModule::onCleanup()
{
}

Module* MainMenuModule::next()
{
    return mNextModule;
}

