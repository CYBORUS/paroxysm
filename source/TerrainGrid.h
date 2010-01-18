#ifndef TERRAINGRID_H
#define TERRAINGRID_H

#include "Matrix.h"

#include <SDL_opengl.h>

#define HEIGHT_SCALE 0.5f

class TerrainGrid
{
    public:
        TerrainGrid();
        ~TerrainGrid();

        void create(int inRows, int inCols);
        void display();
        void set(int inRow, int inCol, int inHeight, bool inFindNormal);
        void findNormal(int inRow, int inCol);

    private:
        Matrix<int> mHeights;
        GLfloat* mVertices;
        GLfloat* mNormals;
        GLuint* mIndices;
        GLsizei mNumIndices;
};

#endif
