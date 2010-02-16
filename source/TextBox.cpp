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

#include "TextBox.h"

TextBox::TextBox()
{
    glGenTextures(1, &mTextTexture);
    mText = "test";
    mTextLayer.loadFont("assets/misc/DejaVuSans.ttf", 16);
    mTextLayer.setColor(0, 255, 128, 255);
    update();
}

TextBox::~TextBox()
{
    glDeleteTextures(1, &mTextTexture);
}

void TextBox::display()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    {
        glVertex2f(mObjectUL.x, mObjectLR.y);
        glVertex2f(mObjectLR.x, mObjectLR.y);
        glVertex2f(mObjectLR.x, mObjectUL.y);
        glVertex2f(mObjectUL.x, mObjectUL.y);
    }
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_SCISSOR_TEST);
    glScissor(mPixelUL.x, mDisplay.y - mPixelLR.y, mPixelLR.x - mPixelUL.x,
        mPixelLR.y - mPixelUL.y);
    glBindTexture(GL_TEXTURE_2D, mTextTexture);
    glBegin(GL_QUADS);
    {
        glTexCoord2i(0, 1);
        glVertex2f(mObjectUL.x, mObjectLR.y);
        glTexCoord2i(1, 1);
        glVertex2f(mObjectUL.x + mSize.y * mRatio, mObjectLR.y);
        glTexCoord2i(1, 0);
        glVertex2f(mObjectUL.x + mSize.y * mRatio, mObjectUL.y);
        glTexCoord2i(0, 0);
        glVertex2f(mObjectUL.x, mObjectUL.y);
    }
    glEnd();
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_TEXTURE_2D);
}

void TextBox::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    switch (inSym)
    {
        case SDLK_BACKSPACE:
        {
            int l = mText.length();
            if (l > 0)
            {
                mText = mText.substr(0, l - 1);
            }

            break;
        }

        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
        case SDLK_LCTRL:
        case SDLK_RCTRL:
        case SDLK_LALT:
        case SDLK_RALT:
        case SDLK_ESCAPE:
        {
            break;
        }

        default:
        {
            int nextChar = inSym;
            if (inMod & (KMOD_LSHIFT | KMOD_RSHIFT)) nextChar -= 32;
            mText += nextChar;
            break;
        }
    }

    update();
}

void TextBox::update()
{
    mTextLayer.setText(mText);
    Surface s = mTextLayer.getTextImage();
    mRatio = s == NULL ? 1.0f : float(s->w) / float(s->h);
    DisplayEngine::loadTexture(s, mTextTexture);
}

bool TextBox::canFocus()
{
    return true;
}
