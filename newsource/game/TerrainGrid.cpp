#include "TerrainGrid.h"
#include <CGE/Exception.h>

using namespace CGE;

TerrainGrid::TerrainGrid() : mVBO(2), mHeights(NULL), mRows(0), mCols(0),
    mSize(0)
{
    mTexture.loadImage("assets/images/green.png");
}

TerrainGrid::~TerrainGrid()
{
}

void TerrainGrid::display()
{
//    mVertexVBO.enableVAA(0);
//    mTextureVBO.enableVAA(1);
//    mIVBO.draw();
//    mTextureVBO.disableVAA();
//    mVertexVBO.disableVAA();
    mTexture.bind();
    mVBO.display();
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
    mHeights = Matrix<float>(inRows, inCols);
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

            indices[t++] = mHeights.toIndex(i, j);
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

    mVBO.mount(mIVBO);
    mVBO.mount(mVertexVBO, 0);
    mVBO.mount(mTextureVBO, 1);

    delete [] normals;
    delete [] vertices;
    delete [] textureCoordinates;
    delete [] indices;
}


void TerrainGrid::set(int inRow, int inCol, float inHeight, bool inFindNormal)
{
    mHeights(inRow, inCol) = inHeight;

    GLfloat vertex[3];
    //GLfloat vertexNormal[3];

    int k = mHeights(inRow, inCol) * 3;

    vertex[0] = static_cast<GLfloat>(inCol);
    vertex[1] = inHeight;
    vertex[2] = static_cast<GLfloat>(inRow);

//    mVertices[k] = static_cast<GLfloat>(inCol);
//    mVertices[k + 1] = inHeight;
//    mVertices[k + 2] = static_cast<GLfloat>(inRow);

    if (inFindNormal)
    {
        findNormal(inRow, inCol);

        if (inRow > 0)
        {
            findNormal(inRow - 1, inCol);

            if (inCol > 0)
            {
                findNormal(inRow - 1, inCol - 1);
            }

            if (inCol < mHeights.lastCol())
            {
                findNormal(inRow - 1, inCol + 1);
            }
        }

        if (inRow < mHeights.lastRow())
        {
            findNormal(inRow + 1, inCol);

            if (inCol > 0)
            {
                findNormal(inRow + 1, inCol - 1);
            }

            if (inCol < mHeights.lastCol())
            {
                findNormal(inRow + 1, inCol + 1);
            }
        }

        if (inCol > 0)
        {
            findNormal(inRow, inCol - 1);
        }

        if (inCol < mHeights.lastCol())
        {
            findNormal(inRow, inCol + 1);
        }

//        vertex[0] = mVertices[k];
//        vertex[1] = mVertices[k + 1];
//        vertex[2] = mVertices[k + 2];

        //send the new vertex data to the video card
        mVertexVBO.editData(vertex, k);
//        glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffers[VERTEX_DATA]);
//        glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * k, sizeof(GLfloat) * 3, vertex);
    }

}


float TerrainGrid::findHeight(float inX, float inY)
{
    int x = int(inX);
    int y = int(inY);

    //determine which direction the slant on each tile is
    int slant = ((y % 2) + (x % 2)) % 2;

/*
    //we're outside the bounds of the terrain
    if (x < 0 || x >= mHeights.cols() || z < 0 || z >= mHeights.rows())
        return 0.0f;
*/

    //determine where we are on an individual tile
    float xTest = inX - float(x);
    float yTest = inY - float(y);

    enum { NORTH, SOUTH, EAST, WEST } quadrant;

    if (xTest + yTest < 1.0f)
    {
        if (xTest - yTest > 0.0f)
            quadrant = NORTH; // north (1)
        else
            quadrant = WEST; // west (2)
    }
    else
    {
        if (xTest - yTest < 0.0f)
            quadrant = SOUTH; // south (3)
        else
            quadrant = EAST; // east (4)
    }

    float a = 0.0f;
    float b = 0.0f;
    float t = 0.0f;

    if (!slant)
    {
        switch (quadrant)
        {
            case NORTH:
            case WEST:
            {
                a = linearInterpolate(toIndex(y, x), toIndex(y, x + 1),
                    xTest);
                b = linearInterpolate(toIndex(y + 1, x), toIndex(y, x + 1),
                    xTest);
                t = yTest / (1.0f - xTest); //need to deal with the divide by 0 case
                break;
            }

            case SOUTH:
            case EAST:
            {
                a = linearInterpolate(toIndex(y + 1, x), toIndex(y, x + 1),
                    xTest);
                b = linearInterpolate(toIndex(y + 1, x),
                    toIndex(y + 1, x + 1), xTest);
                t = (yTest - (1.0 - xTest)) / xTest; //again, possible divide by zero
                break;
            }

            default:
            {
            }
        }
    }
    else
    {
        switch (quadrant)
        {
            case NORTH:
            case EAST:
            {
                a = linearInterpolate(toIndex(y, x), toIndex(y, x + 1),
                    xTest);
                b = linearInterpolate(toIndex(y, x), toIndex(y + 1, x + 1),
                    xTest);
                t = yTest / xTest;
                break;
            }

            case WEST:
            case SOUTH:
            {
                a = linearInterpolate(toIndex(y + 1, x), toIndex(y + 1, x + 1),
                    xTest);
                b = linearInterpolate(toIndex(y, x), toIndex(y + 1, x + 1),
                    xTest);
                t = (1.0 - yTest) / (1.0 - xTest);
                break;
            }

            default:
            {
            }
        }
    }

    return linearInterpolate(a, b, t);
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
