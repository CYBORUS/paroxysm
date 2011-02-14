#include "MapEditorModule.h"
#include <CGE/Exception.h>

#include <fstream>
using namespace std;

MapEditorModule::MapEditorModule()
{
}

MapEditorModule::~MapEditorModule()
{
}

void MapEditorModule::onLoad(CGE::PropertyList& inList)
{
    mTerrainTexture.loadImage("assets/images/green.png");
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

    mSpin = 0.0f;
}

void MapEditorModule::onUnload()
{
}

void MapEditorModule::onOpen()
{
    mRunning = true;
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);

    mTerrainTexture.bind();
}

void MapEditorModule::onClose()
{
    glDisable(GL_DEPTH_TEST);
}

void MapEditorModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mModelView.loadIdentity();
    mModelView.translate(0.0f, 0.0f, -20.0f);
    mModelView.rotateX(-45.0f);
    mModelView.rotateZ(mSpin);
    mModelView.translate(-100.0f, -100.0f, 0.0f);

    CGE::Matrix4x4<GLfloat> mvp(mProjection, mModelView);
    mProgram.setMatrix(mvp);
    mGrid.display();
}

void MapEditorModule::onPulse()
{
    mSpin += 1.0f;
    if (mSpin > 180.0f) mSpin -= 360.0f;
}

void MapEditorModule::onResize(int inWidth, int inHeight)
{
    GLfloat ratio = static_cast<GLfloat>(inWidth)
        / static_cast<GLfloat>(inHeight);
    mProjection.loadIdentity();
    mProjection.perspective(30.0f, ratio, 1.0f, 1000.0f);
    glViewport(0, 0, inWidth, inHeight);
}
