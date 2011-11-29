#include "GameModule.h"
#include <CGE/Download.h>

using namespace std;

GameModule::GameModule() : mXPan(0.0f), mYPan(0.0f)
{
    CGE::Download d; // only here for testing curl linking
    memset(mKeyCommands, 0, sizeof(mKeyCommands));
    loadKeyCommands();
    mIntensity = 0.05;
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mLuaAPI.display();
    mUI.display();
}

void GameModule::onPulse()
{
    mViewNode.smartPan(mXPan, mYPan);

    mViewNode.update();
    mViewNode.updateAllMatrices();

    mLuaAPI.update(mProjection);

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
    issueLuaCommand(inSym, 1);
}

void GameModule::onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    issueLuaCommand(inSym, 0);
}

void GameModule::loadKeyCommands()
{
    const vector<LuaInputCommand*>& inputCommands =
        mLuaAPI.getLuaInputCommands();

    for (size_t i = 0; i < inputCommands.size(); i++)
    {
        lua_Integer keyNum = inputCommands[i]->getKeyNum();
        mKeyCommands[keyNum] = inputCommands[i];
    }
}

void GameModule::issueLuaCommand(SDLKey inKey, double inIntensity)
{
    const LuaInputCommand* lic = mKeyCommands[inKey];
    if (lic) lic->issueCommand(inIntensity);
}
