#include "GameModule.h"

using namespace std;

GameModule::GameModule() : mXPan(0.0f),
    mYPan(0.0f)
{
    //ctor
}

GameModule::~GameModule()
{
    //dtor
}


void GameModule::onLoad(CGE::PropertyList& inList)
{
    CGE::Actor* a = new CGE::Actor(&mGrid);

    mViewNode.addChildNode(a);
    mBin.addNode(a);
    mActors.push_back(a);


    ifstream fin("assets/maps/Shared_Map.pmf");
    if (!fin)
    {
        cerr << "Error: Map could not be loaded!";
        return;
    }
    size_t size = 20;
    //fin >> mGrid;
    mGrid.create(size, size);
    mGrid.buildVBO();
    fin.close();

//    Button* button = new Button("assets/images/hud/load_map.png", 2.0f, 1.0f);
//    button->setClickListener(uiLoadMap, this);
//    button->setPosition(3.0f, -2.0f);
//    mUI.addWidget(button);
    mUI.update();

    EntityRenderBins bins;
    bins.general = &mBin;
    mPlayerTank = new Tank(&mGrid, a, bins, &mManager);
}

void GameModule::onUnload()
{
}

void GameModule::onOpen()
{
    mMouseState = NONE;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mViewNode.setAngle(-45.0f);
    mViewNode.setDistance(8.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}

void GameModule::onClose()
{
    glDisable(GL_BLEND);
}

void GameModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    mBin.displayAll();
    glDisable(GL_DEPTH_TEST);

    mUI.display();
}

void GameModule::onPulse()
{
//    for (list<CGE::Actor*>::iterator i = mActors.begin();
//        i != mActors.end(); ++i)
//    {
//        CGE::Actor& an = *(*i);
//        an.update();
//    }

//    mSphere->matrix().loadIdentity();
//    mSphere->matrix().translate(mSpherePosition[0], mSpherePosition[1],
//        mSpherePosition[2]);
//    mSphere->matrix().scale(0.1f);

    mViewNode.smartPan(mXPan, mYPan);

    mViewNode.update();
    mViewNode.updateAllMatrices();

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
