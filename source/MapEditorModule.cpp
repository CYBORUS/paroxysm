#include "MapEditorModule.h"
#include "DisplayEngine.h"
#include "Config.h"

bool MapEditorModule::onInit()
{
    mRunning = true;
    mTrackMode = false;

    int ts = Config::get<int>("terrain size", 10);
    mTerrainHeight = Matrix<int>(ts, ts);
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
            mTerrainVertices[k + 2] = static_cast<GLfloat>(i);
        }
    }

    mNumIndices = (mTerrainHeight.rows() - 1) * (mTerrainHeight.cols() - 1) * 6;
    mTerrainIndices = new GLubyte[mNumIndices];

    int t = 0;
    for (int i = 0; i < mTerrainHeight.rows() - 1; ++i)
    {
        for (int j = 0; j < mTerrainHeight.cols() - 1; ++j)
        {
            int slant = ((i % 2) + (j % 2)) % 2;

            mTerrainIndices[t++] = mTerrainHeight.toIndex(i, j);

            if (slant)
            {
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i + 1, j);
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i, j + 1);
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i + 1, j);
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i + 1, j + 1);
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i, j + 1);
            }
            else
            {
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i + 1, j);
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i + 1, j + 1);
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i, j);
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i + 1, j + 1);
                mTerrainIndices[t++] = mTerrainHeight.toIndex(i, j + 1);
            }
        }
    }

    mTrackball[2] = 10.0f;

    int w = SDL_GetVideoSurface()->w;
    mCenterX = w / 2;
    int h = SDL_GetVideoSurface()->h;
    mCenterY = h / 2;

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
    glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_BYTE,
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
    if (!mTrackMode || (inX == mCenterX && inY == mCenterY)) return;

    mTrackball[1] += static_cast<GLfloat>(inX - mCenterX) * TRACKBALL_STEP;
    mTrackball[0] += static_cast<GLfloat>(inY - mCenterY) * TRACKBALL_STEP;

    SDL_WarpMouse(mCenterX, mCenterY);
}

void MapEditorModule::onLButtonDown(int inX, int inY)
{
    if (mTrackMode)
    {
        SDL_ShowCursor(SDL_ENABLE);
    }
    else
    {
        SDL_WarpMouse(mCenterX, mCenterY);
        SDL_ShowCursor(SDL_DISABLE);
    }

    mTrackMode = !mTrackMode;
}
