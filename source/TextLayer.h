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