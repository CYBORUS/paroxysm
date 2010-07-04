#include "Matrix3D.h"

#include <iomanip>
using namespace std;

Matrix3D::Matrix3D()
{
    // default to identity matrix
    loadIdentity();
}

Matrix3D::Matrix3D(const Matrix3D& inMatrix)
{
    copy(inMatrix);
}

void Matrix3D::loadIdentity()
{
    memset(mData, 0, 16 * sizeof(float));
    for (size_t i = 0; i < 16; i += 5) mData[i] = 1;
}

void Matrix3D::rotate(float inDegrees, float inX, float inY, float inZ)
{
    /// arbitrary rotation about an axis
    /// http://www.opengl.org/sdk/docs/man/xhtml/glRotate.xml

    // yet to be implemented; see the URL above
}

void Matrix3D::rotateX(float inDegrees)
{
    float r = TO_RADIANS(inDegrees);
    float c = cos(r);
    float s = sin(r);
    mData[5] = c;
    mData[6] = -s;
    mData[9] = s;
    mData[10] = c;
}

void Matrix3D::rotateY(float inDegrees)
{
    float r = TO_RADIANS(inDegrees);
    float c = cos(r);
    float s = sin(r);
    mData[0] = c;
    mData[2] = s;
    mData[8] = -s;
    mData[10] = c;
}

void Matrix3D::rotateZ(float inDegrees)
{
    float r = TO_RADIANS(inDegrees);
    float c = cos(r);
    float s = sin(r);
    mData[0] = c;
    mData[1] = -s;
    mData[4] = s;
    mData[5] = c;
}

void Matrix3D::scale(float inScale)
{
    mData[0] = (mData[5] = (mData[10] = inScale));
}

void Matrix3D::scale(float inX, float inY, float inZ)
{
    mData[0] = inX;
    mData[5] = inY;
    mData[10] = inZ;
}

void Matrix3D::translate(float inX, float inY, float inZ)
{
    mData[3] = inX;
    mData[7] = inY;
    mData[11] = inZ;
}

void Matrix3D::frustrum(float inLeft, float inRight, float inBottom,
    float inTop, float inNear, float inFar)
{
    mData[0] = (2 * inNear) / (inRight - inLeft);
    mData[2] = (inRight + inLeft) / (inRight - inLeft);
    mData[5] = (2 * inNear) / (inTop - inBottom);
    mData[6] = (inTop + inBottom) / (inTop - inBottom);
    mData[10] = (inFar + inNear) / (inNear - inFar);
    mData[11] = (2.0f * inFar * inNear) / (inNear - inFar);
    mData[14] = -1.0f;
    mData[15] = 0.0f;
}

void Matrix3D::perspective(float inFieldOfView, float inRatio, float inNear,
    float inFar)
{
    /// adaptation of gluPerspective
    /// http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
    float r = TO_RADIANS(inFieldOfView);
    float f = 1.0f / tan(inFieldOfView / 2.0f);
    mData[0] = f / inRatio;
    mData[5] = f;
    mData[10] = (inFar + inNear) / (inNear - inFar);
    mData[11] = (2 * inFar * inNear) / (inNear - inFar);
    mData[14] = -1.0f;
    mData[15] = 0.0f;

}

void Matrix3D::orthographic(float inLeft, float inRight, float inBottom,
    float inTop, float inNear, float inFar)
{
    mData[0] = 2.0f / (inRight - inLeft);
    mData[3] = (inRight + inLeft) / (inRight - inLeft);
    mData[5] = 2.0f / (inTop - inBottom);
    mData[7] = (inTop + inBottom) / (inTop - inBottom);
    mData[10] = 2.0f / (inNear - inFar);
    mData[11] = (inFar + inNear) / (inFar - inNear);
}

void Matrix3D::orthographic(float inRange, float inRatio)
{
    if (inRatio < 1.0f)
    {
        orthographic(-inRange, inRange, -inRange / inRatio, inRange / inRatio,
            -1.0f, 1.0f);
    }
    else
    {
        orthographic(-inRange * inRatio, inRange * inRatio, -inRange, inRange,
            -1.0f, 1.0f);
    }
}

Matrix3D& Matrix3D::operator=(const Matrix3D& inMatrix)
{
    if (this != &inMatrix) copy(inMatrix);
    return *this;
}

Matrix3D& Matrix3D::operator*=(const Matrix3D& inMatrix)
{
    Matrix3D result;
    for (size_t i = 0; i < 4; ++i)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            float value = 0.0f;
            for (size_t k = 0; k < 4; ++k)
                value += (*this)(i, k) * inMatrix(k, j);

            result(i, j) = value;
        }
    }
    copy(result);
    return *this;
}

ostream& operator<<(ostream& inStream, const Matrix3D& inMatrix)
{
    inStream << setprecision(2);
    for (size_t i = 0; i < 16; ++i)
    {
        if (i > 0 && i % 4 == 0) inStream << '\n';
        inStream << setw(6) << inMatrix[i];
    }

    return inStream;
}
