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

#include "Widget.h"

Widget::Widget()
{
}

Widget::~Widget()
{
}

void Widget::setState(MouseState inState)
{
    mMouseState = inState;
}

void Widget::setLocation(float inX, float inY)
{
    mLocation.x = inX;
    mLocation.y = inY;
}

void Widget::setSize(float inX, float inY)
{
    mSize.x = inX;
    mSize.y = inY;
}

void Widget::set(const Point2D<float>& inLocation, const Point2D<float>& inSize)
{
    mLocation = inLocation;
    mSize = inSize;
}

void Widget::findPixels(const Point2D<int>& inDisplay, float inRange)
{
    Point2D<int> center;
    center.x = inDisplay.x / 2;
    center.y = inDisplay.y / 2;

    float ratio = float(center.y) / inRange;

    mPixelUL.x = center.x + int(mLocation.x * ratio);
    mPixelUL.y = center.y - int(mLocation.y * ratio);
    mPixelLR.x = mPixelUL.x + int(mSize.x * ratio);
    mPixelLR.y = mPixelUL.y + int(mSize.y * ratio);
}
