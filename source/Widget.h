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

#ifndef WIDGET_H
#define WIDGET_H

#include "Point2D.h"

enum MouseState { OUTSIDE, HOVER, PRESS };

class Widget
{
    public:
        Widget();
        virtual ~Widget();

        virtual void display() = 0;

        void setState(MouseState inState);
        void setLocation(float inX, float inY);
        void setSize(float inX, float inY);
        void set(const Point2D<float>& inLocation,
            const Point2D<float>& inSize);
        void findPixels(const Point2D<int>& inDisplay, float inRange);
        bool isOver(int inX, int inY);
        int getID();

    protected:
        int mID;
        MouseState mMouseState;

    private:
        Point2D<float> mLocation;
        Point2D<float> mSize;
        Point2D<int> mPixelUL;
        Point2D<int> mPixelLR;
};

inline bool Widget::isOver(int inX, int inY)
{
    return inX >= mPixelUL.x && inX <= mPixelLR.x && inY >= mPixelUL.y
        && inY <= mPixelLR.y;
}

inline int Widget::getID()
{
    return mID;
}

#endif
