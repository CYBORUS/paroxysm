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

#ifndef _DISPLAYENGINE_H_
#define _DISPLAYENGINE_H_

#include "Module.h"

#include <SDL.h>
typedef SDL_Surface* Surface;

#define ENGINE_FPS 60
#define FRAME_LENGTH (1000 / ENGINE_FPS)

class DisplayEngine
{
    public:
        static void start(Module* inModule);
        static void initialize();
        static Surface loadImage(const char* inFile);

    private:
        static void cleanup();

        static Surface mDisplay;
        static Surface mWindowIcon;
        static SDL_Rect** mModes;
        static unsigned int mNextFrame;
        static unsigned int mCurrentFrame;
};

#endif