/**
 *  This file is part of Zero2D.
 *
 *  Zero2D is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Zero2D is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Zero2D.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _TEXTPIC_H_
#define _TEXTPIC_H_

#include "Vector3D.h"
#include "Point2D.h"

#include <SDL_ttf.h>
#include <SDL_opengl.h>

#include <string>
using namespace std;

enum TextQuality { TTF_SOLID, TTF_SHADED, TTF_BLENDED };

class TextPic
{
    public:
        TextPic();
        ~TextPic();

        void loadFont(const char* inFile, int inSize);
        const string& getText() const;
        void setText(const char* inText);
        void setText(const string& inText);
        void draw(float inX, float inY, float inScale);
        void setQuality(TextQuality inQuality);

    private:
        void render();

        TTF_Font* mFont;
        SDL_Color mColor;
        Vector3D<float> mColorMod;
        string mText;
        GLuint mTexture;
        Point2D<float> mScales;
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

#endif
