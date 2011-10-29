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

#include "PlainPic.h"

PlainPic::PlainPic()
{
    glGenTextures(1, &mTexture);
}

PlainPic::~PlainPic()
{
    glDeleteTextures(1, &mTexture);
}

void PlainPic::setImage(Surface inSurface)
{
    DisplayEngine::loadTexture(inSurface, mTexture, true);
}

void PlainPic::display()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, mTexture);

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
