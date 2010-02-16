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

#ifndef HUD_H
#define HUD_H

#include "Point2D.h"
#include "Widget.h"
#include "Button.h"

#include <vector>
#include <iostream>
using namespace std;

#define HUD_RANGE 8.0f
#define HUD_OUT 0
#define HUD_HOVER 1
#define HUD_PRESS 2

class HUD
{
    public:
        HUD();
        ~HUD();

        void setDisplay(const Point2D<int>& inDisplay,
            float inRange = HUD_RANGE);
        void onKeyPress(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        void addWidget(Widget* inWidget);
        void findPixels();
        void display();
        int setStates(int inX, int inY, bool inPress);

        bool hasFocus();

    private:
        vector<Widget*> mWidgets;
        Point2D<int> mDisplay;
        float mRange;

        Widget* mFocusWidget;
};

inline bool HUD::hasFocus()
{
    return (mFocusWidget != NULL);
}

#endif
