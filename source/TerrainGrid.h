#ifndef TERRAINGRID_H
#define TERRAINGRID_H

#include "Matrix.h"
#include "Vector3D.h"
#include "DisplayEngine.h"

#include <SDL_opengl.h>

#include <iostream>
using namespace std;

#define HEIGHT_SCALE 0.5f

class TerrainGrid
{
    public:
        TerrainGrid();
        ~TerrainGrid();

        void create(int inRows, int inCols);
        void display();
        void set(int inRow, int inCol, float inHeight, bool inFindNormal);
        Vector3D<float> getVertex(int inRow, int inCol);
        const Matrix<float>& getMatrix() const;
        void findNormal(int inRow, int inCol);

    private:
        Matrix<float> mHeights;
        GLfloat* mVertices;
        GLfloat* mNormals;
        GLfloat* mTextureCoordinates;
        GLuint* mIndices;
        GLsizei mNumIndices;
        Surface mTexture;
        GLuint mTextureIndex;
};

istream& operator>>(istream& inStream, TerrainGrid& inGrid);
ostream& operator<<(ostream& inStream, const TerrainGrid& inGrid);

#endif
