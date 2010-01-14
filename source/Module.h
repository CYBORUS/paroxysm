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

#ifndef _MODULE_H_
#define _MODULE_H_

#include <SDL.h>

#include <SDL_opengl.h>

class Module
{
    public:
        virtual ~Module();

        /// module operation
        virtual bool onInit() = 0;
        virtual void onLoop();
        virtual void onFrame();
        virtual void onCleanup() = 0;
        virtual bool isDead();
        virtual Module* next();
        bool isRunning();

        /// input events
        void onEvent(SDL_Event* inEvent);
        virtual void onInputFocus();
        virtual void onInputBlur();
        virtual void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onMouseFocus();
        virtual void onMouseBlur();
        virtual void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);
        virtual void onMouseWheel(bool inUp, bool inDown);
        virtual void onLButtonDown(int inX, int inY);
        virtual void onLButtonUp(int inX, int inY);
        virtual void onRButtonDown(int inX, int inY);
        virtual void onRButtonUp(int inX, int inY);
        virtual void onMButtonDown(int inX, int inY);
        virtual void onMButtonUp(int inX, int inY);
        virtual void onJoyAxis(Uint8 inWhich, Uint8 inAxis, Sint16 inValue);
        virtual void onJoyButtonDown(Uint8 inWhich, Uint8 inButton);
        virtual void onJoyButtonUp(Uint8 inWhich, Uint8 inButton);
        virtual void onJoyHat(Uint8 inWhich, Uint8 inHat, Uint8 inValue);
        virtual void onJoyBall(Uint8 inWhich, Uint8 inBall, Sint16 inXRel,
            Sint16 inYRel);
        virtual void onMinimize();
        virtual void onRestore();
        virtual void onResize(int inWidth, int inHeight);
        virtual void onExpose();
        virtual void onExit();
        virtual void onUser(Uint8 inType, int inCode, void* inData1,
            void* inData2);

    protected:
        bool mRunning;
};

inline bool Module::isRunning()
{
    return mRunning;
}

#endif

