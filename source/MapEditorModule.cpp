#include "MapEditorModule.h"
#include "DisplayEngine.h"
#include "Config.h"

bool MapEditorModule::onInit()
{
    mRunning = true;
    mMouseMode = MM_DEFAULT;

    int ts = Config::get<int>("terrain size", 10);
    mTerrainHeight = Matrix<int>(ts, ts);

    mTerrainVertices = new GLfloat[mTerrainHeight.size() * 3];

    mTerrainHeight(1,1) = 3;
    mTerrainHeight(4,1) = 3;

    for (int i = 0; i < mTerrainHeight.rows(); ++i)
    {
        for (int j = 0; j < mTerrainHeight.cols(); ++j)
        {
            int k = mTerrainHeight.toIndex(i, j) * 3;
            mTerrainVertices[k] = static_cast<GLfloat>(j);
            mTerrainVertices[k + 1] = static_cast<GLfloat>(mTerrainHeight(i, j))
                * 0.5f;
            mTerrainVertices[k + 2] = static_cast<GLfloat>(i);
        }
    }

    mNumIndices = (mTerrainHeight.rows() - 1) * (mTerrainHeight.cols() - 1) * 6;
    mTerrainIndices = new GLuint[mNumIndices];

    int t = 0;
    for (int i = 0; i < mTerrainHeight.rows() - 1; ++i)
    {
        for (int j = 0; j < mTerrainHeight.cols() - 1; ++j)
        {
            int slant = ((i % 2) + (j % 2)) % 2;

            mTerrainIndices[t++] = mTerrainHeight.toIndex(i, j);
            mTerrainIndices[t++] = mTerrainHeight.toIndex(i + 1, j);

            if (slant)
            {
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i, j + 1);
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i + 1, j);
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i + 1, j + 1);
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i, j + 1);
            }
            else
            {
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i + 1, j + 1);
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i, j);
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i + 1, j + 1);
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i, j + 1);
            }
        }
    }


    mTrackball[0] = 22.0f;
    mTrackball[2] = 20.0f;
    mPanning[0] = static_cast<GLfloat>(mTerrainHeight.cols()) / -2.0f;
    mPanning[2] = static_cast<GLfloat>(mTerrainHeight.rows()) / -2.0f;

    int w = SDL_GetVideoSurface()->w;
    mCenterX = w / 2;
    int h = SDL_GetVideoSurface()->h;
    mCenterY = h / 2;

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(FIELD_OF_VIEW, (GLdouble)w / (GLdouble)h, NEAR_CP, FAR_CP);


    glMatrixMode(GL_MODELVIEW);
/*
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, mTerrainVertices);

    glNewList(mList, GL_COMPILE);
    {

        glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT,
            mTerrainIndices);
    }
    glEndList();

        glDisableClientState(GL_VERTEX_ARRAY);
*/
    return true;
}

void MapEditorModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -mTrackball[2]);
    glRotatef(mTrackball[0], 1.0f, 0.0f, 0.0f);
    glRotatef(mTrackball[1], 0.0f, 1.0f, 0.0f);

    glTranslatef(mPanning[0], mPanning[1], mPanning[2]);

    //glCallList(mList);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, mTerrainVertices);
    glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT,
        mTerrainIndices);

    glPopMatrix();
}

void MapEditorModule::onFrame()
{
}

void MapEditorModule::onCleanup()
{
    delete [] mTerrainVertices;
    delete [] mTerrainIndices;
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

        case SDLK_SPACE:
        {
            mTrackball[0] = 22.0f;
            mTrackball[1] = 0.0f;
            mTrackball[2] = 20.0f;
            mPanning[0] = static_cast<GLfloat>(mTerrainHeight.cols()) / -2.0f;
            mPanning[2] = static_cast<GLfloat>(mTerrainHeight.rows()) / -2.0f;
            break;
        }

        default:
        {
            break;
        }
    }
}

void MapEditorModule::onMouseWheel(bool inUp, bool inDown)
{
    if (inUp)
        mTrackball[2] -= TRACKBALL_STEP;
    else if (inDown)
        mTrackball[2] += TRACKBALL_STEP;

    if (mTrackball[2] < 0.0f) mTrackball[2] = 0.0f;
}

void MapEditorModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
    bool inLeft, bool inRight, bool inMiddle)
{
    if (mMouseMode == MM_DEFAULT || (inX == mCenterX && inY == mCenterY))
        return;

    if (mMouseMode == MM_ROTATING)
    {
        mTrackball[1] += static_cast<GLfloat>(inX - mCenterX) * TRACKBALL_STEP;
        if (mTrackball[1] < -180.0f)
            mTrackball[1] += 360.0f;
        else if (mTrackball[1] > 180.0f)
            mTrackball[1] -= 360.0f;

        mTrackball[0] += static_cast<GLfloat>(inY - mCenterY) * TRACKBALL_STEP;
        if (mTrackball[0] < -180.0f)
            mTrackball[0] += 360.0f;
        else if (mTrackball[0] > 180.0f)
            mTrackball[0] -= 360.0f;
    }
    else if (mMouseMode == MM_PANNING)
    {
        GLfloat dx = static_cast<GLfloat>(inX - mCenterX) * PANNING_STEP;
        GLfloat dy = static_cast<GLfloat>(inY - mCenterY) * PANNING_STEP;

        GLfloat theta = TO_RADIANS(mTrackball[1]);
        GLfloat dxp = cos(theta) * dx;
        GLfloat dyp = sin(theta) * dx;
        dxp -= sin(theta) * dy;
        dyp += cos(theta) * dy;

        mPanning[0] += dxp;
        mPanning[2] += dyp;
    }

    SDL_WarpMouse(mCenterX, mCenterY);
}

void MapEditorModule::onLButtonDown(int inX, int inY)
{
    if (mMouseMode != MM_DEFAULT) return;

    SDL_ShowCursor(SDL_DISABLE);
    mOldMouseX = inX;
    mOldMouseY = inY;
    SDL_WarpMouse(mCenterX, mCenterY);
    mMouseMode = MM_PANNING;
}

void MapEditorModule::onLButtonUp(int inX, int inY)
{
    if (mMouseMode != MM_PANNING) return;

    mMouseMode = MM_DEFAULT;
    SDL_WarpMouse(mOldMouseX, mOldMouseY);
    SDL_ShowCursor(SDL_ENABLE);
}

void MapEditorModule::onRButtonDown(int inX, int inY)
{
    if (mMouseMode != MM_DEFAULT) return;

    SDL_ShowCursor(SDL_DISABLE);
    mOldMouseX = inX;
    mOldMouseY = inY;
    SDL_WarpMouse(mCenterX, mCenterY);
    mMouseMode = MM_ROTATING;
}

void MapEditorModule::onRButtonUp(int inX, int inY)
{
    if (mMouseMode != MM_ROTATING) return;

    mMouseMode = MM_DEFAULT;
    SDL_WarpMouse(mOldMouseX, mOldMouseY);
    SDL_ShowCursor(SDL_ENABLE);
}
