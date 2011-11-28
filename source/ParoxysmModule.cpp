#include "ParoxysmModule.h"

ParoxysmModule::ParoxysmModule()
{
}

ParoxysmModule::~ParoxysmModule()
{
}

void ParoxysmModule::onResize(int inWidth, int inHeight)
{
    GLfloat ratio = static_cast<GLfloat>(inWidth)
        / static_cast<GLfloat>(inHeight);

    mUI.onResize(inWidth, inHeight);
    mUI.update();

    mProjection.loadIdentity();
    mProjection.perspective(30.0f, ratio, 1.0f, 1000.0f);
    mViewNode.setProjection(mProjection);
    mViewNode.updateAllMatrices();

    glViewport(0, 0, inWidth, inHeight);
    glGetIntegerv(GL_VIEWPORT, mViewport);
}
