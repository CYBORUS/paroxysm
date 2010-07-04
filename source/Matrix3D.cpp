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
    Matrix3D transform;

    transform[5] = c;
    transform[6] = -s;
    transform[9] = s;
    transform[10] = c;

    (*this) *= transform;
}

void Matrix3D::rotateY(float inDegrees)
{
    float r = TO_RADIANS(inDegrees);
    float c = cos(r);
    float s = sin(r);
    Matrix3D transform;

    transform[0] = c;
    transform[2] = s;
    transform[8] = -s;
    transform[10] = c;

    (*this) *= transform;
}

void Matrix3D::rotateZ(float inDegrees)
{
    float r = TO_RADIANS(inDegrees);
    float c = cos(r);
    float s = sin(r);
    Matrix3D transform;

    transform[0] = c;
    transform[1] = -s;
    transform[4] = s;
    transform[5] = c;

    (*this) *= transform;
}

void Matrix3D::scale(float inScale)
{
    mData[0] = (mData[5] = (mData[10] = inScale));
}

void Matrix3D::scale(float inX, float inY, float inZ)
{
    Matrix3D transform;

    transform[0] = inX;
    transform[5] = inY;
    transform[10] = inZ;

    (*this) *= transform;
}

void Matrix3D::translate(float inX, float inY, float inZ)
{
    Matrix3D transform;

    transform[3] = inX;
    transform[7] = inY;
    transform[11] = inZ;

    (*this) *= transform;
}

void Matrix3D::frustrum(float inLeft, float inRight, float inBottom,
    float inTop, float inNear, float inFar)
{
    Matrix3D transform;

    transform[0] = (2 * inNear) / (inRight - inLeft);
    transform[2] = (inRight + inLeft) / (inRight - inLeft);
    transform[5] = (2 * inNear) / (inTop - inBottom);
    transform[6] = (inTop + inBottom) / (inTop - inBottom);
    transform[10] = (inFar + inNear) / (inNear - inFar);
    transform[11] = (2.0f * inFar * inNear) / (inNear - inFar);
    transform[14] = -1.0f;
    transform[15] = 0.0f;

    (*this) *= transform;
}

void Matrix3D::perspective(float inFieldOfView, float inRatio, float inNear,
    float inFar)
{
    /// adaptation of gluPerspective
    /// http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
    float r = TO_RADIANS(inFieldOfView);
    float f = 1.0f / tan(inFieldOfView / 2.0f);

    Matrix3D transform;

    transform[0] = f / inRatio;
    transform[5] = f;
    transform[10] = (inFar + inNear) / (inNear - inFar);
    transform[11] = (2 * inFar * inNear) / (inNear - inFar);
    transform[14] = -1.0f;
    transform[15] = 0.0f;

    (*this) *= transform;
}

void Matrix3D::orthographic(float inLeft, float inRight, float inBottom,
    float inTop, float inNear, float inFar)
{
    Matrix3D transform;

    transform[0] = 2.0f / (inRight - inLeft);
    transform[3] = (inRight + inLeft) / (inRight - inLeft);
    transform[5] = 2.0f / (inTop - inBottom);
    transform[7] = (inTop + inBottom) / (inTop - inBottom);
    transform[10] = 2.0f / (inNear - inFar);
    transform[11] = (inFar + inNear) / (inFar - inNear);

    (*this) *= transform;
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
