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
    mSurface = NULL;
    mSize = 0;

    if (!TTF_WasInit() && TTF_Init() == -1)
    {
        cerr << "TTF_Init: " << TTF_GetError() << endl;
    }

    mColor.r = 255;
    mColor.g = 255;
    mColor.b = 255;
    mColor.unused = 255;
}

TextLayer::~TextLayer()
{
    if (mFont != NULL)
    {
        TTF_CloseFont(mFont);
        mFont = NULL;
    }

    SDL_FreeSurface(mSurface);
}

bool TextLayer::loadFont(const char* inFile, int inSize)
{
    mSize = inSize;
    mFont = TTF_OpenFont(inFile, inSize);

    if (mFont == NULL)
    {
        cerr << "TTF_OpenFont: " << TTF_GetError() << endl;
        exit(2);
    }

    return mFont != NULL;
}

void TextLayer::setColor(Uint8 inRed, Uint8 inGreen, Uint8 inBlue)
{
    mColor.r = inRed;
    mColor.g = inGreen;
    mColor.b = inBlue;
}

void TextLayer::setText(const char* inText)
{
    mText = inText;

    if (mSurface != NULL)
    {
        SDL_FreeSurface(mSurface);
    }
    //surface = TTF_RenderText_Solid(mFont, mText.c_str(), mColor);
    mSurface = TTF_RenderText_Blended(mFont, mText.c_str(), mColor);
}

void TextLayer::setText(const string& inString)
{
    mText = inString;

    if (mSurface != NULL)
    {
        SDL_FreeSurface(mSurface);
    }
    //surface = TTF_RenderText_Solid(mFont, mText.c_str(), mColor);
    mSurface = TTF_RenderText_Blended(mFont, mText.c_str(), mColor);
}

string TextLayer::getText()
{
   return mText;
}
