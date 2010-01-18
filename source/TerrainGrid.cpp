#include "TerrainGrid.h"
#include "Vector3D.h"

#include <vector>
using namespace std;

TerrainGrid::TerrainGrid() : mNumIndices(0)
{
}

TerrainGrid::~TerrainGrid()
{
    if (mNumIndices > 0)
    {
        delete [] mVertices;
        delete [] mNormals;
        delete [] mIndices;
    }
}

void TerrainGrid::create(int inRows, int inCols)
{
    mHeights = Matrix<int>(inRows, inCols);

    for (int i = 0; i < mHeights.size(); ++i)
        mHeights[i] = rand() % 3;

    mNumIndices = (mHeights.rows() - 1) * (mHeights.cols() - 1) * 6;
    mIndices = new GLuint[mNumIndices];

    int t = 0;
    for (int i = 0; i < mHeights.rows() - 1; ++i)
    {
        for (int j = 0; j < mHeights.cols() - 1; ++j)
        {
            int slant = ((i % 2) + (j % 2)) % 2;

            mIndices[t++] = mHeights.toIndex(i, j);
            mIndices[t++] = mHeights.toIndex(i + 1, j);

            if (slant)
            {
                mIndices[t++] = mHeights.toIndex(i, j + 1);
                mIndices[t++] = mHeights.toIndex(i + 1, j);
                mIndices[t++] = mHeights.toIndex(i + 1, j + 1);
                mIndices[t++] = mHeights.toIndex(i, j + 1);
            }
            else
            {
                mIndices[t++] = mHeights.toIndex(i + 1, j + 1);
                mIndices[t++] = mHeights.toIndex(i, j);
                mIndices[t++] = mHeights.toIndex(i + 1, j + 1);
                mIndices[t++] = mHeights.toIndex(i, j + 1);
            }
        }
    }

    int numVertices = mHeights.size() * 3;
    mVertices = new GLfloat[numVertices];
    mNormals = new GLfloat[numVertices];

    for (int i = 0; i < mHeights.rows(); ++i)
    {
        for (int j = 0; j < mHeights.cols(); ++j)
        {
            int k = mHeights.toIndex(i, j) * 3;
            mVertices[k] = static_cast<GLfloat>(j);
            mVertices[k + 1] = static_cast<GLfloat>(mHeights(i, j))
                * HEIGHT_SCALE;
            mVertices[k + 2] = static_cast<GLfloat>(i);

            mNormals[k] = 0.0f;
            mNormals[k + 1] = 1.0f;
            mNormals[k + 2] = 0.0f;
        }
    }

    for (int i = 0; i < mHeights.rows(); ++i)
    {
        for (int j = 0; j < mHeights.cols(); ++j)
        {
            findNormal(i, j);
        }
    }
}

void TerrainGrid::display()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, mVertices);
    glNormalPointer(GL_FLOAT, 0, mNormals);
    glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT,
        mIndices);
    //glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void TerrainGrid::set(int inRow, int inCol, int inHeight)
{
    mHeights(inRow, inCol) = inHeight;
    findNormal(inRow, inCol);
}

void TerrainGrid::findNormal(int inRow, int inCol)
{
    int k = mHeights.toIndex(inRow, inCol) * 3;
    int slant = ((inRow % 2) + (inCol % 2)) % 2;
    vector< Vector3D<GLfloat> > normals;

    Vector3D<GLfloat> a;
    Vector3D<GLfloat> b;
    Vector3D<GLfloat> c;

    if (slant)
    {
        if (inRow > 0)
        {
        }
        else
        {
        }
    }
    else
    {
    }

    Vector3D<GLfloat> normal;
    for (int i = 0; i < normals.size(); ++i) normal += normals[i];

    normal.normalize();

    mNormals[k] = normal[0];
    mNormals[k + 1] = normal[1];
    mNormals[k + 2] = normal[2];
}
