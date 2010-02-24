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

    Surface textSurface = NULL;

    //textSurface = TTF_RenderText_Solid(mFont, mText.c_str(), mColor);
    textSurface = TTF_RenderText_Blended(mFont, mText.c_str(), mColor);

    if (textSurface == NULL)
    {
        mSurface = NULL;
        return;
    }

    //textSurface = SDL_DisplayFormatAlpha(textSurface);

    mTextSize.x = textSurface->w;
    mTextSize.y = textSurface->h;

    SDL_Rect src;
    SDL_Rect dest;

    src.x = 0;
    src.y = 0;
    src.h = textSurface->h;
    src.w = textSurface->w;


    dest.x = 0;
    dest.y = 0;
    dest.h = textSurface->h;
    dest.w = textSurface->w;

    int widthPower = int(log(textSurface->w) / log(2.0f)) + 1;
    int heightPower = int(log(textSurface->h) / log(2.0f)) + 1;

    widthPower = (int)pow(2.0f, widthPower);
    heightPower = (int)pow(2.0f, heightPower);


    cerr << "text dimensions: " << textSurface->w << ", " << textSurface->h << endl;
    cerr << "widthPower: " << widthPower << " heightPower: " << heightPower << endl;

    mSurface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, widthPower,
                    heightPower, textSurface->format->BitsPerPixel, textSurface->format->Rmask,
                    textSurface->format->Gmask, textSurface->format->Bmask, textSurface->format->Amask);

    SDL_SetAlpha(textSurface, 0, 0xff);
    SDL_SetAlpha(mSurface, SDL_SRCALPHA, 0x00);

    if (SDL_BlitSurface(textSurface, NULL, mSurface, NULL) != 0)
    {
        cerr << "blitting error" << endl;
    }

    SDL_SetAlpha(textSurface, SDL_SRCALPHA, textSurface->format->alpha);
    SDL_SetAlpha(mSurface, SDL_SRCALPHA, textSurface->format->alpha);

    SDL_SaveBMP(textSurface, "textSurface.bmp");
    SDL_SaveBMP(mSurface, "mSurface.bmp");
}

void TextLayer::setText(const string& inString)
{
    setText(inString.c_str());
/*
    mText = inString;

    if (mSurface != NULL)
    {
        SDL_FreeSurface(mSurface);
    }
    //surface = TTF_RenderText_Solid(mFont, mText.c_str(), mColor);
    mSurface = TTF_RenderText_Blended(mFont, mText.c_str(), mColor);
    */
}

string TextLayer::getText()
{
   return mText;
}


Point2D<int> TextLayer::getTextSize()
{
    return mTextSize;
}
