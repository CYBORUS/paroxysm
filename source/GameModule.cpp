#include "GameModule.h"

bool GameModule::onLoad()
{
    //Surface someImage = DisplayEngine::loadImage("assets/images/green.png");
    ScrollList* maps = new ScrollList(&mSelectedMap, 2.0f, 5.0f, 34);
    maps->setLocation(-5.0f, 3.0f);
    maps->setSize(3.0f, 3.0f);
    maps->setFontSize(20);
    maps->addListItem("boogey");
    //maps->setFontSize(24);
    maps->addListItem("Cyborus");
    maps->addListItem("Cdaragorn");
    maps->addListItem("TheBuzzSaw");
    maps->addListItem("They RopopoULE ALL!!!");

    Surface someImage = DisplayEngine::loadImage("assets/images/hud/load_map_up_arrow.png");

    maps->setUpArrow(someImage);

    someImage = DisplayEngine::loadImage("assets/images/hud/load_map_down_arrow.png");

    maps->setDownArrow(someImage);

    TextBox* typing = new TextBox();
    typing->setLocation(2.0f, 0.0f);
    typing->setSize(10.0f, 1.0f);

    mHUD.addWidget(maps);
    mHUD.addWidget(typing);

    return true;
}

void GameModule::onInit()
{
    mRunning = true;
    mNextModule = NULL;

    GLdouble ratio = 0;
    int w = SDL_GetVideoSurface()->w;
    int h = SDL_GetVideoSurface()->h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    //gluPerspective(FIELD_OF_VIEW, (GLdouble)w / (GLdouble)h, NEAR_CP, FAR_CP);

    //we need to deal with the possibility of a monitor on it's side
    if (w >= h)
    {
        ratio = (GLdouble)w / (GLdouble)h;
        glOrtho(-GAME_RANGE * ratio, GAME_RANGE * ratio, -GAME_RANGE, GAME_RANGE, -10.0, 10.0);
    }
    else
    {
        ratio = (GLdouble)h / (GLdouble)w;
        glOrtho(-GAME_RANGE, GAME_RANGE, -GAME_RANGE * ratio, GAME_RANGE * ratio, 1.0, 10.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Point2D<int> display;
    display.x = 1024;
    display.y = 768;

    mHUD.setDisplay(display, GAME_RANGE);

}

void GameModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT);
    mHUD.display();
}

void GameModule::onFrame()
{
}

void GameModule::onCleanup()
{
}

Module* GameModule::next()
{
    return mNextModule;
}


void GameModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
    bool inLeft, bool inRight, bool inMiddle)
{
    mHUD.setStates(inX, inY, inLeft);
}

void GameModule::onMouseWheel(bool inUp, bool inDown)
{
}

void GameModule::onLButtonDown(int inX, int inY)
{
    mHUD.setStates(inX, inY, true);
    cerr << "selected map: " << mSelectedMap << endl;
}


void GameModule::onLButtonUp(int inX, int inY)
{
    mHUD.setStates(inX, inY, false);
}


void GameModule::onRButtonDown(int inX, int inY)
{
}


void GameModule::onRButtonUp(int inX, int inY)
{
}



