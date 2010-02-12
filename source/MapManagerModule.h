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

#ifndef MAPMANAGERMODULE_H
#define MAPMANAGERMODULE_H

#include "Module.h"
#include "HUD.h"
#include "Button.h"

/// button IDs
#define MMM_NUM_BUTTONS 1
#define B_SAVE 0

class MapManagerModule : public Module
{
    public:
        virtual bool onLoad();
        virtual void onInit();
        virtual void onLoop();
        virtual void onCleanup();

    private:
        HUD mHUD;
        Button* mButtons[MMM_NUM_BUTTONS];
};

#endif
