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

#ifndef BUTTON_H
#define BUTTON_H

#include <SDL_opengl.h>
#include "Point2D.h"
#include "Widget.h"

#include <string>
using namespace std;

class Button
{
    public:
        Button(const char* inKeyword, int inID);
        virtual ~Button();

        void display();
        void setState(int inState);
        void setLocation(float inX, float inY);
        void setSize(float inX, float inY);
        void set(const Point2D<float>& inLocation,
            const Point2D<float>& inSize);
        void findPixels(const Point2D<int>& inDisplay, float inRange);
        bool isOver(int inX, int inY);
        int getID();

    private:
        void assemble(string& inString, const char* inAdd);

        static string mPrefix;
        static string mSuffix;

        string mKeyword;
        GLuint mTextures[3];
        int mState;
        int mID;

        Point2D<float> mLocation;
        Point2D<float> mSize;
        Point2D<float> mPixelUL;
        Point2D<float> mPixelLR;
};

inline bool Button::isOver(int inX, int inY)
{
    return inX >= mPixelUL.x && inX <= mPixelLR.x && inY >= mPixelUL.y
        && inY <= mPixelLR.y;
}

inline int Button::getID()
{
    return mID;
}

#endif
