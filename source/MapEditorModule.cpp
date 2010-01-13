#include "MapEditorModule.h"

#include <SDL_opengl.h>

bool MapEditorModule::onInit()
{
    mRunning = true;
    mTerrain = Matrix<int>(DEFAULT_TERRAIN_SIZE, DEFAULT_TERRAIN_SIZE);

    int w = SDL_GetVideoSurface()->w;
    int h = SDL_GetVideoSurface()->h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    //gluPerspective(fov, (GLfloat)w / (GLfloat)h, near, far);

    return true;
}

void MapEditorModule::onLoop()
{
}

void MapEditorModule::onFrame()
{
}

void MapEditorModule::onCleanup()
{
}
