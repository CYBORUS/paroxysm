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

    glGenTextures(NUM_TEXTURES, mTextures);
    mList = glGenLists(1);
    glBindTexture(GL_TEXTURE_2D, mTextures[0]);

     //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
     //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
     //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 10);


    Surface next;

    string names[4] = {"map_editor.png", "map_editor_hover.png", "new_game.png", "new_game_hover.png"};

    stringstream input;

    input << "./assets/images/" << names[0];

    next = DisplayEngine::loadImage(input.str().c_str());

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
    if (!DisplayEngine::loadTexture(next, mTextures[0]))
    {
        return false;
    }

    glNewList(mList, GL_COMPILE);
    {

        //glBindTexture(GL_TEXTURE_2D, mTextures[0]);

        glBegin(GL_QUADS);
        {
            glColor3f(1.0f, 1.0f, 1.0f);
            glNormal3f(0.0f, 0.0f, 1.0f);
            glTexCoord2i(0, 1);
            glVertex2f(-6.0f, -0.5f);
            glTexCoord2i(0, 0);
            glVertex2f(-6.0f, 0.5f);
            glTexCoord2i(1, 0);
            glVertex2f(6.0f, 0.5f);
            glTexCoord2i(1, 1);
            glVertex2f(6.0f, -0.5f);
        }
        glEnd();

    }
    glEndList();

    return true;
}

void MainMenuModule::onLoop()
{
            glEnable(GL_TEXTURE_2D);

    //cerr << "looping" << endl;
    glCallList(mList);
        glDisable(GL_TEXTURE_2D);

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

