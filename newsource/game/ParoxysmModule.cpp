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

    CGE::Matrix4x4<GLfloat> projection;
    projection.perspective(30.0f, ratio, 1.0f, 1000.0f);
    mViewNode.setProjection(projection);
    mViewNode.updateAllMatrices();

    glViewport(0, 0, inWidth, inHeight);
    glGetIntegerv(GL_VIEWPORT, mViewport);
}
