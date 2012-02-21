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

#include "TerrainGrid.h"

#include <vector>
#include <fstream>
using namespace std;

GLuint TerrainGrid::mWallIndices[16] = {4, 5, 1, 0, 5, 6, 2, 1, 6, 7, 3, 2, 7,
    4, 0, 3};

TerrainGrid::TerrainGrid() : mNumIndices(0), mFriction(0.5f), mShowWall(false)
{
    glGenBuffers(4, mVertexBuffers);
    glGenTextures(1, &mWallTexture);
    DisplayEngine::loadTexture("assets/images/brick.png", mWallTexture);
}

TerrainGrid::~TerrainGrid()
{
    destroy();

    glDeleteBuffers(4, mVertexBuffers);
    glDeleteTextures(1, &mWallTexture);
}

void TerrainGrid::save(const char* inFile)
{
    ofstream lastTerrain(inFile);
    lastTerrain << *this;
    lastTerrain.close();
}

void TerrainGrid::destroy()
{
    if (mNumIndices > 0)
    {
        mNumIndices = 0;
        delete [] mVertices;
        delete [] mNormals;
        delete [] mTextureCoordinates;
        delete [] mIndices;
        SDL_FreeSurface(mTexture);
        glDeleteTextures(1, &mTextureIndex);
    }
}

void TerrainGrid::create(int inRows, int inCols)
{
    destroy();

    mHeights = Matrix<float>(inRows, inCols);
    create();
}

void TerrainGrid::create()
{
    destroy();

    mNumIndices = (mHeights.rows() - 1) * (mHeights.cols() - 1) * 6;
    mTextureCoordinates = new GLint[mHeights.size() * 2];
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
                mIndices[t++] = mHeights.toIndex(i + 1, j + 1);
                mIndices[t++] = mHeights.toIndex(i, j);
                mIndices[t++] = mHeights.toIndex(i + 1, j + 1);
                mIndices[t++] = mHeights.toIndex(i, j + 1);
            }
            else
            {
                mIndices[t++] = mHeights.toIndex(i, j + 1);
                mIndices[t++] = mHeights.toIndex(i + 1, j);
                mIndices[t++] = mHeights.toIndex(i + 1, j + 1);
                mIndices[t++] = mHeights.toIndex(i, j + 1);
            }
        }
    }

    mNumVerticesX3 = mHeights.size() * 3;
    mVertices = new GLfloat[mNumVerticesX3];
    mNormals = new GLfloat[mNumVerticesX3];

    for (int i = 0; i < mHeights.rows(); ++i)
    {
        for (int j = 0; j < mHeights.cols(); ++j)
        {
            set(i, j, mHeights(i, j), false);

            int k = mHeights.toIndex(i, j) * 2;
            mTextureCoordinates[k] = j % 2;
            mTextureCoordinates[k + 1] = i % 2;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffers[VERTEX_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mNumVerticesX3, mVertices,
        GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffers[NORMAL_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mNumVerticesX3, mNormals,
        GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffers[TEXTURE_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * mNumVerticesX3 / 3 * 2,
        mTextureCoordinates, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexBuffers[INDEX_DATA]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mNumIndices,
        mIndices, GL_STATIC_DRAW);

    for (int i = 0; i < mHeights.rows(); ++i)
    {
        for (int j = 0; j < mHeights.cols(); ++j)
        {
            findNormal(i, j);
        }
    }

    glGenTextures(1, &mTextureIndex);
    mTexture = DisplayEngine::loadImage("assets/images/green.png");
    if (!DisplayEngine::loadTexture(mTexture, mTextureIndex))
        cerr << "Error loading texture!" << endl;

    mWallHigh = 0.0f;
    mWallLow = 0.0f;

    for (int i = 0; i < mHeights.size(); ++i)
    {
        if (mHeights[i] > mWallHigh) mWallHigh = mHeights[i];
        if (mHeights[i] < mWallLow) mWallLow = mHeights[i];
    }

    if (mWallHigh < 10.0f)
        mWallHigh = 10.0f;
    else
        ++mWallHigh;

    --mWallLow;

    // point 0
    mWallVertices[0] = 0.0f;
    mWallVertices[1] = mWallHigh;
    mWallVertices[2] = 0.0f;
    mWallTextureCoords[0] = 0;
    mWallTextureCoords[1] = 0;

    // point 1
    mWallVertices[3] = mHeights.lastCol();
    mWallVertices[4] = mWallHigh;
    mWallVertices[5] = 0.0f;
    mWallTextureCoords[2] = mHeights.lastCol();
    mWallTextureCoords[3] = 0;

    // point 2
    mWallVertices[6] = mHeights.lastCol();
    mWallVertices[7] = mWallHigh;
    mWallVertices[8] = mHeights.lastRow();
    mWallTextureCoords[4] = 0;
    mWallTextureCoords[5] = 0;

    // point 3
    mWallVertices[9] = 0.0f;
    mWallVertices[10] = mWallHigh;
    mWallVertices[11] = mHeights.lastRow();
    mWallTextureCoords[6] = mHeights.lastCol();
    mWallTextureCoords[7] = 0;

    // point 4
    mWallVertices[12] = 0.0f;
    mWallVertices[13] = mWallLow;
    mWallVertices[14] = 0.0f;
    mWallTextureCoords[8] = 0;
    mWallTextureCoords[9] = GLint(mWallHigh - mWallLow);

    // point 5
    mWallVertices[15] = mHeights.lastCol();
    mWallVertices[16] = mWallLow;
    mWallVertices[17] = 0.0f;
    mWallTextureCoords[10] = mHeights.lastCol();
    mWallTextureCoords[11] = GLint(mWallHigh - mWallLow);

    // point 6
    mWallVertices[18] = mHeights.lastCol();
    mWallVertices[19] = mWallLow;
    mWallVertices[20] = mHeights.lastRow();
    mWallTextureCoords[12] = 0;
    mWallTextureCoords[13] = GLint(mWallHigh - mWallLow);

    // point 7
    mWallVertices[21] = 0.0f;
    mWallVertices[22] = mWallLow;
    mWallVertices[23] = mHeights.lastRow();
    mWallTextureCoords[14] = mHeights.lastCol();
    mWallTextureCoords[15] = GLint(mWallHigh - mWallLow);

    cerr << mWallLow << ", " << mWallHigh << endl;
}

void TerrainGrid::display()
{
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

    glEnable(GL_TEXTURE_2D);

    if (mShowWall)
    {
        glPushAttrib(GL_LIGHTING_BIT);
        glDisable(GL_LIGHTING);
        glBindTexture(GL_TEXTURE_2D, mWallTexture);
        glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        for (int i = 0; i < 16; ++i)
        {
            glTexCoord2iv(mWallTextureCoords + (mWallIndices[i] * 2));
            glVertex3fv(mWallVertices + (mWallIndices[i] * 3));
        }
        glEnd();
        glPopAttrib();
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, mTextureIndex);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffers[VERTEX_DATA]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffers[NORMAL_DATA]);
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffers[TEXTURE_DATA]);
    glTexCoordPointer(2, GL_INT, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexBuffers[INDEX_DATA]);
    glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glPopClientAttrib();

    glDisable(GL_TEXTURE_2D);

    //displayNormals();
}

void TerrainGrid::displayNormals()
{
    // visualize normal vectors (debugging)
    glPushAttrib(GL_LIGHTING_BIT);
    {
        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        {
            for (int i = 0; i < mHeights.rows(); ++i)
            {
                for (int j = 0; j < mHeights.cols(); ++j)
                {
                    int k = mHeights.toIndex(i, j) * 3;
                    GLfloat x = static_cast<GLfloat>(j);
                    GLfloat y = mVertices[k + 1];
                    GLfloat z = static_cast<GLfloat>(i);

                    glVertex3f(x, y, z);

                    x += mNormals[k];
                    y += mNormals[k + 1];
                    z += mNormals[k + 2];

                    glVertex3f(x, y, z);
                }
            }
        }
        glEnd();
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    glPopAttrib();
}

void TerrainGrid::set(int inRow, int inCol, float inHeight, bool inFindNormal)
{
    mHeights(inRow, inCol) = inHeight;

    GLfloat vertex[3];
    //GLfloat vertexNormal[3];

    int k = mHeights.toIndex(inRow, inCol) * 3;


    mVertices[k] = static_cast<GLfloat>(inCol);
    mVertices[k + 1] = inHeight;
    mVertices[k + 2] = static_cast<GLfloat>(inRow);

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

        vertex[0] = mVertices[k];
        vertex[1] = mVertices[k + 1];
        vertex[2] = mVertices[k + 2];

        //send the new vertex data to the video card
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffers[VERTEX_DATA]);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * k, sizeof(GLfloat) * 3, vertex);
    }

}

void TerrainGrid::findNormal(int inRow, int inCol)
{
    int k = mHeights.toIndex(inRow, inCol) * 3;
    int slant = ((inRow % 2) + (inCol % 2)) % 2;
    vector< Vector3D<GLfloat> > normals;

    Vector3D<GLfloat> a;
    Vector3D<GLfloat> b;
    Vector3D<GLfloat> c;
    int t;

    //cout << "\nfinding normal for row " << inRow << " col " << inCol << endl;

    if (!slant)
    {
        // center of a diamond (four triangles to average)

        if (inRow > 0)
        {
            if (inCol > 0)
            {
                // upper left triangle
                //cout << "upper left triangle" << endl;

                t = mHeights.toIndex(inRow, inCol - 1) * 3;

                a[0] = -1.0f;
                a[1] = mVertices[t + 1] - mVertices[k + 1];
                a[2] = 0.0f;

                t = mHeights.toIndex(inRow - 1, inCol) * 3;

                b[0] = 0.0f;
                b[1] = mVertices[t + 1] - mVertices[k + 1];
                b[2] = -1.0f;

                c = b ^ a;
                //cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);
            }

            if (inCol < mHeights.lastCol())
            {
                // upper right triangle
                //cout << "upper right triangle" << endl;

                t = mHeights.toIndex(inRow, inCol + 1) * 3;

                a[0] = 1.0f;
                a[1] = mVertices[t + 1] - mVertices[k + 1];
                a[2] = 0.0f;

                t = mHeights.toIndex(inRow - 1, inCol) * 3;

                b[0] = 0.0f;
                b[1] = mVertices[t + 1] - mVertices[k + 1];
                b[2] = -1.0f;

                c = a ^ b;
                //cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);
            }
        }

        if (inRow < mHeights.lastRow())
        {
            if (inCol > 0)
            {
                // lower left triangle
                //cout << "lower left triangle" << endl;

                t = mHeights.toIndex(inRow, inCol - 1) * 3;

                a[0] = -1.0f;
                a[1] = mVertices[t + 1] - mVertices[k + 1];
                a[2] = 0.0f;

                t = mHeights.toIndex(inRow + 1, inCol) * 3;

                b[0] = 0.0f;
                b[1] = mVertices[t + 1] - mVertices[k + 1];
                b[2] = 1.0f;

                c = a ^ b;
                //cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);
            }

            if (inCol < mHeights.lastCol())
            {
                // lower right triangle
                //cout << "lower right triangle" << endl;

                t = mHeights.toIndex(inRow, inCol + 1) * 3;

                a[0] = 1.0f;
                a[1] = mVertices[t + 1] - mVertices[k + 1];
                a[2] = 0.0f;

                t = mHeights.toIndex(inRow + 1, inCol) * 3;

                b[0] = 0.0f;
                b[1] = mVertices[t + 1] - mVertices[k + 1];
                b[2] = 1.0f;

                c = b ^ a;
                //cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);
            }
        }
    }
    else
    {
        // center of a square (eight triangles to average)

        if (inRow > 0)
        {
            if (inCol > 0)
            {
                // upper left triangles
                //cout << "upper left triangles" << endl;

                t = mHeights.toIndex(inRow, inCol - 1) * 3;

                a[0] = -1.0f;
                a[1] = mVertices[t + 1] - mVertices[k + 1];
                a[2] = 0.0f;

                t = mHeights.toIndex(inRow - 1, inCol - 1) * 3;

                b[0] = -1.0f;
                b[1] = mVertices[t + 1] - mVertices[k + 1];
                b[2] = -1.0f;

                c = b ^ a;
                //cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);

                t = mHeights.toIndex(inRow - 1, inCol) * 3;

                a[0] = 0.0f;
                a[1] = mVertices[t + 1] - mVertices[k + 1];
                a[2] = -1.0f;

                c = a ^ b;
                //cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);
            }

            if (inCol < mHeights.lastCol())
            {
                // upper right triangles
                //cout << "upper right triangles" << endl;
                t = mHeights.toIndex(inRow - 1, inCol) * 3;

                a[0] = 0.0f;
                a[1] = mVertices[t + 1] - mVertices[k + 1];
                a[2] = -1.0f;

                t = mHeights.toIndex(inRow - 1, inCol + 1) * 3;

                b[0] = 1.0f;
                b[1] = mVertices[t + 1] - mVertices[k + 1];
                b[2] = -1.0f;

                c = b ^ a;
                //cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);

                t = mHeights.toIndex(inRow, inCol + 1) * 3;

                a[0] = 1.0f;
                a[1] = mVertices[t + 1] - mVertices[k + 1];
                a[2] = 0.0f;

                c = a ^ b;
                //cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);
            }
        }

        if (inRow < mHeights.lastRow())
        {
            if (inCol > 0)
            {
                // lower left triangles
                //cout << "lower left triangles" << endl;

                t = mHeights.toIndex(inRow, inCol - 1) * 3;

                a[0] = -1.0f;
                a[1] = mVertices[t + 1] - mVertices[k + 1];
                a[2] = 0.0f;

                t = mHeights.toIndex(inRow + 1, inCol - 1) * 3;

                b[0] = -1.0f;
                b[1] = mVertices[t + 1] - mVertices[k + 1];
                b[2] = 1.0f;

                c = a ^ b;
                //cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);

                t = mHeights.toIndex(inRow + 1, inCol) * 3;

                a[0] = 0.0f;
                a[1] = mVertices[t + 1] - mVertices[k + 1];
                a[2] = 1.0f;

                c = b ^ a;
                //cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);
            }

            if (inCol < mHeights.lastCol())
            {
                // lower right triangles
                //cout << "lower right triangles" << endl;

                t = mHeights.toIndex(inRow, inCol + 1) * 3;

                a[0] = 1.0f;
                a[1] = mVertices[t + 1] - mVertices[k + 1];
                a[2] = 0.0f;

                t = mHeights.toIndex(inRow + 1, inCol + 1) * 3;

                b[0] = 1.0f;
                b[1] = mVertices[t + 1] - mVertices[k + 1];
                b[2] = 1.0f;

                c = b ^ a;
                //cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);

                t = mHeights.toIndex(inRow + 1, inCol) * 3;

                a[0] = 0.0f;
                a[1] = mVertices[t + 1] - mVertices[k + 1];
                a[2] = 1.0f;

                c = a ^ b;
                //cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);
            }
        }
    }

    Vector3D<GLfloat> normal;
    for (size_t i = 0; i < normals.size(); ++i) normal += normals[i];

    normal.normalize();

    //cout << "final " << normal << endl;

    mNormals[k] = normal[0];
    mNormals[k + 1] = normal[1];
    mNormals[k + 2] = normal[2];

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffers[NORMAL_DATA]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * k, sizeof(GLfloat) * 3, normal.array());

}

Vector3D<float> TerrainGrid::getVertex(int inRow, int inCol)
{
    Vector3D<float> outVector;
    int k = mHeights.toIndex(inRow, inCol) * 3;
    for (int i = 0; i < 3; ++i) outVector[i] = mVertices[k + i];
    return outVector;
}

float TerrainGrid::findHeight(float inX, float inZ)
{
    int x = int(inX);
    int z = int(inZ);

    //determine which direction the slant on each tile is
    int slant = ((z % 2) + (x % 2)) % 2;

/*
    //we're outside the bounds of the terrain
    if (x < 0 || x >= mHeights.cols() || z < 0 || z >= mHeights.rows())
        return 0.0f;
*/

    //determine where we are on an individual tile
    float xTest = inX - float(x);
    float zTest = inZ - float(z);

    enum { NORTH, SOUTH, EAST, WEST } quadrant;

    if (xTest + zTest < 1.0f)
    {
        if (xTest - zTest > 0.0f)
            quadrant = NORTH; // north (1)
        else
            quadrant = WEST; // west (2)
    }
    else
    {
        if (xTest - zTest < 0.0f)
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
                a = linearInterpolate(mHeights(z, x), mHeights(z, x + 1),
                    xTest);
                b = linearInterpolate(mHeights(z + 1, x), mHeights(z, x + 1),
                    xTest);
                t = zTest / (1.0f - xTest); //need to deal with the divide by 0 case
                break;
            }

            case SOUTH:
            case EAST:
            {
                a = linearInterpolate(mHeights(z + 1, x), mHeights(z, x + 1),
                    xTest);
                b = linearInterpolate(mHeights(z + 1, x),
                    mHeights(z + 1, x + 1), xTest);
                t = (zTest - (1.0 - xTest)) / xTest; //again, possible divide by zero
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
                a = linearInterpolate(mHeights(z, x), mHeights(z, x + 1),
                    xTest);
                b = linearInterpolate(mHeights(z, x), mHeights(z + 1, x + 1),
                    xTest);
                t = zTest / xTest;
                break;
            }

            case WEST:
            case SOUTH:
            {
                a = linearInterpolate(mHeights(z + 1, x), mHeights(z + 1, x + 1),
                    xTest);
                b = linearInterpolate(mHeights(z, x), mHeights(z + 1, x + 1),
                    xTest);
                t = (1.0 - zTest) / (1.0 - xTest);
                break;
            }

            default:
            {
            }
        }
    }

    return linearInterpolate(a, b, t);
}

istream& operator>>(istream& inStream, TerrainGrid& inGrid)
{
    inStream >> inGrid.mHeights;
    inGrid.create();
    return inStream;
}

ostream& operator<<(ostream& inStream, const TerrainGrid& inGrid)
{
    inStream << inGrid.mHeights.rows() << ' ' << inGrid.mHeights.cols();

    for (int i = 0; i < inGrid.mHeights.size(); ++i)
        inStream << ' ' << inGrid.mHeights[i];

    return inStream;
}
