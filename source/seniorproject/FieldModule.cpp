#include "FieldModule.h"

#include <ctime>

FieldModule::FieldModule()
{
}

FieldModule::~FieldModule()
{
}

bool FieldModule::onLoad()
{
    srand(time(NULL));
    mField.createRandom();
    mField.dump();
    return true;
}

void FieldModule::onUnload()
{
}

void FieldModule::onOpen()
{
    mRunning = true;

    GLdouble w = SDL_GetVideoSurface()->w;
    GLdouble h = SDL_GetVideoSurface()->h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FIELD_OF_VIEW, w / h, NEAR_CP, FAR_CP);

    glMatrixMode(GL_MODELVIEW);
}

void FieldModule::onClose()
{
}

void FieldModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
}

void FieldModule::onFrame()
{
}
