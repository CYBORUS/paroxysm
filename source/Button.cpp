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

#include "Button.h"
#include "DisplayEngine.h"

string Button::mPrefix("assets/images/hud/");
string Button::mSuffix(".png");

Button::Button(const char* inKeyword, int inID) : mKeyword(inKeyword),
    mState(0), mID(inID)
{
    glGenTextures(3, mTextures);

    string file;

    assemble(file, "_out");
    DisplayEngine::loadTexture(file.c_str(), mTextures[0]);
    assemble(file, "_hover");
    DisplayEngine::loadTexture(file.c_str(), mTextures[1]);
    assemble(file, "_press");
    DisplayEngine::loadTexture(file.c_str(), mTextures[2]);
}

Button::~Button()
{
    glDeleteTextures(3, mTextures);
}

void Button::display()
{
    glBindTexture(GL_TEXTURE_2D, mTextures[mState]);
    glBegin(GL_QUADS);
    {
        glTexCoord2i(0, 1);
        glVertex2f(mLocation.x, mLocation.y - mSize.y);
        glTexCoord2i(1, 1);
        glVertex2f(mLocation.x + mSize.x, mLocation.y - mSize.y);
        glTexCoord2i(1, 0);
        glVertex2f(mLocation.x + mSize.x, mLocation.y);
        glTexCoord2i(0, 0);
        glVertex2f(mLocation.x, mLocation.y);
    }
    glEnd();
}

void Button::setState(int inState)
{
    mState = inState;
}

void Button::setLocation(float inX, float inY)
{
    mLocation.x = inX;
    mLocation.y = inY;
}

void Button::setSize(float inX, float inY)
{
    mSize.x = inX;
    mSize.y = inY;
}

void Button::set(const Point2D<float>& inLocation, const Point2D<float>& inSize)
{
    mLocation = inLocation;
    mSize = inSize;
}

void Button::findPixels(const Point2D<int>& inDisplay, float inRange)
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

void Button::assemble(string& inString, const char* inAdd)
{
    inString = mPrefix;
    inString += mKeyword;
    inString += inAdd;
    inString += mSuffix;
}

