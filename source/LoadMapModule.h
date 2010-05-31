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

#ifndef LOADMAPMODULE_H
#define LOADMAPMODULE_H

#include "Module.h"
#include "Point2D.h"
#include "HUD.h"

#include "Button.h"
#include "ScrollList.h"
#include "TextBox.h"
#include "Label.h"
#include "ScrollList.h"

#include "MapEditorModule.h"

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

#include <iostream>
#include <cstdlib>
using namespace std;

enum LoadMapHUD { X_SIZE, Y_SIZE };


class LoadMapModule : public Module
{
    public:
        bool onLoad();
        void onOpen();
        void onLoop();
        void onFrame();
        Module* next();

    protected:
        void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);
        void onMouseWheel(bool inUp, bool inDown);
        void onLButtonDown(int inX, int inY);
        void onLButtonUp(int inX, int inY);
        void onRButtonDown(int inX, int inY);
        void onRButtonUp(int inX, int inY);

        void onButtonPress(int inID);

        Module* mNextModule;

        TextBox* mMapSize[2];

        string mSelectedMap;
};

#endif // LOADMAPMODULE_H
