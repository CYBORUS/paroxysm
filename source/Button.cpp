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

    glGenTextures(4, mTextures);

    string file;

    assemble(file, "_out");
    DisplayEngine::loadTexture(file.c_str(), mTextures[OUTSIDE]);
    assemble(file, "_hover");
    DisplayEngine::loadTexture(file.c_str(), mTextures[HOVER]);
    assemble(file, "_press");
    DisplayEngine::loadTexture(file.c_str(), mTextures[PRESS]);
    assemble(file, "_disabled");
    DisplayEngine::loadTexture(file.c_str(), mTextures[3]);

}

Button::~Button()
{
    glDeleteTextures(4, mTextures);
}

void Button::display()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mTextures[mEnabled ? mMouseState : 3]);

    glBegin(GL_QUADS);
    {
        glTexCoord2i(0, 1);
        glVertex2f(mObjectUL.x, mObjectLR.y);
        glTexCoord2i(1, 1);
        glVertex2f(mObjectLR.x, mObjectLR.y);
        glTexCoord2i(1, 0);
        glVertex2f(mObjectLR.x, mObjectUL.y);
        glTexCoord2i(0, 0);
        glVertex2f(mObjectUL.x, mObjectUL.y);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void Button::assemble(string& inString, const char* inAdd)
{
    inString = mPrefix;
    inString += mKeyword;
    inString += inAdd;
    inString += mSuffix;
}

void Button::onStateChange()
{
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
