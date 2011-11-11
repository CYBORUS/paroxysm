#include "SkyBoxBin.h"
#include <CGE/OpenGL/VertexShader.h>
#include <CGE/OpenGL/FragmentShader.h>

SkyBoxBin::SkyBoxBin()
{
    CGE::VertexShader vs;
    CGE::FragmentShader fs;

    vs.loadFromFile("data/shaders/skybox.vs");
    fs.loadFromFile("data/shaders/skybox.fs");

    mProgram.attachShader(vs);
    mProgram.attachShader(fs);

    mProgram.bindAttribLocation(0, "iPosition");
    mProgram.bindAttribLocation(1, "iTexture");
    mProgram.link();

    mUniMVPM = mProgram.getUniformLocation("uMVPM");
    mUniTexture = mProgram.getUniformLocation("uTexture");

    glUniform1i(mUniTexture, 0);
}

SkyBoxBin::~SkyBoxBin()
{
}

void SkyBoxBin::beforeRender()
{
    mProgram.use();
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
}

void SkyBoxBin::afterRender()
{
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}

void SkyBoxBin::displayActor(CGE::Actor* inActor)
{
    glUniformMatrix4fv(mUniMVPM, 1, GL_FALSE, inActor->compositeMatrix());
    inActor->display();
}
