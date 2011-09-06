#include "MapEditorModule.h"
#include "TestActorNode.h"
#include <CGE/Exception.h>

#include <fstream>
using namespace std;

MapEditorModule::MapEditorModule() : mLeftClickDown(false), mXPan(0.0f),
    mYPan(0.0f)
{
    mModel = mManager.load("bradley.c3m");
    mSphere = mManager.load("cube_texture.c3m");
    mLeftClickDown = false;
}

MapEditorModule::~MapEditorModule()
{
}

void MapEditorModule::onLoad(CGE::PropertyList& inList)
{
    ActorNode* an = new ActorNode(mGrid);
    mViewNode.addChildNode(*an);
    mBin.addNode(*an);
    mActors.push_back(an);

    an = new TestActorNode(*mModel);
    mViewNode.addChildNode(*an);
    mBin.addNode(*an);
    mActors.push_back(an);

    mSphereNode = new SimpleMatrixNode(*mSphere);
    mViewNode.addChildNode(*mSphereNode);
    mBin.addNode(*mSphereNode);

    ifstream fin("assets/maps/Shared_Map.pmf");
    if (!fin)
        throw CGE::Exception("", "no map to load");
    int size = 10;
    //fin >> mGrid;
    mGrid.create(size, size);
    mGrid.buildVBO();
    fin.close();
}

void MapEditorModule::onUnload()
{
}

void MapEditorModule::onOpen()
{
    mMouseState = NONE;

    mViewNode.setAngle(-45.0f);
    mViewNode.setDistance(8.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
}

void MapEditorModule::onClose()
{
    glDisable(GL_DEPTH_TEST);
}

void MapEditorModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //mSphere->display();
    mBin.displayAll();
}

void MapEditorModule::onPulse()
{
    for (list<ActorNode*>::iterator i = mActors.begin();
        i != mActors.end(); ++i)
    {
        ActorNode& an = *(*i);
        an.update();
    }

    mSphereNode->matrix().loadIdentity();
    mSphereNode->matrix().translate(mSpherePosition[0], mSpherePosition[1],
        mSpherePosition[2]);
    mSphereNode->matrix().scale(0.1f);

    mViewNode.smartPan(mXPan, mYPan);

    mViewNode.update();
    mViewNode.updateAllMatrices();
}


vec4f MapEditorModule::selectVertex(int inX, int inY)
{
    vec4f currentVertex;

    vec4f tempPoint;

    GLfloat depthZ = 0;

    //we have to invert the y axis because of opengl's viewport
    int newY = SDL_GetVideoSurface()->h - inY;

    //read the depth buffer to determine the z coordinate at the input
    //x,y coordinates
    glReadPixels(inX, newY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthZ);

    //now let the glu library do the math for us :)
//    if (gluUnProject((GLdouble)inX, (GLdouble)newY, depthZ, mModelView, mProjection, mViewport, &tempX, &tempY, &tempZ) == GL_FALSE)
//    {
//        cerr << "gluUnProject failed." << endl;
//    }

    if (!CGE::unproject((GLfloat)inX, (GLfloat)newY, depthZ, mViewNode.compositeMatrix(), mViewport, tempPoint))
    {
        cerr << "unproject failed." << endl;
    }

    int numRows = mGrid.getMatrix().rows();
    int numCols = mGrid.getMatrix().cols();

    if (tempPoint[1] >= numRows || tempPoint[1] < 0 || tempPoint[0] >= numCols || tempPoint[0] < 0)
    {
        currentVertex = mSelectPosition;
    }
    else
    {
        int closestRow = int(tempPoint[1] + 0.5);
        if (closestRow >= numRows)
        {
            closestRow = numRows - 1;
        }
        int closestColumn = int(tempPoint[0] + 0.5);
        if (closestColumn >= numCols)
        {
            closestColumn = numCols - 1;
        }

        currentVertex = mGrid.getVertex(closestRow, closestColumn);
    }

    return currentVertex;
}

void MapEditorModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
    bool inLeft, bool inRight, bool inMiddle)
{
    if (mLeftClickDown)
    {
        mViewNode.changeRotation((inX - mXStart)/2);
        mViewNode.changeAngle((inY - mYStart)/2);
        mXStart = inX;
        mYStart = inY;
    }

    switch(mMouseState)
    {
        case NONE:
        {
            vec4f hoverVertex = selectVertex(inX, inY);

            cerr << "hoververtex: " << hoverVertex[0] << ", " << hoverVertex[1] << ", " << hoverVertex[2] << endl;
            if (hoverVertex[0] >= 0.0f
                && hoverVertex[0] <= float(mGrid.getMatrix().lastCol())
                && hoverVertex[1] >= 0.0f
                && hoverVertex[1] <= float(mGrid.getMatrix().lastRow()))
            {
                mSelectPosition = hoverVertex;

                hoverVertex[0] -= 0.05;
                hoverVertex[1] -= 0.05;
                hoverVertex[2] -= 0.05;
                mSpherePosition = hoverVertex;
            }
            break;
        }

        case PANNING:
        {
            mViewNode.smartPan(-inRelX * 0.1f, inRelY * 0.1f);
            break;
        }

        case ROTATING:
        {
            mViewNode.changeAngle(inRelY);
            mViewNode.changeRotation(inRelX);
            break;
        }

        case EDITING_TERRAIN:
        {
            GLfloat dy = -(inRelY);
            dy /= abs(dy);

            if (dy != 1 && dy != -1)
            {
                dy = 0;
            }
            //cerr << "vertex before: " << mClickedVertex[1];
            mClickedVertex[2] += (dy * VERTEX_STEP);

            mGrid.set((int)mClickedVertex[1], (int)mClickedVertex[0],
                mClickedVertex[2], false);

            mSpherePosition = mClickedVertex;
            break;
        }

        default: {}
    }
}

void MapEditorModule::onLButtonDown(int inX, int inY)
{
    Uint8* keys = SDL_GetKeyState(NULL);

    if (keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT])
    {
        mMouseState = PANNING;
    }
    else
    {
        mClickedVertex = selectVertex(inX, inY);
        mMouseState = EDITING_TERRAIN;
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

void MapEditorModule::onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
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
            mYPan = 0.5f;
            break;
        }

        case SDLK_a:
        {
            mXPan = -0.5f;
            break;
        }

        case SDLK_s:
        {
            mYPan = -0.5f;
            break;
        }

        case SDLK_d:
        {
            mXPan = 0.5f;
            break;
        }

        default: {}
    }
}

void MapEditorModule::onKeyUp(SDLKey inSym, SDLMod inMod, Uint16 inUnicode)
{
    switch (inSym)
    {
        case SDLK_w:
        {
            if (mYPan > 0.0f)
                mYPan = 0.0f;
            break;
        }

        case SDLK_s:
        {
            if (mYPan < 0.0f)
                mYPan = 0.0f;
            break;
        }

        case SDLK_a:
        {
            if (mXPan < 0.0f)
                mXPan = 0.0f;
            break;
        }

        case SDLK_d:
        {
            if (mXPan > 0.0f)
                mXPan = 0.0f;
            break;
        }

        default: {}
    }
}

void MapEditorModule::onMouseWheel(bool inUp)
{
    if (inUp)
    {
        mViewNode.changeDistance(-0.5f);
    }
    else
    {
        mViewNode.changeDistance(0.5f);
    }
}

void MapEditorModule::onResize(int inWidth, int inHeight)
{
    GLfloat ratio = static_cast<GLfloat>(inWidth)
        / static_cast<GLfloat>(inHeight);

    mUI.onResize(inWidth, inHeight);

    CGE::Matrix4x4<GLfloat> projection;
    projection.perspective(30.0f, ratio, 1.0f, 1000.0f);
    mViewNode.setProjection(projection);
    glViewport(0, 0, inWidth, inHeight);
    glGetIntegerv(GL_VIEWPORT, mViewport);
}
