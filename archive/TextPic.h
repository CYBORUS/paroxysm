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

#ifndef _TEXTPIC_H_
#define _TEXTPIC_H_

#include "Vector3D.h"
#include "Point2D.h"

#include <SDL_ttf.h>
#include "OGL.h"

#include <string>
using namespace std;

enum TextQuality { TTF_SOLID, TTF_SHADED, TTF_BLENDED };

class TextPic
{
    public:
        TextPic();
        ~TextPic();

        bool loadFont(const char* inFile, int inSize);
        const string& getText() const;
        void setText(const char* inText);
        void setText(const string& inText);
        void draw(float inX, float inY, float inScale);
        void setQuality(TextQuality inQuality);
        void setColor(const Vector3D<float>& inVector);
        Point2D<int> getTextSize();

    private:
        void render();

        TTF_Font* mFont;
        SDL_Color mColor;
        Vector3D<float> mColorMod;
        string mText;
        GLuint mTexture;
        Point2D<float> mScales;
        Point2D<int> mTextSize;
        TextQuality mQuality;
};

inline const string& TextPic::getText() const
{
   return mText;
}

inline void TextPic::setQuality(TextQuality inQuality)
{
    mQuality = inQuality;
}

inline Point2D<int> TextPic::getTextSize()
{
    return mTextSize;
}

#endif
