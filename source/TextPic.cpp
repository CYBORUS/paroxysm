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

#include "TextPic.h"
#include "DisplayEngine.h"

TextPic::TextPic() : mFont(NULL), mColorMod(1.0f), mTexture(0),
    mQuality(TTF_BLENDED)
{
    mColor.r = 255;
    mColor.g = 255;
    mColor.b = 255;
    mColor.unused = 255;

    glGenTextures(1, &mTexture);
}

TextPic::~TextPic()
{
    if (mFont != NULL)
    {
        TTF_CloseFont(mFont);
        mFont = NULL;
    }

    glDeleteTextures(1, &mTexture);
}

bool TextPic::loadFont(const char* inFile, int inSize)
{
    mFont = TTF_OpenFont(inFile, inSize);

    if (mFont == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void TextPic::setText(const char* inText)
{
    if (mText == inText) return;
    mText = inText;
    render();
}

void TextPic::setText(const string& inText)
{
    if (mText == inText) return;
    mText = inText;
    render();
}

void TextPic::render()
{
    Surface textSurface;

    switch (mQuality)
    {
        case TTF_BLENDED:
        {
            textSurface = TTF_RenderText_Blended(mFont, mText.c_str(), mColor);
            break;
        }

        case TTF_SHADED:
        {
            //textSurface = TTF_RenderText_Shaded(mFont, mText.c_str(), mColor);
            break;
        }

        case TTF_SOLID:
        default:
        {
            textSurface = TTF_RenderText_Solid(mFont, mText.c_str(), mColor);
        }
    }

    if (textSurface == NULL)
    {
        DisplayEngine::loadTexture((Surface)NULL, mTexture);
        return;
    }

    mTextSize.x = textSurface->w;
    mTextSize.y = textSurface->h;

    int widthPower = int(log(mTextSize.x) / log(2.0f)) + 1;
    int heightPower = int(log(mTextSize.y) / log(2.0f)) + 1;

    widthPower = (int)pow(2.0f, widthPower);
    heightPower = (int)pow(2.0f, heightPower);

    Surface s = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, widthPower,
        heightPower, textSurface->format->BitsPerPixel,
        textSurface->format->Rmask, textSurface->format->Gmask,
        textSurface->format->Bmask, textSurface->format->Amask);

    SDL_SetAlpha(textSurface, 0, 0xff);
    SDL_SetAlpha(s, SDL_SRCALPHA, 0x00);

    if (SDL_BlitSurface(textSurface, NULL, s, NULL) != 0)
    {
        cerr << "blitting error" << endl;
    }

    SDL_SetAlpha(s, SDL_SRCALPHA, textSurface->format->alpha);

    SDL_FreeSurface(textSurface);

    float ratio = float(mTextSize.x) / float(mTextSize.y);
    Point2D<float> ratios;
    ratios.x = float(mTextSize.x) / (s == NULL ? 1.0f : s->w);
    ratios.y = float(mTextSize.y) / (s == NULL ? 1.0f : s->h);
    mScales.x = ratio / ratios.x;
    mScales.y = 1.0f / ratios.y;

    DisplayEngine::loadTexture(s, mTexture, true);
}

void TextPic::draw(float inX, float inY, float inScale)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glBegin(GL_QUADS);
    {
        glColor4fv(mColorMod.array());
        glTexCoord2i(0, 1);
        glVertex2f(inX, inY - (inScale * mScales.y));
        glTexCoord2i(1, 1);
        glVertex2f(inX + (inScale * mScales.x), inY - (inScale * mScales.y));
        glTexCoord2i(1, 0);
        glVertex2f(inX + (inScale * mScales.x), inY);
        glTexCoord2i(0, 0);
        glVertex2f(inX, inY);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
}

void TextPic::setColor(const Vector3D<float>& inVector)
{
    mColorMod = inVector;
}
