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
}

TextBox::~TextBox()
{
}

void TextBox::display()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    {
        glVertex2f(mLocation.x - (mSize.x / 2.0f),
            mLocation.y - (mSize.y / 2.0f));
        glVertex2f(mLocation.x + (mSize.x / 2.0f),
            mLocation.y - (mSize.y / 2.0f));
        glVertex2f(mLocation.x + (mSize.x / 2.0f),
            mLocation.y + (mSize.y / 2.0f));
        glVertex2f(mLocation.x - (mSize.x / 2.0f),
            mLocation.y + (mSize.y / 2.0f));
    }
    glEnd();
}
