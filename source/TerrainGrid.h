/**
 *  This file is part of "Paroxysm".
 *
 *  "Paroxysm" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Paroxysm" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with "Paroxysm".  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TERRAINGRID_H
#define TERRAINGRID_H

#include "Matrix.h"
#include "Vector3D.h"
#include "DisplayEngine.h"
#define GL_GLEXT_PROTOTYPES

#include "OGL.h"

#include <iostream>
using namespace std;

#define HEIGHT_SCALE 0.5f

#define VERTEX_DATA 0
#define NORMAL_DATA 1
#define TEXTURE_DATA 2
#define INDEX_DATA 3

class TerrainGrid
{
    public:
        TerrainGrid();
        ~TerrainGrid();

        void create(int inRows, int inCols);
        void create();
        void destroy();
        void save(const char* inFile);
        void display();
        void set(int inRow, int inCol, float inHeight,
            bool inFindNormal = true);
        Vector3D<float> getVertex(int inRow, int inCol);
        const Matrix<float>& getMatrix() const;
        void findNormal(int inRow, int inCol);
        float findHeight(float inX, float inZ);

        friend istream& operator>>(istream& inStream, TerrainGrid& inGrid);
        friend ostream& operator<<(ostream& inStream,
            const TerrainGrid& inGrid);

    private:
        void displayNormals();

        Matrix<float> mHeights;
        GLfloat* mVertices;
        GLfloat* mNormals;
        GLint* mTextureCoordinates;
        GLuint* mIndices;
        GLsizei mNumIndices;
        GLuint mNumVerticesX3;
        Surface mTexture;
        GLuint mTextureIndex;

        GLuint mVertexBuffers[4];
};

inline const Matrix<float>& TerrainGrid::getMatrix() const
{
    return mHeights;
}

#endif
