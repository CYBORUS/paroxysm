/**
 *  This file is part of "Paroxysm".
 *
 *  "Paroxysm" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Paroxysm" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with "Paroxysm".  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GameModule.h"

GameModule::GameModule(const char* inMapFile)
{
    string inFile = "assets/maps/";
    inFile += inMapFile;
    ifstream input(inFile.c_str());

    if (!input.fail())
    {
        input >> mTerrain;
        mTerrainSize.x = mTerrain.getMatrix().cols();
        mTerrainSize.y = mTerrain.getMatrix().rows();

        input.close();
    }
    else
    {
        cerr << "failed to open file." << endl;
        exit(3);
    }
}

GameModule::~GameModule()
{
}

bool GameModule::onLoad()
{
    mTrackball[0] = 22.0f;
    mTrackball[2] = 20.0f;
    mPanning[0] = static_cast<GLfloat>(mTerrainSize.x) / -2.0f;
    mPanning[2] = static_cast<GLfloat>(mTerrainSize.y) / -2.0f;

    mDisplay.x = SDL_GetVideoSurface()->w;
    mCenter.x = mDisplay.x / 2;
    mDisplay.y = SDL_GetVideoSurface()->h;
    mCenter.y = mDisplay.y / 2;

    mHUD.setDisplay(mDisplay);

    mLuaConsole = new TextBox;
    mLuaConsole->setLocation(0.0f, -6.0f);
    mLuaConsole->setSize(12.0f, 1.0f);
    mLuaConsole->setTextColor(Vector3D<float>(0.0f, 0.5f, 1.0f));
    mLuaConsole->hideOnEnter(true);
    mLuaConsole->setVisible(false);
    mHUD.addWidget(mLuaConsole);
    //mLua.addFunction("cameraPan", luaCameraPan);

    return true;
}


void GameModule::onInit()
{
    mRunning = true;
    mDead = true;
    mNextModule = NULL;
    mMouseMode = MM_DEFAULT;

    SoundEngine::loadBackgroundMusic("portal_still_alive.wav");

    glViewport(0, 0, (GLsizei)mDisplay.x, (GLsizei)mDisplay.y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FIELD_OF_VIEW, (GLdouble)mDisplay.x / (GLdouble)mDisplay.y,
        NEAR_CP, FAR_CP);

    glMatrixMode(GL_MODELVIEW);

    //glGetDoublev(GL_PROJECTION_MATRIX, mProjection.array());
    //mProjection.transpose();

    //glGetIntegerv(GL_VIEWPORT, mViewport.array());

    //cerr << "\nProjection Matrix: \n" << mProjection << endl;
    //glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    mLight.diffuse.set(1.0f);
    mLight.direction[1] = -1.0f;
    mLight.position[1] = 10.0f;
    mLight.position[3] = 0.0f; // distant light source

    glEnable(GL_LIGHT0);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, mLight.ambient.array());
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}


void GameModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLightfv(GL_LIGHT0, GL_AMBIENT, mLight.ambient.array());
    glLightfv(GL_LIGHT0, GL_DIFFUSE, mLight.diffuse.array());
    glLightfv(GL_LIGHT0, GL_SPECULAR, mLight.specular.array());
    glLightfv(GL_LIGHT0, GL_POSITION, mLight.position.array());
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -mTrackball[2]);
    glRotatef(mTrackball[0], 1.0f, 0.0f, 0.0f);
    glRotatef(mTrackball[1], 0.0f, 1.0f, 0.0f);

    Vector3D<float> pan = mTank.getPosition();
    glTranslatef(-pan[0], -pan[1], -pan[2]);


    mTerrain.display();

    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    {
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 10.0f, 0.0f);
    }
    glEnd();
    glPopAttrib();

    mTank.display();

    glPopMatrix();

    mHUD.display();

}

void GameModule::onFrame()
{
}

void GameModule::onCleanup()
{
}

void GameModule::onRButtonDown(int inX, int inY)
{
    if (mMouseMode == MM_DEFAULT)
    {
        SDL_ShowCursor(SDL_DISABLE);
        mOldMouse.x = inX;
        mOldMouse.y = inY;
        SDL_WarpMouse(mCenter.x, mCenter.y);
        mMouseMode = MM_ROTATING;
    }
}

void GameModule::onRButtonUp(int inX, int inY)
{
    if (mMouseMode == MM_ROTATING)
    {
        mMouseMode = MM_DEFAULT;
        SDL_WarpMouse(mOldMouse.x, mOldMouse.y);
        SDL_ShowCursor(SDL_ENABLE);
        //mSceneChanged = true;
    }
}

void GameModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
    bool inLeft, bool inRight, bool inMiddle)
{
    bool lockMouse = false;

    switch (mMouseMode)
    {
        case MM_ROTATING:
        case MM_PANNING:
        {
            if (abs(inX - mCenter.x) > 100 || abs(inY - mCenter.y) > 100)
            {
                return;
            }
            break;
        }
    }

    switch (mMouseMode)
    {
        case MM_ROTATING:
        {
            lockMouse = true;

            mTrackball[1] += static_cast<GLfloat>(inX - mCenter.x) * TRACKBALL_STEP;
            if (mTrackball[1] < -180.0f)
                mTrackball[1] += 360.0f;
            else if (mTrackball[1] > 180.0f)
                mTrackball[1] -= 360.0f;

            mTrackball[0] += static_cast<GLfloat>(inY - mCenter.y) * TRACKBALL_STEP;
            if (mTrackball[0] < -180.0f)
                mTrackball[0] += 360.0f;
            else if (mTrackball[0] > 180.0f)
                mTrackball[0] -= 360.0f;
            break;
        }
        case MM_PANNING:
        {
            lockMouse = true;
            GLfloat dx = static_cast<GLfloat>(inX - mCenter.x) * PANNING_STEP;
            GLfloat dy = static_cast<GLfloat>(inY - mCenter.y) * PANNING_STEP;

            GLfloat theta = TO_RADIANS(mTrackball[1]);
            GLfloat dxp = cos(theta) * dx;
            GLfloat dyp = sin(theta) * dx;
            dxp -= sin(theta) * dy;
            dyp += cos(theta) * dy;

            mPanning[0] += dxp;
            mPanning[2] += dyp;
            break;
        }
        case MM_DEFAULT:
        {
            mHUD.setStates(inX, inY, false);
            break;
        }
    }

    if (lockMouse && (inX != mCenter.x || inY != mCenter.y))
    {
        SDL_WarpMouse(mCenter.x, mCenter.y);
    }
}

void GameModule::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    switch (inSym)
    {
        case SDLK_RETURN:
        {
            mLuaConsole->setText("");
            mLuaConsole->setVisible(true);
            mHUD.setFocus(mLuaConsole);
            break;
        }

        case SDLK_ESCAPE:
        {
            mRunning = false;
            break;
        }

        default:
        {
        }
    }
}
