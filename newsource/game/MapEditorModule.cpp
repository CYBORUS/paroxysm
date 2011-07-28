#include "MapEditorModule.h"
#include <CGE/Exception.h>

#include <fstream>
using namespace std;

MapEditorModule::MapEditorModule()
{
    mModel = mManager.load("bradley.c3m");
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

    fin >> mGrid;
    fin.close();
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
    mCamera.changeRotation(20.0f);
    mCamera.update();
}

void MapEditorModule::onResize(int inWidth, int inHeight)
{
    GLfloat ratio = static_cast<GLfloat>(inWidth)
        / static_cast<GLfloat>(inHeight);
    mProjection.loadIdentity();
    mProjection.perspective(30.0f, ratio, 1.0f, 1000.0f);
    glViewport(0, 0, inWidth, inHeight);
}
