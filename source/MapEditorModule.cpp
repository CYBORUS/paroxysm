#include "MapEditorModule.h"
#include "DisplayEngine.h"
#include "Config.h"

bool MapEditorModule::onInit()
{
    mRunning = true;
    mMouseMode = MM_DEFAULT;

    mTerrainSize = Config::get<int>("terrain size", 10);
    mTerrainGrid.create(mTerrainSize, mTerrainSize);

    mTrackball[0] = 22.0f;
    mTrackball[2] = 20.0f;
    mPanning[0] = static_cast<GLfloat>(mTerrainSize) / -2.0f;
    mPanning[2] = static_cast<GLfloat>(mTerrainSize) / -2.0f;

    int w = SDL_GetVideoSurface()->w;
    mCenterX = w / 2;
    int h = SDL_GetVideoSurface()->h;
    mCenterY = h / 2;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    gluPerspective(FIELD_OF_VIEW, (GLdouble)w / (GLdouble)h, NEAR_CP, FAR_CP);


    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
/*
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, mVertices);

    glNewList(mList, GL_COMPILE);
    {

        glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_INT,
            mIndices);
    }
    glEndList();

        glDisableClientState(GL_VERTEX_ARRAY);
*/

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



    return true;
}

void MapEditorModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    //glCallList(mList);
    mTerrainGrid.display();

    glBegin(GL_LINES);
    {
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 10.0f, 0.0f);
    }
    glEnd();

    mSphere.display();

    glPopMatrix();
}

void MapEditorModule::onFrame()
{
    /// There really is no game logic running in the map editor. Virtually
    /// everything is event-driven (mouse clicks, keystrokes, etc.).
}

void MapEditorModule::onCleanup()
{
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
            mPanning[0] = static_cast<GLfloat>(mTerrainSize) / -2.0f;
            mPanning[2] = static_cast<GLfloat>(mTerrainSize) / -2.0f;
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


    Uint8* keyState = SDL_GetKeyState(NULL);

    if (keyState[SDLK_LSHIFT])
    {
        cerr << "left shift down" << endl;

        GLfloat modelMatrix[16];

        glGetFloatv(GL_MODELVIEW_MATRIX, modelMatrix);
/*
        cerr << "modelMatrix: \n";

        // @Chad -- This is showing an identity matrix because before any
        // transformations are applied, we glPushMatrix, do a bunch of stuff,
        // and then glPopMatrix. Since glGetFloatv is called after the
        // glPopMatrix, you see only the identity matrix since the matrix you
        // wanted was popped off.
        cerr << modelMatrix[0] << " " << modelMatrix[4] << " " << modelMatrix[8] << " " << modelMatrix[12] << endl;
        cerr << modelMatrix[1] << " " << modelMatrix[5] << " " << modelMatrix[9] << " " << modelMatrix[13] << endl;
        cerr << modelMatrix[2] << " " << modelMatrix[6] << " " << modelMatrix[10] << " " << modelMatrix[14] << endl;
        cerr << modelMatrix[3] << " " << modelMatrix[7] << " " << modelMatrix[11] << " " << modelMatrix[15] << endl;
*/
        Vector3D<int> sceneTranslation;
        Vector3D<float> sceneRotation;

        Matrix<float> transform(4);

        transform(0, 3) = mPanning[0];
        transform(1, 3) = mPanning[1];
        transform(2, 3) = mPanning[2] - mTrackball[2];

        float cosRotation = cos(mTrackball[0]);
        float sinRotation = sin(mTrackball[0]);

        transform(1,1) += cosRotation;
        transform(2,2) += cosRotation;
        transform(1,2) += sinRotation;
        transform(2,1) += -sinRotation;

        cosRotation = cos(mTrackball[1]);
        sinRotation = sin(mTrackball[1]);

        transform(0,0) += cosRotation;
        transform(0,2) += -sinRotation;
        transform(2,0) += sinRotation;
        transform(2,2) += cosRotation;

        cerr << "\nTransformation matrix: \n" << transform << endl;


        //cerr << "x rotation: " << acos(modelMatrix[
    }
    else
    {
        SDL_ShowCursor(SDL_DISABLE);
        mOldMouseX = inX;
        mOldMouseY = inY;
        SDL_WarpMouse(mCenterX, mCenterY);
        mMouseMode = MM_PANNING;
    }
}

void MapEditorModule::onLButtonUp(int inX, int inY)
{
    if (mMouseMode == MM_PANNING)
    {
        mMouseMode = MM_DEFAULT;
        SDL_WarpMouse(mOldMouseX, mOldMouseY);
        SDL_ShowCursor(SDL_ENABLE);
    }
}

void MapEditorModule::onRButtonDown(int inX, int inY)
{
    if (mMouseMode == MM_DEFAULT)
    {
        SDL_ShowCursor(SDL_DISABLE);
        mOldMouseX = inX;
        mOldMouseY = inY;
        SDL_WarpMouse(mCenterX, mCenterY);
        mMouseMode = MM_ROTATING;
    }
}

void MapEditorModule::onRButtonUp(int inX, int inY)
{
    if (mMouseMode == MM_ROTATING)
    {
        mMouseMode = MM_DEFAULT;
        SDL_WarpMouse(mOldMouseX, mOldMouseY);
        SDL_ShowCursor(SDL_ENABLE);
    }
}
