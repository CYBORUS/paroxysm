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

TerrainGrid::TerrainGrid() : mNumIndices(0)
{
    //glGenBuffers(4, mVertexBuffers);
}

TerrainGrid::~TerrainGrid()
{
    destroy();

    //glDeleteBuffers(4, mVertexBuffers);
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
        save("last_terrain.txt");

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
    mTextureCoordinates = new GLfloat[mHeights.size() * 2];
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

    int numVertices = mHeights.size() * 3;
    mVertices = new GLfloat[numVertices];
    mNormals = new GLfloat[numVertices];

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

    for (int i = 0; i < mHeights.rows(); ++i)
    {
        for (int j = 0; j < mHeights.cols(); ++j)
        {
            findNormal(i, j);
        }
    }

    glGenTextures(1, &mTextureIndex);
    mTexture = DisplayEngine::loadImage("./assets/images/green.png");
    if (!DisplayEngine::loadTexture(mTexture, mTextureIndex))
        cerr << "Error loading texture!" << endl;
/*
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffers[VERTEX_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertices * 3, mVertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffers[NORMAL_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertices * 3, mNormals, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffers[TEXTURE_DATA]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertices * 3, mTextureCoordinates, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexBuffers[INDEX_DATA]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numVertices, mIndices, GL_DYNAMIC_DRAW);
    */
}

void TerrainGrid::display()
{

    //glPushAttrib(GL_POLYGON_BIT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D, mTextureIndex);
    glVertexPointer(3, GL_FLOAT, 0, mVertices);
    glNormalPointer(GL_FLOAT, 0, mNormals);
    glTexCoordPointer(2, GL_FLOAT, 0, mTextureCoordinates);
    glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT,
        mIndices);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    //glDisableClientState(GL_NORMAL_ARRAY);
    //glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glPopClientAttrib();

/*
    glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mTextureIndex);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffers[VERTEX_DATA]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffers[NORMAL_DATA]);
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffers[TEXTURE_DATA]);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVertexBuffers[INDEX_DATA]);
    glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT, 0);


    glDisable(GL_TEXTURE_2D);
    glPopClientAttrib();
*/
    //displayNormals();
    //glPopAttrib();
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
    for (unsigned int i = 0; i < normals.size(); ++i) normal += normals[i];

    normal.normalize();

    //cout << "final " << normal << endl;

    mNormals[k] = normal[0];
    mNormals[k + 1] = normal[1];
    mNormals[k + 2] = normal[2];
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
