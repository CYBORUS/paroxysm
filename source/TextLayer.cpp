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
