#ifndef _TEXTLAYER_H
#define	_TEXTLAYER_H

#include "DisplayEngine.h"

#include <SDL_ttf.h>

#include <string>
#include <iostream>
using namespace std;

class TextLayer
{
    public:
        TextLayer();
        virtual ~TextLayer();

        bool loadFont(const char* inFile, int inSize);
        void setColor(char inRed, char inGreen, char inBlue, char inAlpha);
        void setText(const char* inText);
        string getText();
        SDL_Surface* getTextImage();

    protected:
        SDL_Color mColor;
        TTF_Font* mFont;
        int mSize;
        std::string mText;

        Surface mSurface;
};

#endif
