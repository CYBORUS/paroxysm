#include "TSphere.h"

#define IX 0.525731112119133606
#define IZ 0.850650808352039932

GLfloat TSphere::mVData[12][3] =
    {{-IX, 0.0, IZ}, {IX, 0.0, IZ}, {-IX, 0.0, -IZ}, {IX, 0.0, -IZ},
     {0.0, IZ, IX}, {0.0, IZ, -IX}, {0.0, -IZ, IX}, {0.0, -IZ, -IX},
     {IZ, IX, 0.0}, {-IZ, IX, 0.0}, {IZ, -IX, 0.0}, {-IZ, -IX, 0.0}};
GLuint TSphere::mTIndices[20][3] =
    {{1, 4, 0}, {4, 9, 0}, {4, 5, 9}, {8, 5, 4}, {1, 8, 4},
     {1, 10, 8}, {10, 3, 8}, {8, 3, 5}, {3, 2, 5}, {3, 7, 2},
     {3, 10, 7}, {10, 6, 7}, {6,  11, 7}, {6, 0, 11}, {6, 1, 0},
     {10, 1, 6}, {11, 0, 9}, {2, 11, 9}, {5, 2, 9}, {11, 2, 7}};

void TSphere::setup()
{
    mFill = true;

/*
    string fill;
    addToStream(inNode, "fill", "false");
    addToStream(inNode, "color0", "0.5,0.5,0.5");
    addToStream(inNode, "color1", "1,1,1");
    addToStream(inNode, "detail", "4");
    mStream >> fill >> mColor[0] >> mColor[1] >> mDetail;
    if (fill == "true") mFill = true;
*/
    mDetail = 4;

    mID = glGenLists(1);
    mCurrentColor = 1;

    glNewList(mID, GL_COMPILE);
    {
        glBegin(GL_TRIANGLES);
        for (int i = 0; i < 20; ++i)
        {
            subdivide(&mVData[mTIndices[i][2]][0], &mVData[mTIndices[i][1]][0],
                &mVData[mTIndices[i][0]][0], mDetail);
        }
        glEnd();
    }
    glEndList();
}

void TSphere::display()
{
    glPushAttrib(GL_POLYGON_BIT);
    {
        glPolygonMode(GL_FRONT, GL_FILL);
        if (!mFill)
        {
            glPolygonMode(GL_FRONT, GL_LINE);
        }
        else
        {
            glShadeModel(GL_FLAT);
        }

        glPushMatrix();
        {
            glTranslatef(mTranslation[0], mTranslation[1], mTranslation[2]);
            glCallList(mID);
        }
        glPopMatrix();
/*
        if (!mFill)
            glPolygonMode(GL_FRONT, GL_FILL);
        else
            glShadeModel(GL_SMOOTH);
            */
    }
    glPopAttrib();
}

void TSphere::moveSphere(int inX, int inY, int inZ)
{
    mTranslation[0] = inX;
    mTranslation[1] = inY;
    mTranslation[2] = inZ;
}

void TSphere::drawTriangle(float* inA, float* inB, float* inC)
{
    glNormal3fv(inA);
    glVertex3fv(inA);
    glNormal3fv(inB);
    glVertex3fv(inB);
    glNormal3fv(inC);
    glVertex3fv(inC);
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
