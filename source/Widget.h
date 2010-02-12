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
#include "SoundEngine.h"

enum MouseState { OUTSIDE = 0, HOVER = 1, PRESS = 2 };

class Widget
{
    public:
        Widget();
        virtual ~Widget();

        virtual void display() = 0;

        void setState(MouseState inState);
        void setLocation(float inX, float inY);
        void setSize(float inX, float inY);
        virtual void onMouseChange(int inX, int inY);
        void set(const Point2D<float>& inLocation,
            const Point2D<float>& inSize);
        virtual void findPixels(const Point2D<int>& inDisplay, float inRange);
        virtual void findObject();
        bool isOver(int inX, int inY);
        bool isVisible();
        virtual void setVisible(bool inVisible);
        int getID();

        void enable();
        void enable(bool inEnable);
        void disable();
        void toggle();

    protected:
        bool mEnabled;
        bool mVisible;
        int mID;
        MouseState mMouseState;

        Point2D<float> mLocation;
        Point2D<float> mSize;
        Point2D<float> mObjectUL;
        Point2D<float> mObjectLR;
        Point2D<int> mPixelUL;
        Point2D<int> mPixelLR;
};

inline bool Widget::isOver(int inX, int inY)
{
    return mVisible && inX >= mPixelUL.x && inX <= mPixelLR.x
        && inY >= mPixelUL.y && inY <= mPixelLR.y;
}

inline bool Widget::isVisible()
{
    return mVisible;
}

inline int Widget::getID()
{
    return mID;
}

#endif
