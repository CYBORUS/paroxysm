#include "MainMenuModule.h"

bool MainMenuModule::onInit()
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
        glOrtho(-RANGE * ratio, RANGE * ratio, -RANGE, RANGE, -10.0, 10.0);
    }
    else
    {
        ratio = (GLdouble)h / (GLdouble)w;
        glOrtho(-RANGE, RANGE, -RANGE * ratio, RANGE * ratio, 1.0, 10.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    mDisplay.x = SDL_GetVideoSurface()->w;
    mDisplay.y = SDL_GetVideoSurface()->h;


    mHUD.setDisplay(mDisplay, RANGE);

    Button* b = new Button("map_editor", 2);
    b->setLocation(-8.0f, -2.0f);
    b->setSize(8.0f, 2.0f);
    mHUD.addButton(b);


    return true;
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
    mHUD.deleteButtons();
}

Module* MainMenuModule::next()
{
    return mNextModule;
}

/*
void MainMenuModule::displayButton(float inX, float inY, float inWidthRange, float inHeightRange, GLuint inTexture)
{
    glBindTexture(GL_TEXTURE_2D, inTexture);
    glBegin(GL_QUADS);
    {
        glTexCoord2i(0, 1);
        glVertex2f(inX - inWidthRange, inY - inHeightRange);
        glTexCoord2i(1, 1);
        glVertex2f(inX + inWidthRange, inY - inHeightRange);
        glTexCoord2i(1, 0);
        glVertex2f(inX + inWidthRange, inY + inHeightRange);
        glTexCoord2i(0, 0);
        glVertex2f(inX - inWidthRange, inY + inHeightRange);
    }
    glEnd();
}
*/

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


void MainMenuModule::onLButtonDown(int inX, int inY)
{
    mHUD.setStates(inX, inY, true);
}

void MainMenuModule::onLButtonUp(int inX, int inY)
{
    if (mHUD.setStates(inX, inY, false))
    {
        mNextModule = new MapEditorModule();
        mRunning = false;
    }
}
