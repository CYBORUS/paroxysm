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

#ifndef BUTTON_H
#define BUTTON_H

#include <SDL_opengl.h>
#include "Widget.h"
#include "SoundEngine.h"

#include <string>
using namespace std;

class Button : public Widget
{
    public:
        Button(const char* inKeyword, int inID);
        virtual ~Button();

        virtual void display();
        virtual void onStateChange();

    private:
        void assemble(string& inString, const char* inAdd);

        static string mPrefix;
        static string mSuffix;
        static SoundEffect mHoverSound;
        static SoundEffect mClickSound;

        string mKeyword;
        GLuint mTextures[4];
        MouseState mLastState;
};

#endif
