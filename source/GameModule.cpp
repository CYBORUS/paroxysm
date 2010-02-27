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
    //dtor
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

    return true;
}


void GameModule::onInit()
{
    mRunning = true;
    mDead = true;
    mNextModule = NULL;

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

    glTranslatef(mPanning[0], mPanning[1], mPanning[2]);


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

    glPopMatrix();

    mHUD.display();

}

void GameModule::onFrame()
{
}

void GameModule::onCleanup()
{
}

