#include "MapEditorModule.h"
#include "DisplayEngine.h"

bool MapEditorModule::onInit()
{
    mRunning = true;
    mTerrainHeight = Matrix<int>(DEFAULT_TERRAIN_SIZE, DEFAULT_TERRAIN_SIZE);
    mTerrainHeight(2, 2) = 3;
    mTerrainVertices = new GLfloat[mTerrainHeight.size() * 3];

    for (int i = 0; i < mTerrainHeight.rows(); ++i)
    {
        for (int j = 0; j < mTerrainHeight.cols(); ++j)
        {
            int k = mTerrainHeight.toIndex(i, j) * 3;
            mTerrainVertices[k] = static_cast<GLfloat>(j);
            mTerrainVertices[k + 1] = static_cast<GLfloat>(mTerrainHeight(i, j))
                * 0.5f;
            mTerrainVertices[k + 2] = static_cast<GLfloat>(i) * 0.5f;
        }
    }

    mNumIndices = (mTerrainHeight.rows() - 1) * (mTerrainHeight.cols() * 2 - 1)
        + 1;
    mTerrainIndices = new GLubyte[mNumIndices];
    mTerrainIndices[0] = 0;

    int t = 1;
    for (int i = 0; i < mTerrainHeight.rows() - 1; ++i)
    {
        int j;

        if (i % 2)
        {
            for (j = mTerrainHeight.cols() - 1; j > 0; --j)
            {
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i + 1, j);
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i, j - 1);
            }
        }
        else
        {
            for (j = 0; j < mTerrainHeight.cols() - 1; ++j)
            {
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i + 1, j);
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i, j + 1);
            }
        }

        mTerrainIndices[t++] = mTerrainHeight.toIndex(i + 1, j);
    }

    mTrackball[2] = 10.0f;

    int w = SDL_GetVideoSurface()->w;
    mCenterX = w / 2;
    int h = SDL_GetVideoSurface()->h;
    mCenterY = h / 2;

    SDL_ShowCursor(SDL_DISABLE);
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(FIELD_OF_VIEW, (GLdouble)w / (GLdouble)h, NEAR_CP, FAR_CP);

    glMatrixMode(GL_MODELVIEW);

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

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, mTerrainVertices);
    glDrawElements(GL_TRIANGLE_STRIP, mNumIndices, GL_UNSIGNED_BYTE,
        mTerrainIndices);
    glPopMatrix();
}

void MapEditorModule::onFrame()
{
}

void MapEditorModule::onCleanup()
{
    SDL_ShowCursor(SDL_ENABLE);

    delete [] mTerrainVertices;
    delete [] mTerrainIndices;
}

void MapEditorModule::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    if (inSym == SDLK_ESCAPE)
    {
        mRunning = false;
        return;
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
    if (inX == mCenterX && inY == mCenterY) return;

    mTrackball[1] += static_cast<GLfloat>(inX - mCenterX) * TRACKBALL_STEP;
    mTrackball[0] += static_cast<GLfloat>(inY - mCenterY) * TRACKBALL_STEP;

    SDL_WarpMouse(mCenterX, mCenterY);
}
