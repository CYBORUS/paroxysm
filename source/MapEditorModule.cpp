#include "MapEditorModule.h"
#include "DisplayEngine.h"

#include <SDL_opengl.h>

bool MapEditorModule::onInit()
{
    mRunning = true;
    mTerrain = Matrix<int>(DEFAULT_TERRAIN_SIZE, DEFAULT_TERRAIN_SIZE);

    int w = SDL_GetVideoSurface()->w;
    int h = SDL_GetVideoSurface()->h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    gluPerspective(FIELD_OF_VIEW, (GLdouble)w / (GLdouble)h, NEAR_CP, FAR_CP);

    return true;
}

void MapEditorModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MapEditorModule::onFrame()
{
}

void MapEditorModule::onCleanup()
{
}
