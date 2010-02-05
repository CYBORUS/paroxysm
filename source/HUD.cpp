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
    for (unsigned int i = 0; i < mWidgets.size(); ++i)
    {
        delete mWidgets[i];
        mWidgets[i] = NULL;
    }
}

void HUD::setDisplay(const Point2D<int>& inDisplay, float inRange)
{
    mDisplay = inDisplay;
    mRange = inRange;
    findPixels();
}

void HUD::addWidget(Widget* inWidget)
{
    if (inWidget == NULL) return;

    inWidget->findPixels(mDisplay, mRange);
    mWidgets.push_back(inWidget);
}


void HUD::findPixels()
{
    for (unsigned int i = 0; i < mWidgets.size(); ++i)
        mWidgets[i]->findPixels(mDisplay, mRange);
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
    for (unsigned int i = 0; i < mWidgets.size(); ++i)
    {
        if (mWidgets[i]->isVisible()) mWidgets[i]->display();
    }

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
    MouseState hover = inPress ? PRESS : HOVER;

    for (unsigned int i = 0; i < mWidgets.size(); ++i)
    {
        MouseState state = (mWidgets[i]->isOver(inX, inY)
            && mWidgets[i]->isVisible()) ? hover : OUTSIDE;
        mWidgets[i]->setState(state);
        mWidgets[i]->onMouseChange(inX, inY);
        if (state != HUD_OUT) outHit = mWidgets[i]->getID();
    }

    return outHit;
}
