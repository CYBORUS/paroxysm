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

#include "MapManagerModule.h"

bool MapManagerModule::onLoad()
{
    Point2D<int> mDisplay;
    Point2D<int> mCenter;

    mDisplay.x = SDL_GetVideoSurface()->w;
    mCenter.x = mDisplay.x / 2;
    mDisplay.y = SDL_GetVideoSurface()->h;
    mCenter.y = mDisplay.y / 2;

    mHUD.setDisplay(mDisplay);

    mTextBox = new TextBox;
    mTextBox->setSize(8.0f, 1.0f);

    mHUD.addWidget(mTextBox);

    return true;
}

void MapManagerModule::onInit()
{
    mRunning = true;
}

void MapManagerModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mHUD.display();
}

void MapManagerModule::onCleanup()
{
}

void MapManagerModule::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    switch(inSym)
    {
        case SDLK_ESCAPE:
        {
            mRunning = false;
            break;
        }

        default:
        {
            mTextBox->keyPress(inSym);
            break;
        }
    }
}
