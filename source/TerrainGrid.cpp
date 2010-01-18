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
            set(i, j, rand() % 3, false);

            //mNormals[k] = 0.0f;
            //mNormals[k + 1] = 1.0f;
            //mNormals[k + 2] = 0.0f;
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
    glEnableClientState(GL_NORMAL_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, mVertices);
    glNormalPointer(GL_FLOAT, 0, mNormals);
    glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT,
        mIndices);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

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

void TerrainGrid::set(int inRow, int inCol, int inHeight,
    bool inFindNormal = true)
{
    mHeights(inRow, inCol) = inHeight;

    int k = mHeights.toIndex(inRow, inCol) * 3;
    mVertices[k] = static_cast<GLfloat>(inCol);
    mVertices[k + 1] = static_cast<GLfloat>(mHeights(inRow, inCol))
        * HEIGHT_SCALE;
    mVertices[k + 2] = static_cast<GLfloat>(inRow);

    if (inFindNormal) findNormal(inRow, inCol);
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

    cout << "finding normal for row " << inRow << " col " << inCol << endl;

    if (slant)
    {
        // center of a diamond (four triangles to average)

        if (inRow > 0)
        {
            if (inCol > 0)
            {
                // upper left triangle
                t = mHeights.toIndex(inRow, inCol - 1) * 3;

                a[0] = -1.0f;
                a[1] = mNormals[t + 1] - mNormals[k + 1];
                a[2] = 0.0f;

                t = mHeights.toIndex(inRow - 1, inCol) * 3;

                b[0] = 0.0f;
                b[1] = mNormals[t + 1] - mNormals[k + 1];
                b[2] = -1.0f;

                c = b ^ a;
                cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);
            }

            if (inCol < mHeights.lastCol())
            {
                // upper right triangle
                t = mHeights.toIndex(inRow, inCol + 1) * 3;

                a[0] = 1.0f;
                a[1] = mNormals[t + 1] - mNormals[k + 1];
                a[2] = 0.0f;

                t = mHeights.toIndex(inRow - 1, inCol) * 3;

                b[0] = 0.0f;
                b[1] = mNormals[t + 1] - mNormals[k + 1];
                b[2] = -1.0f;

                c = a ^ b;
                cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);
            }
        }

        if (inRow < mHeights.lastRow())
        {
            if (inCol > 0)
            {
                // lower left triangle
                t = mHeights.toIndex(inRow, inCol - 1) * 3;

                a[0] = -1.0f;
                a[1] = mNormals[t + 1] - mNormals[k + 1];
                a[2] = 0.0f;

                t = mHeights.toIndex(inRow + 1, inCol) * 3;

                b[0] = 0.0f;
                b[1] = mNormals[t + 1] - mNormals[k + 1];
                b[2] = -1.0f;

                c = a ^ b;
                cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);
            }

            if (inCol < mHeights.lastCol())
            {
                // lower right triangle
                t = mHeights.toIndex(inRow, inCol + 1) * 3;

                a[0] = 1.0f;
                a[1] = mNormals[t + 1] - mNormals[k + 1];
                a[2] = 0.0f;

                t = mHeights.toIndex(inRow + 1, inCol) * 3;

                b[0] = 0.0f;
                b[1] = mNormals[t + 1] - mNormals[k + 1];
                b[2] = -1.0f;

                c = b ^ a;
                cout << "abc: " << a << " | " << b << " | " << c << endl;
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
                t = mHeights.toIndex(inRow, inCol - 1) * 3;

                a[0] = -1.0f;
                a[1] = mNormals[t + 1] - mNormals[k + 1];
                a[2] = 0.0f;

                t = mHeights.toIndex(inRow - 1, inCol - 1) * 3;

                b[0] = -1.0f;
                b[1] = mNormals[t + 1] - mNormals[k + 1];
                b[2] = -1.0f;

                c = b ^ a;
                cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);

                t = mHeights.toIndex(inRow - 1, inCol) * 3;

                a[0] = 0.0f;
                a[1] = mNormals[t + 1] - mNormals[k + 1];
                a[2] = -1.0f;

                c = a ^ b;
                cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);
            }

            if (inCol < mHeights.lastCol())
            {
                // upper right triangles
                t = mHeights.toIndex(inRow - 1, inCol) * 3;

                a[0] = 0.0f;
                a[1] = mNormals[t + 1] - mNormals[k + 1];
                a[2] = -1.0f;

                t = mHeights.toIndex(inRow - 1, inCol + 1) * 3;

                b[0] = 1.0f;
                b[1] = mNormals[t + 1] - mNormals[k + 1];
                b[2] = -1.0f;

                c = b ^ a;
                cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);

                t = mHeights.toIndex(inRow, inCol + 1) * 3;

                a[0] = 1.0f;
                a[1] = mNormals[t + 1] - mNormals[k + 1];
                a[2] = 0.0f;

                c = a ^ b;
                cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);
            }
        }

        if (inRow < mHeights.lastRow())
        {
            if (inCol > 0)
            {
                // lower left triangles
                t = mHeights.toIndex(inRow, inCol - 1) * 3;

                a[0] = -1.0f;
                a[1] = mNormals[t + 1] - mNormals[k + 1];
                a[2] = 0.0f;

                t = mHeights.toIndex(inRow + 1, inCol - 1) * 3;

                b[0] = -1.0f;
                b[1] = mNormals[t + 1] - mNormals[k + 1];
                b[2] = 1.0f;

                c = a ^ b;
                cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);

                t = mHeights.toIndex(inRow + 1, inCol) * 3;

                a[0] = 0.0f;
                a[1] = mNormals[t + 1] - mNormals[k + 1];
                a[2] = 1.0f;

                c = b ^ a;
                cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);
            }

            if (inCol < mHeights.lastCol())
            {
                // lower right triangles
                t = mHeights.toIndex(inRow, inCol + 1) * 3;

                a[0] = 1.0f;
                a[1] = mNormals[t + 1] - mNormals[k + 1];
                a[2] = 0.0f;

                t = mHeights.toIndex(inRow + 1, inCol + 1) * 3;

                b[0] = 1.0f;
                b[1] = mNormals[t + 1] - mNormals[k + 1];
                b[2] = 1.0f;

                c = b ^ a;
                cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);

                t = mHeights.toIndex(inRow + 1, inCol) * 3;

                a[0] = 0.0f;
                a[1] = mNormals[t + 1] - mNormals[k + 1];
                a[2] = 1.0f;

                c = a ^ b;
                cout << "abc: " << a << " | " << b << " | " << c << endl;
                c.normalize();
                normals.push_back(c);
            }
        }
    }

    Vector3D<GLfloat> normal;
    for (unsigned int i = 0; i < normals.size(); ++i) normal += normals[i];

    normal.normalize();

    cout << "final " << normal << endl;

    mNormals[k] = normal[0];
    mNormals[k + 1] = normal[1];
    mNormals[k + 2] = normal[2];
}
