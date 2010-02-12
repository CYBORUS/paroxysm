#include "GameModule.h"

bool GameModule::onLoad()
{
    Surface someImage = NULL;
    string something;
    ScrollList* list = new ScrollList(&something, 2.0f, 5.0f, 34);
    list->setLocation(0.0f, 0.0f);
    list->setSize(3.0f, 3.0f);
    list->addListItem("boogey", someImage);
    list->addListItem("Cyborus");

    mHUD.addWidget(list);

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
