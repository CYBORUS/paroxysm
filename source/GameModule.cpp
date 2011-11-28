#include "GameModule.h"
#include <CGE/Download.h>

using namespace std;

GameModule::GameModule() : mLuaAPI(mViewNode), mXPan(0.0f), mYPan(0.0f)
{
    CGE::Download d; // only here for testing curl linking
    memset(mKeyCommands, 0, sizeof(mKeyCommands));
    loadKeyCommands();
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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void GameModule::onClose()
{
}

void GameModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mLuaAPI.display();
    mUI.display();
}

void GameModule::onPulse()
{
    mViewNode.smartPan(mXPan, mYPan);

    mViewNode.update();
    mViewNode.updateAllMatrices();

    mLuaAPI.update();

    mUI.update();
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
    switch (inSym)
    {
        case SDLK_ESCAPE:
        {
            issueLuaCommand(SDLK_ESCAPE, 1);
            mRunning = false;
            break;
        }

        case SDLK_w:
        {
            issueLuaCommand(SDLK_w, 1);
            mYPan = 0.5f;
            break;
        }

        case SDLK_a:
        {
            issueLuaCommand(SDLK_a, 1);
            mXPan = -0.5f;
            break;
        }

        case SDLK_s:
        {
            issueLuaCommand(SDLK_s, 1);
            mYPan = -0.5f;
            break;
        }

        case SDLK_d:
        {
            issueLuaCommand(SDLK_d, 1);
            mXPan = 0.5f;
            break;
        }

        default: {}
    }
}

void GameModule::onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    switch (inSym)
    {
        case SDLK_w:
        {
            if (mYPan > 0.0f)
                mYPan = 0.0f;
            break;
        }

        case SDLK_s:
        {
            if (mYPan < 0.0f)
                mYPan = 0.0f;
            break;
        }

        case SDLK_a:
        {
            if (mXPan < 0.0f)
                mXPan = 0.0f;
            break;
        }

        case SDLK_d:
        {
            if (mXPan > 0.0f)
                mXPan = 0.0f;
            break;
        }

        default: {}
    }
}

void GameModule::loadKeyCommands()
{
    const vector<LuaInputCommand*>& inputCommands = mLuaAPI.getLuaInputCommands();
    for (int i = 0; i < inputCommands.size(); i++)
    {
        lua_Integer keyNum = inputCommands[i]->getKeyNum();
        mKeyCommands[keyNum] = inputCommands[i];
    }
}

void GameModule::issueLuaCommand(SDLKey inKey, int inIntensity)
{
    if (mKeyCommands[inKey])
        mKeyCommands[inKey]->issueCommand(inIntensity);
}
