#include "GameModule.h"
#include <CGE/Download.h>
#include <CGE/Heap.h>

using namespace std;

GameModule::GameModule() : mXPan(0.0f), mYPan(0.0f)
{
    CGE::Download d; // only here for testing curl linking
}

GameModule::~GameModule()
{
}

void GameModule::onLoad(CGE::PropertyList& inList)
{
}

void GameModule::onUnload()
{
}

void GameModule::onOpen()
{
    mMouseState = NONE;

    mViewNode.setAngle(-45.0f);
    mViewNode.setDistance(8.0f);

    // Prevent the first-frame flicker.
    mLuaAPI.update(mProjection);
    mViewNode.update();
    mViewNode.updateAllMatrices();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void GameModule::onClose()
{
}

void GameModule::onLoop()
{
    //cerr << "start loop" << endl;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //cerr << "clear passed" << endl;

    mLuaAPI.display();
    //cerr << "lua display" << endl;
    mUI.display();
    //cerr << "ui display" << endl;
}

void GameModule::onPulse()
{
    //cerr << "start pulse" << endl;
    mViewNode.smartPan(mXPan, mYPan);

    mViewNode.update();
    mViewNode.updateAllMatrices();

    mLuaAPI.update(mProjection);

    mUI.update();
    //cerr << "end pulse" << endl;
}

void GameModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
    bool inLeft, bool inRight, bool inMiddle)
{
    mUI.onMouseMove(inX, inY);

    switch(mMouseState)
    {
        case NONE:
        {

            break;
        }

        case PANNING:
        {
            mViewNode.smartPan(-inRelX * 0.01f, inRelY * 0.01f);
            break;
        }

        case ROTATING:
        {
            mViewNode.changeAngle(inRelY);
            mViewNode.changeRotation(inRelX);
            break;
        }

        default: {}
    }
}

void GameModule::onMouseWheel(bool inUp)
{
    if (inUp)
    {
        mViewNode.changeDistance(-0.5f);
    }
    else
    {
        mViewNode.changeDistance(0.5f);
    }
}

void GameModule::onLButtonDown(int inX, int inY)
{
    Uint8* keys = SDL_GetKeyState(NULL);
    mUI.onMouseDown();

    if (keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT])
    {
        mMouseState = PANNING;
    }
    else
    {

    }

}

void GameModule::onLButtonUp(int inX, int inY)
{
    mMouseState = NONE;
    mUI.onMouseUp();
}

void GameModule::onRButtonDown(int inX, int inY)
{
    mMouseState = ROTATING;
}

void GameModule::onRButtonUp(int inX, int inY)
{
    mMouseState = NONE;
}

void GameModule::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    mLuaAPI.onKey(inSym, 1.0);

    switch (inSym)
    {
        case SDLK_ESCAPE:
        {
            mRunning = false;
            break;
        }

        case SDLK_BACKSLASH:
        {
            CGE::Heap::dump();
            break;
        }

        default: {}
    }
}

void GameModule::onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    mLuaAPI.onKey(inSym, 0.0);

    switch (inSym)
    {
        default: {}
    }
}

void GameModule::onJoyAxis(Uint8 inWhich, Uint8 inAxis, Sint16 inValue)
{
    double value = 0;
    if (inValue > 0)
    {
        value = (double)inValue / (double)32767;
    }
    else
    {
        value = (double)inValue / (double)32768;
    }

    mLuaAPI.onJoystickAxis(inWhich, inAxis, value);
}

void GameModule::onJoyButtonDown(Uint8 inWhich, Uint8 inButton)
{
    mLuaAPI.onJoystickButton(inWhich, inButton, 1);
}

void GameModule::onJoyButtonUp(Uint8 inWhich, Uint8 inButton)
{
    mLuaAPI.onJoystickButton(inWhich, inButton, 0);
}

void GameModule::onJoyHat(Uint8 inWhich, Uint8 inHat, Uint8 inValue)
{
    double value = 0;
    if (inValue > 0)
    {
        value = (double)inValue / (double)32767;
    }
    else
    {
        value = (double)inValue / (double)32768;
    }

    mLuaAPI.onJoystickHat(inWhich, inHat, value);
}
