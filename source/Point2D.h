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

#ifndef POINT2D_H
#define POINT2D_H

template<class T>
struct Point2D
{
    Point2D<T>();
    Point2D<T>(T inValue);
    Point2D<T>(T inX, T inY);
    T x;
    T y;
};

template<class T>
Point2D<T>::Point2D()
{
    x = 0;
    y = 0;
}

template<class T>
Point2D<T>::Point2D(T inValue)
{
    x = inValue;
    y = inValue;
}

template<class T>
Point2D<T>::Point2D(T inX, T inY)
{
    x = inX;
    y = inY;
}

#endif
