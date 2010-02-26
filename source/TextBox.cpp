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

TextBox::TextBox(int inID)
{
    mID = inID;
    mHideOnEnter = false;
    mText = "test";
    mTextPic.loadFont("assets/misc/DejaVuSans.ttf", 24);
    mLockedIn = false;
    update();
}

TextBox::~TextBox()
{
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

    glEnable(GL_SCISSOR_TEST);
    glScissor(mPixelUL.x, mDisplay.y - mPixelLR.y, mPixelLR.x - mPixelUL.x,
        mPixelLR.y - mPixelUL.y);
    mTextPic.draw(mObjectUL.x, mObjectUL.y, mSize.y);
    glDisable(GL_SCISSOR_TEST);
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

        case SDLK_RETURN:
        {
            mLockedIn = true;
            // 'break' omitted intentionally
        }
        case SDLK_ESCAPE:
        {
            if (mHideOnEnter) mVisible = false;
            break;
        }

        default:
        {
            mLockedIn = false;
            int nextChar = DisplayEngine::processKey(inSym, inMod);
            if (nextChar > 0) mText += nextChar;
            break;
        }
    }

    update();
}

void TextBox::update()
{
    mTextPic.setText(mText);
}

bool TextBox::canFocus()
{
    return true;
}

void TextBox::setTextColor(Uint8 inRed, Uint8 inGreen, Uint8 inBlue)
{
    //mTextLayer.setColor(inRed, inGreen, inBlue);
}

void TextBox::hideOnEnter(bool inChange)
{
    mHideOnEnter = inChange;
}

void TextBox::setText(const char* inString)
{
    mText = inString;
    update();
}
