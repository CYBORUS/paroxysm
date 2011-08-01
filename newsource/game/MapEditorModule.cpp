#include "MapEditorModule.h"
#include <CGE/Exception.h>

#include <fstream>
using namespace std;

MapEditorModule::MapEditorModule()
{
    mModel = mManager.load("bradley.c3m");
    mLeftClickDown = false;
    mKeyDown = false;
}

MapEditorModule::~MapEditorModule()
{
}

void MapEditorModule::onLoad(CGE::PropertyList& inList)
{
    //mTerrainTexture.loadImage("assets/images/green.png");
    //CGE::Image img;
    //CGE::Font f("assets/misc/DejaVuSans.ttf", 12);
    //img.loadText(f, "TEST");

    //int n = 1;
    //while (n < img.width()) n <<= 1;

    //CGE::Image img2(n, n);
    //img.blitOnto(img2);

    //mTerrainTexture.loadImage(img2);


    ifstream fin("assets/maps/Shared_Map.pmf");
    if (!fin)
        throw CGE::Exception("", "no map to load");
    int size = 500;
    //fin >> mGrid;
    mGrid.create(size, size);
    mGrid.buildVBO();
    fin.close();
    mCamera.setPosition(size / 2, size / 2, 0);
}

void MapEditorModule::onUnload()
{
}

void MapEditorModule::onOpen()
{
    mRunning = true;
    mCamera.setAngle(-45.0f);
    mCamera.setDistance(8.0f);
    //mCamera.setPosition(100.0f, 100.0f, 0.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);

    //mTerrainTexture.bind();
}

void MapEditorModule::onClose()
{
    glDisable(GL_DEPTH_TEST);
}

void MapEditorModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mModelView = mCamera.matrix();
//    mModelView.loadIdentity();
//    mModelView.translate(0.0f, 0.0f, -20.0f);
//    mModelView.rotateX(-45.0f);
//    mModelView.rotateZ(mSpin);
//    mModelView.translate(-100.0f, -100.0f, 0.0f);

    CGE::Matrix4x4<GLfloat> mvp(mProjection, mModelView);
    mProgram.setMatrix(mvp);
    mGrid.display();
    mModel->display();
}

void MapEditorModule::onPulse()
{
    mCamera.update();

    if (mKeyDown)
        mCamera.smartPan(mXPan, mYPan);
}


vec3f MapEditorModule::selectVertex(int inX, int inY)
{
    vec3f currentVertex;

    GLdouble tempX = 0;
    GLdouble tempY = 0;
    GLdouble tempZ = 0;

    GLfloat depthZ = 0;

    //we have to invert the y axis because of opengl's viewport
    int newY = SDL_GetVideoSurface()->h - inY;

    //read the depth buffer to determine the z coordinate at the input
    //x,y coordinates
    glReadPixels(inX, newY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthZ);

    //now let the glu library do the math for us :)
    if (gluUnProject((GLdouble)inX, (GLdouble)newY, depthZ, mModelView, mProjection, mViewport, &tempX, &tempY, &tempZ) == GL_FALSE)
    {
        cerr << "gluUnProject failed." << endl;
    }

    int numRows = mTerrainGrid.getMatrix().rows();
    int numCols = mTerrainGrid.getMatrix().cols();

    if (tempZ >= numRows || tempZ < 0 || tempX >= numCols || tempX < 0)
    {
        currentVertex = mSphere.getTranslation();
    }
    else
    {
        int closestRow = int(tempZ + 0.5);
        if (closestRow >= numRows)
        {
            closestRow = numRows - 1;
        }
        int closestColumn = int(tempX + 0.5);
        if (closestColumn >= numCols)
        {
            closestColumn = numCols - 1;
        }

        currentVertex = mTerrainGrid.getVertex(closestRow, closestColumn);
    }

    //return mTerrainGrid.getVertex(closestRow, closestColumn);
    return currentVertex;
}

void MapEditorModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
                                  bool inLeft, bool inRight, bool inMiddle)
{
    if (mLeftClickDown)
    {
        mCamera.changeRotation((inX - mXStart)/2);
        mCamera.changeAngle((inY - mYStart)/2);
        mXStart = inX;
        mYStart = inY;
    }

    switch(mMouseState)
    {
        case PANNING:
        {
            mCamera.smartPan(-inRelX * 0.1f, inRelY * 0.1f);
            break;
        }

        case ROTATING:
        {
            mCamera.changeAngle(inRelY);
            mCamera.changeRotation(inRelX);
            break;
        }
    }
}

void MapEditorModule::onLButtonDown(int inX, int inY)
{
    Uint8 *keys = SDL_GetKeyState(NULL);

    if (keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT])
    {
        mMouseState = PANNING;
    }

}

void MapEditorModule::onLButtonUp(int inX, int inY)
{
    mMouseState = NONE;
}

void MapEditorModule::onRButtonDown(int inX, int inY)
{
    mMouseState = ROTATING;
}

void MapEditorModule::onRButtonUp(int inX, int inY)
{
    mMouseState = NONE;
}

void MapEditorModule::onKeyDown(SDLKey inSym, SDLMod inMod,
                                Uint16 inUnicode)
{
    switch (inSym)
    {
        case SDLK_ESCAPE:
        {
            mRunning = false;
            break;
        }
        case SDLK_w:
        {
            mXPan = 0.0f;
            mYPan = 0.5f;
            mKeyDown = true;
            break;
        }
        case SDLK_a:
        {
            mXPan = -0.5f;
            mYPan = 0.0f;
            mKeyDown = true;
            break;
        }
        case SDLK_s:
        {
            mXPan = 0.0f;
            mYPan = -0.5f;
            mKeyDown = true;
            break;
        }
        case SDLK_d:
        {
            mXPan = 0.5f;
            mYPan = 0.0f;
            mKeyDown = true;
            break;
        }
    }
}

void MapEditorModule::onKeyUp(SDLKey inSym, SDLMod inMod,
                              Uint16 inUnicode)
{
    mKeyDown = false;
}


void MapEditorModule::onMouseWheel(bool inUp)
{
    if (inUp)
    {
        mCamera.changeDistance(-0.5f);
    }
    else
    {
        mCamera.changeDistance(0.5f);
    }
}

void MapEditorModule::onResize(int inWidth, int inHeight)
{
    GLfloat ratio = static_cast<GLfloat>(inWidth)
                    / static_cast<GLfloat>(inHeight);
    mProjection.loadIdentity();
    mProjection.perspective(30.0f, ratio, 1.0f, 1000.0f);
    glViewport(0, 0, inWidth, inHeight);
}
