#include "MainMenuModule.h"
#include "PlainPic.h"

#include "Model3D.h" // testing purposes only

bool MainMenuModule::onLoad()
{
    Button* b = new Button("map_editor", MAP_EDITOR);
    b->setLocation(0.0f, -1.0f);
    b->setSize(8.0f, 2.0f);
    mHUD.addWidget(b);

    b = new Button("new_game", NEW_GAME);
    b->setLocation(0.0f, 1.0f);
    b->setSize(8.0f, 2.0f);
    mHUD.addWidget(b);

    PlainPic* pp = new PlainPic;
    pp->setImage(DisplayEngine::loadImage("assets/images/title.png"));
    pp->setLocation(0.0f, 6.0f);
    pp->setSize(24.0f, 6.0f);
    mHUD.addWidget(pp);

    Model3D m("cube.obj"); // testing purposes only

    return true;
}

void MainMenuModule::onInit()
{
    mRunning = true;
    mDead = true;
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
        glOrtho(-MAIN_MENU_RANGE * ratio, MAIN_MENU_RANGE * ratio, -MAIN_MENU_RANGE, MAIN_MENU_RANGE, -10.0, 10.0);
    }
    else
    {
        ratio = (GLdouble)h / (GLdouble)w;
        glOrtho(-MAIN_MENU_RANGE, MAIN_MENU_RANGE, -MAIN_MENU_RANGE * ratio, MAIN_MENU_RANGE * ratio, 1.0, 10.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    mDisplay.x = SDL_GetVideoSurface()->w;
    mDisplay.y = SDL_GetVideoSurface()->h;

    mHUD.setDisplay(mDisplay, MAIN_MENU_RANGE);
}

void MainMenuModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT);

    mHUD.display();
}

void MainMenuModule::onFrame()
{

}

void MainMenuModule::onCleanup()
{
}

Module* MainMenuModule::next()
{
    return mNextModule;
}


void MainMenuModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle)
{
    if (inLeft)
    {
        mHUD.setStates(inX, inY, true);
    }
    else
    {
        mHUD.setStates(inX, inY, false);
    }

}

void MainMenuModule::onButtonPress(int inID)
{
    switch(inID)
    {
        case MAP_EDITOR:
        {
            mNextModule = new LoadMapModule;
            mRunning = false;
            mDead = false;
            break;
        }
        case NEW_GAME:
        {
            mNextModule = new LoadGameModule;
            mRunning = false;
            mDead = false;
            break;
        }
        default:
        {
            break;
        }
    }
}


bool MainMenuModule::isDead()
{
    return mDead;
}
