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

#include "TextLayer.h"

TextLayer::TextLayer()
{
    mFont = NULL;
    mSize = 0;
}

TextLayer::~TextLayer()
{
    if (mFont != NULL)
    {
        TTF_CloseFont(mFont);
        mFont = NULL;
    }
}

bool TextLayer::loadFont(const char* inFile, int inSize)
{
    mSize = inSize;
    mFont = TTF_OpenFont(inFile, inSize);
    return mFont != NULL;
}

void TextLayer::setColor(char inRed, char inGreen, char inBlue, char inAlpha)
{
    mColor.r = inRed;
    mColor.g = inGreen;
    mColor.b = inBlue;
    mColor.unused = inAlpha;
}

void TextLayer::setText(const char* inText)
{
    mText = inText;
    SDL_FreeSurface(mSurface);
    //surface = TTF_RenderText_Solid(mFont, mText.c_str(), mColor);
    mSurface = TTF_RenderText_Blended(mFont, mText.c_str(), mColor);
}

string TextLayer::getText()
{
   return mText;
}

SDL_Surface* TextLayer::getTextImage()
{
   return mSurface;
}