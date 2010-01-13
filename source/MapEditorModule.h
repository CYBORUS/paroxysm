#ifndef MAPEDITORMODULE_H
#define MAPEDITORMODULE_H

#include "Module.h"
#include "Matrix.h"

#include <SDL_opengl.h>

#define DEFAULT_TERRAIN_SIZE 40

class MapEditorModule : public Module
{
    public:
        /// module operation
        virtual bool onInit() = 0;
        virtual void onLoop();
        virtual void onFrame();
        virtual void onCleanup() = 0;

    private:
        Matrix<int> mTerrainHeight;
        GLfloat* mTerrainVertices;
        GLubyte* mTerrainIndices;
};

#endif
