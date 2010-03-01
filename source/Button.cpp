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

#include "Button.h"
#include "DisplayEngine.h"

string Button::mPrefix("assets/images/hud/");
string Button::mSuffix(".png");
SoundEffect Button::mHoverSound = NULL;
SoundEffect Button::mClickSound = NULL;

Button::Button(const char* inKeyword, int inID) : mKeyword(inKeyword)
{
    if (mHoverSound == NULL)
    {
        mHoverSound = SoundEngine::loadSound("button_hover_1.ogg");
        mClickSound = SoundEngine::loadSound("button_click_1.ogg");
    }

    mLastState = OUTSIDE;
    mID = inID;

    glGenTextures(1, &mTexture);

    string file;

    assemble(file);
    DisplayEngine::loadTexture(file.c_str(), mTexture);
    onStateChange();
}

Button::~Button()
{
    glDeleteTextures(1, &mTexture);
}

void Button::display()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0.0f, mCoor.y);
        glVertex2f(mObjectUL.x, mObjectLR.y);
        glTexCoord2f(1.0f, mCoor.y);
        glVertex2f(mObjectLR.x, mObjectLR.y);
        glTexCoord2f(1.0f, mCoor.x);
        glVertex2f(mObjectLR.x, mObjectUL.y);
        glTexCoord2f(0.0f, mCoor.x);
        glVertex2f(mObjectUL.x, mObjectUL.y);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void Button::assemble(string& inString)
{
    inString = mPrefix;
    inString += mKeyword;
    inString += mSuffix;
}

void Button::onStateChange()
{
    switch (mMouseState)
    {
        case OUTSIDE:
        {
            mCoor.x = 0.0f;
            break;
        }

        case HOVER:
        {
            mCoor.x = 0.25f;
            break;
        }

        case PRESS:
        {
            mCoor.x = 0.5f;
            break;
        }

        default:
        {
        }
    }

    if (!mEnabled) mCoor.x = 0.75f;
    mCoor.y = mCoor.x + 0.25f;

    if (mEnabled && mVisible && mLastState != mMouseState)
    {
        switch (mMouseState)
        {
            case HOVER:
            {
                if (mLastState != PRESS)
                    SoundEngine::playSound(mHoverSound);
                break;
            }

            case PRESS:
            {
                SoundEngine::playSound(mClickSound);
                break;
            }

            default:
            {
            }
        }

        mLastState = mMouseState;
    }
}


void Button::preProcessing(float inRange)
{

}
