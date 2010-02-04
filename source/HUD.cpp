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

#include "HUD.h"

HUD::HUD()
{
    mDisplay.x = 1;
    mDisplay.y = 1;
    mRange = 1.0f;
}

HUD::~HUD()
{
    // see HUD::deleteButtons()
}

void HUD::setDisplay(const Point2D<int>& inDisplay, float inRange)
{
    mDisplay = inDisplay;
    mRange = inRange;
    setupButtons();
}

void HUD::addButton(Button* inButton)
{
    if (inButton == NULL) return;

    inButton->findPixels(mDisplay, mRange);
    mButtons.push_back(inButton);
}

void HUD::deleteButtons()
{
    // The HUD is not necessarily in charge of managing the Button objects, but
    // this grants the option of having the HUD wipe out all of them.
    for (unsigned int i = 0; i < mButtons.size(); ++i)
    {
        delete mButtons[i];
        mButtons[i] = NULL;
    }
}

void HUD::setupButtons()
{
    for (unsigned int i = 0; i < mButtons.size(); ++i)
        mButtons[i]->findPixels(mDisplay, mRange);
}

void HUD::display()
{
    /// HUD display (orthographic projection)
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    double ratio = double(mDisplay.x) / double(mDisplay.y);
    glOrtho(-mRange * ratio, mRange * ratio, -mRange, mRange,
        -mRange, mRange);

    glMatrixMode(GL_MODELVIEW);

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);

    glColor3f(1.0f, 1.0f, 1.0f);
    for (unsigned int i = 0; i < mButtons.size(); ++i) mButtons[i]->display();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glPopAttrib();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

int HUD::setStates(int inX, int inY, bool inPress)
{
    int outHit = 0;
    int hover = inPress ? HUD_PRESS : HUD_HOVER;

    for (unsigned int i = 0; i < mButtons.size(); ++i)
    {
        int state = mButtons[i]->isOver(inX, inY) ? hover : HUD_OUT;
        mButtons[i]->setState(state);
        if (state != HUD_OUT) outHit = mButtons[i]->getID();
    }

    return outHit;
}
