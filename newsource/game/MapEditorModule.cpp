#include "MapEditorModule.h"
#include "TestActorNode.h"
#include <CGE/Exception.h>

#include <fstream>
using namespace std;

MapEditorModule::MapEditorModule() : mLeftClickDown(false)
{
    mModel = mManager.load("bradley.c3m");
}

MapEditorModule::~MapEditorModule()
{
}

void MapEditorModule::onLoad(CGE::PropertyList& inList)
{
    ActorNode* an = new ActorNode(mGrid);
    mViewNode.addChildNode(*an);
    mBin.addNode(*an);
    mActors.push_back(an);

    an = new TestActorNode(*mModel);
    mViewNode.addChildNode(*an);
    mBin.addNode(*an);
    mActors.push_back(an);

    ifstream fin("assets/maps/Shared_Map.pmf");
    if (!fin)
        throw CGE::Exception("", "no map to load");

    fin >> mGrid;
    fin.close();
}

void MapEditorModule::onUnload()
{
}

void MapEditorModule::onOpen()
{
    mViewNode.setAngle(-45.0f);
    mViewNode.setDistance(8.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
}

void MapEditorModule::onClose()
{
    glDisable(GL_DEPTH_TEST);
}

void MapEditorModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mBin.displayAll();
}

void MapEditorModule::onPulse()
{
    for (list<ActorNode*>::iterator i = mActors.begin();
        i != mActors.end(); ++i)
    {
        ActorNode& an = *(*i);
        an.update();
    }
    mViewNode.smartPan(mXPan, mYPan);

    mViewNode.update();
    mViewNode.updateAllMatrices();
}

void MapEditorModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
    bool inLeft, bool inRight, bool inMiddle)
{
    if (mLeftClickDown)
    {
        mViewNode.changeRotation((inX - mXStart)/2);
        mViewNode.changeAngle((inY - mYStart)/2);
        mXStart = inX;
        mYStart = inY;
    }

    switch(mMouseState)
    {
        case PANNING:
        {
            mViewNode.smartPan(-inRelX * 0.1f, inRelY * 0.1f);
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

void MapEditorModule::onLButtonDown(int inX, int inY)
{
    Uint8* keys = SDL_GetKeyState(NULL);

    if (keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT])
    {
        mMouseState = PANNING;
    }

}

void MapEditorModule::onLButtonUp(int inX, int inY)
{
    mMouseState = NONE;
}

void MapEditorModule::onRButtonDown(int inX, int inY)
{
    mMouseState = ROTATING;
}

void MapEditorModule::onRButtonUp(int inX, int inY)
{
    mMouseState = NONE;
}

void MapEditorModule::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    switch (inSym)
    {
        case SDLK_ESCAPE:
        {
            mRunning = false;
            break;
        }

        case SDLK_w:
        {
            mYPan = 0.5f;
            break;
        }

        case SDLK_a:
        {
            mXPan = -0.5f;
            break;
        }

        case SDLK_s:
        {
            mYPan = -0.5f;
            break;
        }

        case SDLK_d:
        {
            mXPan = 0.5f;
            break;
        }

        default: {}
    }
}

void MapEditorModule::onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
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

void MapEditorModule::onMouseWheel(bool inUp)
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

void MapEditorModule::onResize(int inWidth, int inHeight)
{
    GLfloat ratio = static_cast<GLfloat>(inWidth)
        / static_cast<GLfloat>(inHeight);
    CGE::Matrix4x4<GLfloat> projection;
    projection.perspective(30.0f, ratio, 1.0f, 1000.0f);
    mViewNode.setProjection(projection);
    glViewport(0, 0, inWidth, inHeight);
}
