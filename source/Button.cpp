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

Button::Button(const char* inKeyword) : mKeyword(inKeyword)
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

void Button::assemble(string& inString, const char* inAdd)
{
    inString = mPrefix;
    inString += mKeyword;
    inString += inAdd;
    inString += mSuffix;
}
