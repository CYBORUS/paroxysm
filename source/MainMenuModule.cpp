#include "MainMenuModule.h"

MainMenuModule::MainMenuModule()
{
    //mNewGameButton = new Button("new_game");
    //mMapEditorButton = new Button("map_editor");

    mNewGameButton = new Button("new_game");
    mMapEditorButton = new Button("map_editor");
}

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

    glGenTextures(NUM_TEXTURES, mTextures);
    mList = glGenLists(1);
    glBindTexture(GL_TEXTURE_2D, mTextures[0]);

     //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
     //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
     //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 10);


    Surface next;

    string names[4] = {"map_editor.png", "map_editor_hover.png", "new_game.png", "new_game_hover.png"};

    stringstream input;

    for (int i = 0; i < 4; ++i)
    {
        input << "./assets/images/" << names[i];

        next = DisplayEngine::loadImage(input.str().c_str());

        input.str("");

        if (!DisplayEngine::loadTexture(next, mTextures[i]))
        {
            return false;
        }

    }

/*
    GLint nOfColors = next->format->BytesPerPixel;
    GLenum tFormat = GL_RGBA;
    if (nOfColors == 4)
    {
        if (next->format->Rmask == 0x000000ff)
            tFormat = GL_RGBA;
        else
            tFormat = GL_BGRA;
    }
    else if (nOfColors == 3)
    {
        if (next->format->Rmask == 0x000000ff)
            tFormat = GL_RGB;
        else
            tFormat = GL_BGR;
    }
    else
    {
        cerr << "failed to load texture -- not true color\n";
        SDL_FreeSurface(next);
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, next->w, next->h,
        0, tFormat, GL_UNSIGNED_BYTE, next->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
*/

    newHover = false;
    mapHover = false;
/*
    glNewList(mList, GL_COMPILE);
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



        glEnable(GL_TEXTURE_2D);

        if (newHover)
        {
            displayButton(0.0f, 1.0f, 3.0f, 0.75f, mTextures[1]);
        }
        else
        {
            displayButton(0.0f, 1.0f, 3.0f, 0.75f, mTextures[0]);
        }

        if (mapHover)
        {
            displayButton(0.0f, -1.0f, 3.0f, 0.75f, mTextures[3]);
        }
        else
        {
            displayButton(0.0f, -1.0f, 3.0f, 0.75f, mTextures[2]);
        }

        glDisable(GL_TEXTURE_2D);
        //displayButton(0.0f, -1.0f, 3.0f, 0.75f, mTextures[3]);

        glDisable(GL_BLEND);

    }
    glEndList();
*/
    mCounter = 0;

    mDisplay.x = SDL_GetVideoSurface()->w;
    mDisplay.y = SDL_GetVideoSurface()->h;


    mHUD.setDisplay(mDisplay, RANGE);

    Button* b = new Button("map_editor");
    b->setLocation(-8.0f, -2.0f);
    b->setSize(4.0f, 1.0f);
    mHUD.addButton(b);


    return true;
}

void MainMenuModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT);
/*
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);

    if (newHover)
    {
        displayButton(0.0f, 1.0f, 3.0f, 0.75f, mTextures[1]);
    }
    else
    {
        displayButton(0.0f, 1.0f, 3.0f, 0.75f, mTextures[0]);
    }

    if (mapHover)
    {
        displayButton(0.0f, -1.0f, 3.0f, 0.75f, mTextures[3]);
    }
    else
    {
        displayButton(0.0f, -1.0f, 3.0f, 0.75f, mTextures[2]);
    }

    glDisable(GL_TEXTURE_2D);
    //displayButton(0.0f, -1.0f, 3.0f, 0.75f, mTextures[3]);

    glDisable(GL_BLEND);
    //glCallList(mList);
*/
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


void MainMenuModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle)
{
    mHUD.setStates(inX, inY, false);

}
