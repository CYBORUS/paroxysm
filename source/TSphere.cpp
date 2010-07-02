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

#include "TSphere.h"
#include "DisplayEngine.h"

#include <vector>

#define IX 0.525731112119133606
#define IZ 0.850650808352039932

GLfloat TSphere::mVData[12][3] =
    {{-IX, 0.0, IZ}, {IX, 0.0, IZ}, {-IX, 0.0, -IZ}, {IX, 0.0, -IZ},
     {0.0, IZ, IX}, {0.0, IZ, -IX}, {0.0, -IZ, IX}, {0.0, -IZ, -IX},
     {IZ, IX, 0.0}, {-IZ, IX, 0.0}, {IZ, -IX, 0.0}, {-IZ, -IX, 0.0}};
GLuint TSphere::mTIndices[20][3] =
    {{1, 4, 0}, {4, 9, 0}, {4, 5, 9}, {8, 5, 4}, {1, 8, 4},
     {1, 10, 8}, {10, 3, 8}, {8, 3, 5}, {3, 2, 5}, {3, 7, 2},
     {3, 10, 7}, {10, 6, 7}, {6, 11, 7}, {6, 0, 11}, {6, 1, 0},
     {10, 1, 6}, {11, 0, 9}, {2, 11, 9}, {5, 2, 9}, {11, 2, 7}};

TSphere::TSphere(int inDetail) : mScale(1.0, 1.0, 1.0), mDetail(inDetail)
{
    mFill = true;
    mColor[0].set(1.0f, 1.0f, 1.0f);

    mCurrentColor = 1;

    for (int i = 0; i < 20; ++i)
    {
        subdivide(&mVData[mTIndices[i][2]][0], &mVData[mTIndices[i][1]][0],
            &mVData[mTIndices[i][0]][0], mDetail);
    }

    mVBO.loadVertexArray(PVBO_VERTEX, 3, mVertices.size(), &mVertices[0]);
    mVBO.loadVertexArray(PVBO_NORMAL, 3, mVertices.size(), &mVertices[0]);
    mVBO.loadIndexArray(GL_TRIANGLES, mIndices.size(), &mIndices[0]);
}

TSphere::~TSphere()
{
}

void TSphere::display()
{
    glPushAttrib(GL_CURRENT_BIT);
    {
        glColor3fv(mColor[0].array());
        glPushMatrix();
        {
            glTranslatef(mTranslation[0], mTranslation[1], mTranslation[2]);
            glScalef(mScale[0], mScale[1], mScale[2]);
            mVBO.display();
        }
        glPopMatrix();
    }
    glPopAttrib();
}

void TSphere::moveSphere(float inX, float inY, float inZ)
{
    mTranslation[0] = inX;
    mTranslation[1] = inY;
    mTranslation[2] = inZ;
}

void TSphere::drawTriangle(float* inA, float* inB, float* inC)
{
    mIndices.push_back(mVertices.size() / 3u);
    for (int i = 0; i < 3; ++i) mVertices.push_back(inA[i]);

    mIndices.push_back(mVertices.size() / 3u);
    for (int i = 0; i < 3; ++i) mVertices.push_back(inC[i]);

    mIndices.push_back(mVertices.size() / 3u);
    for (int i = 0; i < 3; ++i) mVertices.push_back(inB[i]);
}

void TSphere::drawTriangle(Vector3D<float>& inA, Vector3D<float>& inB,
    Vector3D<float>& inC)
{
    drawTriangle(inA.array(), inB.array(), inC.array());
}

void TSphere::subdivide(float* inA, float* inB, float* inC, int inDepth)
{
    Vector3D<float> v12;
    Vector3D<float> v23;
    Vector3D<float> v31;

    if (inDepth == 0)
    {
        drawTriangle(inA, inB, inC);
        return;
    }

    for (int i = 0; i < 3; ++i)
    {
        v12[i] = (inA[i] + inB[i]) / 2.0f;
        v23[i] = (inB[i] + inC[i]) / 2.0f;
        v31[i] = (inC[i] + inA[i]) / 2.0f;
    }

    v12.normalize();
    v23.normalize();
    v31.normalize();
    subdivide(inA, v12.array(), v31.array(), inDepth - 1);
    subdivide(inB, v23.array(), v12.array(), inDepth - 1);
    subdivide(inC, v31.array(), v23.array(), inDepth - 1);
    subdivide(v12.array(), v23.array(), v31.array(), inDepth - 1);
}

void TSphere::swapColor()
{
    mCurrentColor = 1 - mCurrentColor;
    glColor3fv(mColor[mCurrentColor].array());
}

void TSphere::setColor(float inR, float inG, float inB)
{
    mColor[0].set(inR, inG, inB);
}

void TSphere::setScale(float inX, float inY, float inZ)
{
    mScale.set(inX, inY, inZ);
}

void TSphere::setScale(float inScale)
{
    mScale.set(inScale);
}
