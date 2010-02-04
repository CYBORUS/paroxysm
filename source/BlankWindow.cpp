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

#include "BlankWindow.h"

#include <SDL_opengl.h>

bool BlankWindow::onLoad()
{
    SoundEngine::loadBackgroundMusic("portal_still_alive.wav");
    return true;
}

void BlankWindow::onInit()
{
    mRunning = true;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    int w = SDL_GetVideoSurface()->w;
    int h = SDL_GetVideoSurface()->h;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    gluPerspective(FIELD_OF_VIEW, (GLdouble)w / (GLdouble)h, NEAR_CP, FAR_CP);

    glMatrixMode(GL_MODELVIEW);
}

void BlankWindow::onLoop()
{
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -10.0f);
    mSphere.display();
    glPopMatrix();
}

void BlankWindow::onCleanup()
{
}


void BlankWindow::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    cerr << "keydown" << endl;
    SoundEngine::playBackgroundMusic();
}
