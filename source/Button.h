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

#include <string>
using namespace std;

class Button
{
    public:
        Button(const char* inKeyword);
        virtual ~Button();

        void setLocation(float inX, float inY);
        void setSize(float inX, float inY);
        void set(const Point2D<float>& inLocation,
            const Point2D<float>& inSize);

    private:
        void assemble(string& inString, const char* inAdd);

        static string mPrefix;
        static string mSuffix;

        string mKeyword;
        GLuint mTextures[3];
        Point2D<float> mLocation;
        Point2D<float> mSize;
};

#endif
