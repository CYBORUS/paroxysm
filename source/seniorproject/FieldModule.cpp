#include "FieldModule.h"

#include <ctime>

FieldModule::FieldModule()
{
}

FieldModule::~FieldModule()
{
}

bool FieldModule::onLoad()
{
    srand(time(NULL));
    mField.createRandom();
    mField.dump();
    mBall.setScale(0.4f);
    mBall.moveSphere(0.5f, 0.5f, 0.5f);
    mBall.setColor(0.6f, 0.6f, 1.0f);
    mCamera.setTrackball(Vector3D<float>(67.5f, 0.0f, 10.0f));
    return true;
}

void FieldModule::onUnload()
{
}

void FieldModule::onOpen()
{
    mRunning = true;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    GLdouble w = SDL_GetVideoSurface()->w;
    GLdouble h = SDL_GetVideoSurface()->h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FIELD_OF_VIEW, w / h, NEAR_CP, FAR_CP);

    glMatrixMode(GL_MODELVIEW);
}

void FieldModule::onClose()
{
}

void FieldModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    mCamera.transform();
    mField.display();
    mBall.display();
}

void FieldModule::onFrame()
{
    mCamera.spin(1.0f);
    mCamera.setPanning(mBall.getTranslation());
}
