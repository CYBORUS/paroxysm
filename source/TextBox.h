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

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "Widget.h"
#include "TextLayer.h"

#include <SDL_opengl.h>

#include <string>
using namespace std;

#define TEXT_OFFSET 0.1f

class TextBox : public Widget
{
    public:
        TextBox();
        virtual ~TextBox();

        virtual void display();
        virtual bool canFocus();
        virtual void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        void setTextColor(Uint8 inRed, Uint8 inGreen, Uint8 inBlue);
        const string& getText();
        void setText(const char* inString);
        void hideOnEnter(bool inChange);

    private:
        void update();

        string mText;
        TextLayer mTextLayer;
        GLuint mTextTexture;
        float mRatio;
        bool mHideOnEnter;
};

inline const string& TextBox::getText()
{
    return mText;
}

#endif