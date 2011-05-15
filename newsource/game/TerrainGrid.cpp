#include "TerrainGrid.h"
#include <CGE/Exception.h>

TerrainGrid::TerrainGrid() : mHeights(NULL), mRows(0), mCols(0), mSize(0)
{
}

TerrainGrid::~TerrainGrid()
{
}

void TerrainGrid::display()
{
    mVertexVBO.enableVAA(0);
    mTextureVBO.enableVAA(1);
    mIVBO.draw();
    mTextureVBO.disableVAA();
    mVertexVBO.disableVAA();
}

size_t TerrainGrid::toIndex(size_t inRow, size_t inCol)
{
    return inRow * mCols + inCol;
}

void TerrainGrid::create(size_t inRows, size_t inCols)
{
    static const char* functionName = "TerrainGrid::create";

    if (!inRows || !inCols)
        throw CGE::Exception(functionName, "invalid size parameters");

    destroy();

    mRows = inRows;
    mCols = inCols;
    mSize = mRows * mCols;
    mHeights = new float[mSize];
    memset(mHeights, 0, mSize * sizeof(float));
}

void TerrainGrid::buildVBO()
{
    /// 1 quad = 2 triangles = 6 indices per square (3 indices per triangle)
    size_t numIndices = (mRows - 1) * (mCols - 1) * 6;
    GLuint* indices = new GLuint[numIndices];

    /// one texture (UV) coordinate for every point on the field
    GLfloat* textureCoordinates = new GLfloat[mSize * 2];

    size_t t = 0;
    for (size_t i = 0; i < mRows - 1; ++i)
    {
        for (size_t j = 0; j < mCols - 1; ++j)
        {
            size_t slant = ((i % 2) + (j % 2)) % 2;

            indices[t++] = toIndex(i, j);
            indices[t++] = toIndex(i + 1, j);

            if (slant)
            {
                indices[t++] = toIndex(i + 1, j + 1);
                indices[t++] = toIndex(i, j);
                indices[t++] = toIndex(i + 1, j + 1);
                indices[t++] = toIndex(i, j + 1);
            }
            else
            {
                indices[t++] = toIndex(i, j + 1);
                indices[t++] = toIndex(i + 1, j);
                indices[t++] = toIndex(i + 1, j + 1);
                indices[t++] = toIndex(i, j + 1);
            }
        }
    }

    GLfloat* vertices = new GLfloat[mSize * 3];
    GLfloat* normals = new GLfloat[mSize * 3];

    for (size_t i = 0; i < mRows; ++i)
    {
        for (size_t j = 0; j < mCols; ++j)
        {
            size_t k = toIndex(i, j);
            GLfloat* v = vertices + (k * 3);
            v[0] = static_cast<GLfloat>(j);
            v[1] = static_cast<GLfloat>(i);
            v[2] = mHeights[k];

            k *= 2;
            textureCoordinates[k] = static_cast<GLfloat>(j % 2);
            textureCoordinates[k + 1] = static_cast<GLfloat>(i % 2);
        }
    }

    //mVBO.loadVAA(0, 3, mSize, vertices);
    mVertexVBO.loadData(vertices, mSize, 3);
    //mVBO.loadVAA(1, 2, mSize, textureCoordinates);
    mTextureVBO.loadData(textureCoordinates, mSize, 2);
    mIVBO.loadData(indices, numIndices);
    //mIVBO.loadData(GL_TRIANGLES, numIndices, indices);

    delete [] normals;
    delete [] vertices;
    delete [] textureCoordinates;
    delete [] indices;
}

void TerrainGrid::destroy()
{
    if (mHeights)
    {
        delete [] mHeights;
        mHeights = NULL;
        mRows = 0;
        mCols = 0;
        mSize = 0;
    }
}

std::istream& operator>>(std::istream& inStream, TerrainGrid& inTerrainGrid)
{
    size_t rows;
    size_t cols;
    inStream >> rows >> cols;
    inTerrainGrid.create(rows, cols);

    for (size_t i = 0; i < inTerrainGrid.mSize; ++i)
        inStream >> inTerrainGrid.mHeights[i];

    inTerrainGrid.buildVBO();

    return inStream;
}

std::ostream& operator<<(std::ostream& inStream, const TerrainGrid&
    inTerrainGrid)
{
    inStream << inTerrainGrid.mRows << ' ' << inTerrainGrid.mCols;

    for (size_t i = 0; i < inTerrainGrid.mSize; ++i)
        inStream << ' ' << inTerrainGrid.mHeights[i];

    return inStream;
}
