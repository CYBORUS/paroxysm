#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_

#include <cmath>
#include "Matrix.h"

#define PI 3.141592654
#define TO_RADIANS(n) ((n) * PI / 180.0)
#define PSEUDO_ZERO 0.000001

template<class T>
class Vector3D
{
    public:
        Vector3D<T>();
        Vector3D<T>(const Vector3D<T>& inVector);
        Matrix<T> toMatrix() const;
        void set(T inValue);
        void set(T inX, T inY, T inZ);
        T get(int inIndex) const;
        void normalize();
        const Vector3D<T> normalized() const;
        bool isZero() const;
        void negate();
        const Vector3D<T> negated() const;
        void processMatrix(const Matrix<T> inMatrix);

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
T* Vector3D<T>::array()
{
    return mVector;
}

template<class T>
void Vector3D<T>::processMatrix(const Matrix<T> inMatrix)
{
    Matrix<T> result(inMatrix * toMatrix());
    for (int i = 0; i < 4; ++i) mVector[i] = result[i];
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
