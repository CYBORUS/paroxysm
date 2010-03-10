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

#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_

#include <cmath>
#include "Matrix.h"

#define PI 3.141592654
#define PI_HALVES PI / 2.0
#define TO_RADIANS(n) ((n) * PI / 180.0)
#define TO_DEGREES(n) ((n) * 180.0 / PI)
#define PSEUDO_ZERO 0.000001

template<class T>
class Vector3D
{
    public:
        Vector3D<T>();
        Vector3D<T>(const Vector3D<T>& inVector);
        explicit Vector3D<T>(T inValue);
        explicit Vector3D<T>(T inX, T inY, T inZ);
        Matrix<T> toMatrix() const;
        void set(T inValue);
        void set(T inX, T inY, T inZ);
        T get(int inIndex) const;
        void normalize();
        const Vector3D<T> normalized() const;
        bool isZero() const;
        void negate();
        inline float length() const;
        const Vector3D<T> negated() const;
        void processMatrix(const Matrix<T>& inMatrix);

        Vector3D<T>& operator=(const Vector3D<T>& inVector);
        Vector3D<T>& operator+=(const Vector3D<T>& inVector);
        Vector3D<T>& operator-=(const Vector3D<T>& inVector);
        const Vector3D<T> operator+(const Vector3D<T>& inVector) const;
        const Vector3D<T> operator-(const Vector3D<T>& inVector) const;
        const Vector3D<T> operator-() const;
        T operator*(const Vector3D<T>& inVector) const;
        const Vector3D<T> operator*(const T inRight) const;
        const Vector3D<T> operator^(const Vector3D<T>& inVector) const;
        T& operator[](int inIndex);
        T operator[](int inIndex) const;
        T* array();

    private:
        T mVector[4];
};

template<class T>
Vector3D<T>::Vector3D()
{
    mVector[0] = 0;
    mVector[1] = 0;
    mVector[2] = 0;
    mVector[3] = 1;
}

template<class T>
Vector3D<T>::Vector3D(const Vector3D<T>& inVector)
{
    mVector[0] = inVector.mVector[0];
    mVector[1] = inVector.mVector[1];
    mVector[2] = inVector.mVector[2];
    mVector[3] = inVector.mVector[3];
}

template<class T>
Vector3D<T>::Vector3D(T inValue)
{
    mVector[0] = inValue;
    mVector[1] = inValue;
    mVector[2] = inValue;
    mVector[3] = 1;
}

template<class T>
Vector3D<T>::Vector3D(T inX, T inY, T inZ)
{
    mVector[0] = inX;
    mVector[1] = inY;
    mVector[2] = inZ;
    mVector[3] = 1;

}

template<class T>
Matrix<T> Vector3D<T>::toMatrix() const
{
    Matrix<T> outMatrix(4, 1);
    outMatrix[0] = mVector[0];
    outMatrix[1] = mVector[1];
    outMatrix[2] = mVector[2];
    outMatrix[3] = mVector[3];
    return outMatrix;
}

template<class T>
void Vector3D<T>::set(T inValue)
{
    mVector[0] = inValue;
    mVector[1] = inValue;
    mVector[2] = inValue;
}

template<class T>
void Vector3D<T>::set(T inX, T inY, T inZ)
{
    mVector[0] = inX;
    mVector[1] = inY;
    mVector[2] = inZ;
}

template<class T>
void Vector3D<T>::normalize()
{
    T length = sqrt((mVector[0] * mVector[0]) + (mVector[1] * mVector[1]) +
        (mVector[2] * mVector[2]));
    if (fabs(length) < PSEUDO_ZERO) return;
    mVector[0] /= length;
    mVector[1] /= length;
    mVector[2] /= length;
}

template<class T>
const Vector3D<T> Vector3D<T>::normalized() const
{
    Vector3D<T> outVector(*this);
    outVector.normalize();
    return outVector;
}

template<class T>
bool Vector3D<T>::isZero() const
{
    return fabs(mVector[0]) < PSEUDO_ZERO
        && fabs(mVector[1]) < PSEUDO_ZERO
        && fabs(mVector[2]) < PSEUDO_ZERO;
}

template<class T>
void Vector3D<T>::negate()
{
    mVector[0] = -mVector[0];
    mVector[1] = -mVector[1];
    mVector[2] = -mVector[2];
}

template<class T>
inline float Vector3D<T>::length() const
{
    return sqrt((mVector[0] * mVector[0]) + (mVector[1] * mVector[1]) +
                (mVector[2] * mVector[2]));
}

template<class T>
Vector3D<T>& Vector3D<T>::operator=(const Vector3D<T>& inVector)
{
    mVector[0] = inVector.mVector[0];
    mVector[1] = inVector.mVector[1];
    mVector[2] = inVector.mVector[2];
    mVector[3] = inVector.mVector[3];
    return *this;
}

template<class T>
Vector3D<T>& Vector3D<T>::operator+=(const Vector3D<T>& inVector)
{
    mVector[0] += inVector.mVector[0];
    mVector[1] += inVector.mVector[1];
    mVector[2] += inVector.mVector[2];
    return *this;
}

template<class T>
Vector3D<T>& Vector3D<T>::operator-=(const Vector3D<T>& inVector)
{
    mVector[0] -= inVector.mVector[0];
    mVector[1] -= inVector.mVector[1];
    mVector[2] -= inVector.mVector[2];
    return *this;
}

template<class T>
const Vector3D<T> Vector3D<T>::operator+(const Vector3D<T>& inVector) const
{
    Vector3D<T> outVector;
    outVector.mVector[0] = mVector[0] + inVector.mVector[0];
    outVector.mVector[1] = mVector[1] + inVector.mVector[1];
    outVector.mVector[2] = mVector[2] + inVector.mVector[2];
    return outVector;
}

template<class T>
const Vector3D<T> Vector3D<T>::operator-(const Vector3D<T>& inVector) const
{
    Vector3D<T> outVector;
    outVector.mVector[0] = mVector[0] - inVector.mVector[0];
    outVector.mVector[1] = mVector[1] - inVector.mVector[1];
    outVector.mVector[2] = mVector[2] - inVector.mVector[2];
    return outVector;
}

template<class T>
const Vector3D<T> Vector3D<T>::operator-() const
{
    Vector3D<T> outVector(*this);
    outVector.negate();
    return outVector;
}

template<class T>
T Vector3D<T>::operator*(const Vector3D<T>& inVector) const
{
    return (mVector[0] * inVector.mVector[0])
        + (mVector[1] * inVector.mVector[1])
        + (mVector[2] * inVector.mVector[2]);
}

template<class T>
const Vector3D<T> Vector3D<T>::operator*(const T inRight) const
{
    Vector3D<T> outVector;
    outVector.mVector[0] = mVector[0] * inRight;
    outVector.mVector[1] = mVector[1] * inRight;
    outVector.mVector[2] = mVector[2] * inRight;
    return outVector;
}

template<class T>
const Vector3D<T> Vector3D<T>::operator^(const Vector3D<T>& inVector) const
{
    Vector3D<T> outVector;
    outVector[0] = (mVector[1] * inVector.mVector[2]) -
        (mVector[2] * inVector.mVector[1]);
    outVector[1] = (mVector[2] * inVector.mVector[0]) -
        (mVector[0] * inVector.mVector[2]);
    outVector[2] = (mVector[0] * inVector.mVector[1]) -
        (mVector[1] * inVector.mVector[0]);
    return outVector;
}

template<class T>
T& Vector3D<T>::operator[](int inIndex)
{
    return mVector[inIndex];
}

template<class T>
T Vector3D<T>::operator[](int inIndex) const
{
    return mVector[inIndex];
}

template<class T>
T Vector3D<T>::get(int inIndex) const
{
    if (inIndex < 0 || inIndex > 2) return 0.0;
    return mVector[inIndex];
}

template<class T>
inline T* Vector3D<T>::array()
{
    return mVector;
}

template<class T>
void Vector3D<T>::processMatrix(const Matrix<T>& inMatrix)
{
    Vector3D<T> result;
    result[0] = mVector[0] * inMatrix[0] + mVector[1] * inMatrix[1]
        + mVector[2] * inMatrix[2] + mVector[3] * inMatrix[3];
    result[1] = mVector[0] * inMatrix[4] + mVector[1] * inMatrix[5]
        + mVector[2] * inMatrix[6] + mVector[3] * inMatrix[7];
    result[2] = mVector[0] * inMatrix[8] + mVector[1] * inMatrix[9]
        + mVector[2] * inMatrix[10] + mVector[3] * inMatrix[11];
    result[3] = mVector[0] * inMatrix[12] + mVector[1] * inMatrix[13]
        + mVector[2] * inMatrix[14] + mVector[3] * inMatrix[15];

    mVector[0] = result[0];
    mVector[1] = result[1];
    mVector[2] = result[2];
    mVector[3] = result[3];
}

template<class T>
istream& operator>>(istream& inStream, Vector3D<T>& inVector)
{
    char comma; // used strictly to help ignore delimiter commas
    T x;
    T y;
    T z;
    inStream >> x >> comma >> y >> comma >> z;
    inVector[0] = x;
    inVector[1] = y;
    inVector[2] = z;
    return inStream;
}

template<class T>
ostream& operator<<(ostream& inStream, const Vector3D<T>& inVector)
{
    inStream << '(' << inVector[0] << ", " << inVector[1] << ", "
        << inVector[2] << ')';
    return inStream;
}

#endif
