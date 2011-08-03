#include "MapEditorModule.h"
#include <CGE/Exception.h>

#include <fstream>
using namespace std;

MapEditorModule::MapEditorModule() : mCube(2)
{
    mModel = mManager.load("bradley.c3m");
    mSphere = mManager.load("cube_texture.c3m");
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
    int size = 200;
    //fin >> mGrid;
    mGrid.create(size, size);
    mGrid.buildVBO();
    fin.close();
    //mCamera.setPosition(size / 2, size / 2, 0);



    GLfloat vertices[24];

    float a = 1.0f;
    float b = 0.0f;

    vertices[0] = b;
    vertices[1] = b;
    vertices[2] = b;

    vertices[3] = b;
    vertices[4] = b;
    vertices[5] = a;

    vertices[6] = b;
    vertices[7] = a;
    vertices[8] = a;

    vertices[9] = a;
    vertices[10] = a;
    vertices[11] = a;

    vertices[12] = a;
    vertices[13] = b;
    vertices[14] = b;

    vertices[15] = a;
    vertices[16] = a;
    vertices[17] = b;

    vertices[18] = b;
    vertices[19] = a;
    vertices[20] = b;

    vertices[21] = a;
    vertices[22] = b;
    vertices[23] = a;




    GLint indices[12 * 3];

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;

    indices[3] = 0;
    indices[4] = 2;
    indices[5] = 6;

    indices[6] = 0;
    indices[7] = 0;
    indices[8] = 0;

    indices[9] = 0;
    indices[10] = 0;
    indices[11] = 0;

    indices[12] = 0;
    indices[13] = 0;
    indices[14] = 0;

    indices[15] = 0;
    indices[16] = 0;
    indices[17] = 0;

    indices[18] = 0;
    indices[19] = 0;
    indices[20] = 0;

    indices[21] = 0;
    indices[22] = 0;
    indices[23] = 0;

    indices[24] = 0;
    indices[25] = 0;
    indices[26] = 0;

    indices[27] = 0;
    indices[28] = 0;
    indices[29] = 0;

    indices[30] = 0;
    indices[31] = 0;
    indices[32] = 0;

    indices[33] = 0;
    indices[34] = 0;
    indices[35] = 0;


    GLfloat textures[16];

    textures[0] = 0;
    textures[1] = 0;

    textures[2] = 0;
    textures[3] = 0;

    textures[4] = 0;
    textures[5] = 0;

    textures[6] = 0;
    textures[7] = 0;

    textures[8] = 0;
    textures[9] = 0;

    textures[10] = 0;
    textures[11] = 0;

    textures[12] = 0;
    textures[13] = 0;

    textures[14] = 0;
    textures[15] = 0;


    CGE::VertexBufferObject vertex;
    vertex.loadData(vertices, 8, 3);

    CGE::VertexBufferObject texture;
    texture.loadData(textures, 8, 2);

    CGE::IndexVBO index;
    index.loadData(indices, 12, 3);

    mTex.loadImage("assets/images/green.png");

    mCube.mount(index);
    mCube.mount(vertex, 0);
    mCube.mount(texture, 1);

//    glBegin(GL_TRIANGLES);
//    {
//        glVertex3f(b, b, b);
//        glVertex3f(0.0f, 0.0f, 0.0f);
//        glVertex3f(0.0f, 0.0f, 0.0f);
//        glVertex3f(0.0f, 0.0f, 0.0f);
//        glVertex3f(0.0f, 0.0f, 0.0f);
//        glVertex3f(0.0f, 0.0f, 0.0f);
//        glVertex3f(0.0f, 0.0f, 0.0f);
//        glVertex3f(0.0f, 0.0f, 0.0f);
//
//
//    }
//    glEnd();

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
    //mModel->display();
    mTex.bind();
    mCube.display();

    mProgram.setMatrix(mvp * mSphere->getPosition());
    mSphere->display();
}

void MapEditorModule::onPulse()
{
    mCamera.update();

    if (mKeyDown)
        mCamera.smartPan(mXPan, mYPan);
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

    if (!CGE::unproject((GLfloat)inX, (GLfloat)newY, depthZ, mProjection * mModelView, mViewport, tempPoint))
    {
        cerr << "unproject failed." << endl;
    }

    int numRows = mGrid.getMatrix().rows();
    int numCols = mGrid.getMatrix().cols();

    if (tempPoint[1] >= numRows || tempPoint[1] < 0 || tempPoint[0] >= numCols || tempPoint[0] < 0)
    {
        currentVertex = mSphere->getTranslation();
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
    //currentVertex[0] = 1.0f;
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
        case NONE:
        {
            vec4f hoverVertex = selectVertex(inX, inY);
            hoverVertex[0] -= 0.05;
            hoverVertex[1] -= 0.05;
            hoverVertex[2] -= 0.05;
            mSphere->setTranslation(hoverVertex);
            //cerr << "sphere pos: " << hoverVertex[0] << ", " << hoverVertex[1] << ", " << hoverVertex[2] << endl;
            break;
        }

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

            mGrid.set((int)mClickedVertex[1], (int)mClickedVertex[0], mClickedVertex[2], false);
            mSphere->setTranslation(mClickedVertex);

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
    glGetIntegerv(GL_VIEWPORT, mViewport);
//    mViewport[0] = 0;
//    mViewport[1] = 0;
//    mViewport[2] = inWidth;
//    mViewport[3] = inHeight;
}
