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

#include "MapEditorModule.h"
#include "DisplayEngine.h"
#include "Config.h"
#include "EditVertexAction.h"

bool MapEditorModule::onInit()
{
    mRunning = true;
    mMouseMode = MM_DEFAULT;
    mEditMode = EM_TERRAIN;
    mCurrentAction = NULL;

    mProjection = Matrix<GLdouble>(4);
    mModelView = Matrix<GLdouble>(4);

    mSceneChanged = true;

    mTerrainSize.x = Config::get<int>("terrain cols", 8);
    mTerrainSize.y = Config::get<int>("terrain rows", 8);
    string terrainFilename = Config::get<string>("terrain file", "_");
    ifstream terrainFile(terrainFilename.c_str());
    if (terrainFile.fail())
    {
        cerr << "no terrain file: " << terrainFilename << endl;
        mTerrainGrid.create(mTerrainSize.y, mTerrainSize.x);
    }
    else
    {
        //hello
        terrainFile >> mTerrainGrid;
        mTerrainSize.x = mTerrainGrid.getMatrix().cols();
        mTerrainSize.y = mTerrainGrid.getMatrix().rows();
        terrainFile.close();
    }

    mTrackball[0] = 22.0f;
    mTrackball[2] = 20.0f;
    mPanning[0] = static_cast<GLfloat>(mTerrainSize.x) / -2.0f;
    mPanning[2] = static_cast<GLfloat>(mTerrainSize.y) / -2.0f;

    mDisplay.x = SDL_GetVideoSurface()->w;
    mCenter.x = mDisplay.x / 2;
    mDisplay.y = SDL_GetVideoSurface()->h;
    mCenter.y = mDisplay.y / 2;

    glViewport(0, 0, (GLsizei)mDisplay.x, (GLsizei)mDisplay.y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FIELD_OF_VIEW, (GLdouble)mDisplay.x / (GLdouble)mDisplay.y,
        NEAR_CP, FAR_CP);

    glMatrixMode(GL_MODELVIEW);

    glGetDoublev(GL_PROJECTION_MATRIX, mProjection.array());
    //mProjection.transpose();

    glGetIntegerv(GL_VIEWPORT, mViewport.array());

    cerr << "\nProjection Matrix: \n" << mProjection << endl;
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

    glGenTextures(3, mTexturesHUD);
    DisplayEngine::loadTexture("assets/images/hud_terrain_out.png",
        mTexturesHUD[0]);
    DisplayEngine::loadTexture("assets/images/hud_terrain_on.png",
        mTexturesHUD[1]);
    DisplayEngine::loadTexture("assets/images/hud_terrain_press.png",
        mTexturesHUD[2]);

    return true;
}

void MapEditorModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

/*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FIELD_OF_VIEW, (GLdouble)mDisplay.x / (GLdouble)mDisplay.y,
        NEAR_CP, FAR_CP);

    glMatrixMode(GL_MODELVIEW);
*/
    // purposefully left outside the camera control to illustrate the changing
    // light patterns
    glLightfv(GL_LIGHT0, GL_AMBIENT, mLight.ambient.array());
    glLightfv(GL_LIGHT0, GL_DIFFUSE, mLight.diffuse.array());
    glLightfv(GL_LIGHT0, GL_SPECULAR, mLight.specular.array());
    glLightfv(GL_LIGHT0, GL_POSITION, mLight.position.array());
    glPushMatrix();

    glTranslatef(0.0f, 0.0f, -mTrackball[2]);
    glRotatef(mTrackball[0], 1.0f, 0.0f, 0.0f);
    glRotatef(mTrackball[1], 0.0f, 1.0f, 0.0f);

    glTranslatef(mPanning[0], mPanning[1], mPanning[2]);

    if (mSceneChanged)
    {
        glGetDoublev(GL_MODELVIEW_MATRIX, mModelView.array());
        //mModelView.transpose();
        //mTransform = mModelView * mProjection;
        //cerr << "\nmTransform: \n" << mTransform << endl;
        mSceneChanged = false;
    }

    mTerrainGrid.display();

    // reference Y-axis
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    {
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 10.0f, 0.0f);
    }
    glEnd();
    glPopAttrib();

    if (mEditMode == EM_TERRAIN)
    {
        mSphere.display();
    }

    glPopMatrix();

    /// HUD display (orthographic projection)
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    double ratio = double(mDisplay.x) / double(mDisplay.y);
    glOrtho(-HUD_RANGE * ratio, HUD_RANGE * ratio, -HUD_RANGE, HUD_RANGE,
        -HUD_RANGE, HUD_RANGE);

    glMatrixMode(GL_MODELVIEW);

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, mTexturesHUD[0]);
    glBegin(GL_QUADS);
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2i(0, 1);
        glVertex2f(-6.0f, -6.0f);
        glTexCoord2i(1, 1);
        glVertex2f(-2.0f, -6.0f);
        glTexCoord2i(1, 0);
        glVertex2f(-2.0f, -5.0f);
        glTexCoord2i(0, 0);
        glVertex2f(-6.0f, -5.0f);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glPopAttrib();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void MapEditorModule::onFrame()
{
    /// There really is no game logic running in the map editor. Virtually
    /// everything is event-driven (mouse clicks, keystrokes, etc.).
}

void MapEditorModule::onCleanup()
{
    while (!mUndo.empty())
    {
        delete mUndo.back();
        mUndo.pop_back();
    }

    while (!mRedo.empty())
    {
        delete mRedo.back();
        mRedo.pop_back();
    }

    glDeleteTextures(3, mTexturesHUD);
}


Vector3D<float> MapEditorModule::selectVertex(int inX, int inY)
{
    Vector3D<float> closestMatch;
    Vector3D<float> currentVertex;

    GLdouble tempX = 0;
    GLdouble tempY = 0;
    GLdouble tempZ = 0;

    GLfloat depthZ = 0;

    //we have to invert the y axis because of opengl's viewport
    int newY = -(inY - Config::get("display height", 600));

    //read the depth buffer to determine the z coordinate at the clicked
    //x,y coordinates
    glReadPixels(inX, newY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthZ);

    //now let the glu library do the math for us :)
    if (gluUnProject((GLdouble)inX, (GLdouble)newY, depthZ, mModelView.array(), mProjection.array(), mViewport.array(), &tempX, &tempY, &tempZ) == GL_FALSE)
    {
        cerr << "gluUnProject failed." << endl;
    }

    Vector3D<float> clickPoint(tempX, tempY, tempZ);


    int numRows = mTerrainGrid.getMatrix().rows();
    int numCols = mTerrainGrid.getMatrix().cols();

    for (int i = 0; i < numRows; ++i)
    {
        for (int j = 0; j < numCols; ++j)
        {
            currentVertex = mTerrainGrid.getVertex(i, j);
            if ((currentVertex - clickPoint).length() <= (closestMatch - clickPoint).length())
            {
                closestMatch = currentVertex;
            }
        }
    }
    return closestMatch;
}


void MapEditorModule::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    switch (inSym)
    {
        case SDLK_z:
        {
            if (inMod & (KMOD_LCTRL | KMOD_RCTRL))
            {
                if (inMod & (KMOD_LSHIFT | KMOD_RSHIFT))
                    redoAction();
                else
                    undoAction();
            }
            break;
        }

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
            mPanning[0] = static_cast<GLfloat>(mTerrainSize.x) / -2.0f;
            mPanning[2] = static_cast<GLfloat>(mTerrainSize.y) / -2.0f;
            mSceneChanged = true;
            break;
        }

        case SDLK_TAB:
        {
            if (mMouseMode != MM_DEFAULT) break;

            mEditMode = !mEditMode;

            if (mEditMode == EM_TERRAIN)
                cerr << "terrain mode" << endl;
            else if (mEditMode == EM_TILE)
                cerr << "tile mode" << endl;

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

    mSceneChanged = true;
}

void MapEditorModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
    bool inLeft, bool inRight, bool inMiddle)
{
    switch (mMouseMode)
    {
        case MM_ROTATING:
        {
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
        case MM_EDITING_VERTEX:
        {
            GLfloat dy = -(inY - mCenter.y);
            dy /= abs(dy);

            if (dy != 1 && dy != -1)
            {
                dy = 0;
            }
            //cerr << "vertex before: " << mClickedVertex[1];
            mClickedVertex[1] += (dy * VERTEX_STEP);
            //cerr << " dy: " << dy << " vertex: " << mClickedVertex[1] << endl;

            EditVertexAction* eva =
                dynamic_cast<EditVertexAction*>(mCurrentAction);
            eva->setAfter(mClickedVertex[1]);
            eva->execute();

            mSphere.moveSphere(mClickedVertex[0], mClickedVertex[1],
                mClickedVertex[2]);
            break;
        }
        case MM_DEFAULT:
        {
            Vector3D<float> hoverVertex = selectVertex(inX, inY);
            mSphere.moveSphere(hoverVertex[0], hoverVertex[1], hoverVertex[2]);
            break;
        }
    }

    if (mMouseMode != MM_DEFAULT && (inX != mCenter.x || inY != mCenter.y))
    {
        SDL_WarpMouse(mCenter.x, mCenter.y);
    }
}

void MapEditorModule::onLButtonDown(int inX, int inY)
{
    if (mMouseMode != MM_DEFAULT) return;


    Uint8* keyState = SDL_GetKeyState(NULL);

    if (keyState[SDLK_LSHIFT] || keyState[SDLK_RSHIFT])
    {
        SDL_ShowCursor(SDL_DISABLE);
        mOldMouse.x = inX;
        mOldMouse.y = inY;
        SDL_WarpMouse(mCenter.x, mCenter.y);
        mMouseMode = MM_PANNING;
    }
    else
    {


/*
        //this math works, and is proven to produce exactly the same results
        //as opengl's math does
        cerr << "modelMatrix: \n" << mModelView.transposed() << endl;

        Matrix<float> transform(4);
        Matrix<float> translateZ(4);
        Matrix<float> translateRest(4);
        Matrix<float> rotateX(4);
        Matrix<float> rotateY(4);

        translateZ(2, 3) = -mTrackball[2];

        translateRest(0,3) = mPanning[0];
        translateRest(1,3) = mPanning[1];
        translateRest(2,3) = mPanning[2];

        float cosRotation = cos(TO_RADIANS(mTrackball[0]));
        float sinRotation = sin(TO_RADIANS(mTrackball[0]));

        rotateX(1,1) = cosRotation;
        rotateX(2,2) = cosRotation;
        rotateX(1,2) = -sinRotation;
        rotateX(2,1) = sinRotation;

        cosRotation = cos(TO_RADIANS(mTrackball[1]));
        sinRotation = sin(TO_RADIANS(mTrackball[1]));

        rotateY(0,0) = cosRotation;
        rotateY(0,2) = sinRotation;
        rotateY(2,0) = -sinRotation;
        rotateY(2,2) = cosRotation;

        transform = translateZ * rotateX * rotateY * translateRest;

        cerr << "\nTransformation matrix: \n" << transform << endl;
*/


        mClickedVertex = selectVertex(inX, inY);
        EditVertexAction* eva = new EditVertexAction(&mTerrainGrid);
        eva->setBefore((int)mClickedVertex[2], (int)mClickedVertex[0],
            mClickedVertex[1]);
        eva->setAfter(mClickedVertex[1]);
        mCurrentAction = eva;

        //mSphere.moveSphere(mClickedVertex[0], mClickedVertex[1], mClickedVertex[2]);

        SDL_ShowCursor(SDL_DISABLE);
        mOldMouse.x = inX;
        mOldMouse.y = inY;
        SDL_WarpMouse(mCenter.x, mCenter.y);
        mMouseMode = MM_EDITING_VERTEX;

    }
}

void MapEditorModule::onLButtonUp(int inX, int inY)
{
    switch (mMouseMode)
    {
        case MM_PANNING:
        {
            mMouseMode = MM_DEFAULT;
            SDL_WarpMouse(mOldMouse.x, mOldMouse.y);
            SDL_ShowCursor(SDL_ENABLE);
            mSceneChanged = true;
            break;
        }
        case MM_EDITING_VERTEX:
        {
            doAction();
            mCurrentAction = NULL;
            mMouseMode = MM_DEFAULT;
            SDL_WarpMouse(mOldMouse.x, mOldMouse.y);
            SDL_ShowCursor(SDL_ENABLE);
            break;
        }
    }
}

void MapEditorModule::onRButtonDown(int inX, int inY)
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

void MapEditorModule::onRButtonUp(int inX, int inY)
{
    if (mMouseMode == MM_ROTATING)
    {
        mMouseMode = MM_DEFAULT;
        SDL_WarpMouse(mOldMouse.x, mOldMouse.y);
        SDL_ShowCursor(SDL_ENABLE);
        mSceneChanged = true;
    }
}

void MapEditorModule::doAction()
{
    while (!mRedo.empty())
    {
        delete mRedo.back();
        mRedo.pop_back();
    }

    mUndo.push_back(mCurrentAction);
    mCurrentAction->execute();
    mCurrentAction = NULL;
}

void MapEditorModule::redoAction()
{
    if (mRedo.empty()) return;

    MapEditorAction* action = mRedo.back();
    action->execute();

    mUndo.push_back(action);
    mRedo.pop_back();
}

void MapEditorModule::undoAction()
{
    if (mUndo.empty()) return;

    MapEditorAction* action = mUndo.back();
    action->undo();

    mRedo.push_back(action);
    mUndo.pop_back();
}
