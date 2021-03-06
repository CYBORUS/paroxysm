#include "GeneralBin.h"
#include <CGE/OpenGL/VertexShader.h>
#include <CGE/OpenGL/FragmentShader.h>

GeneralBin::GeneralBin()
{
    CGE::VertexShader vs;
    CGE::FragmentShader fs;

    vs.loadFromFile("data/shaders/general.vs");
    fs.loadFromFile("data/shaders/general.fs");

    mProgram.attachShader(vs);
    mProgram.attachShader(fs);

    mProgram.bindAttribLocation(0, "iPosition");
    mProgram.bindAttribLocation(1, "iTexture");
    mProgram.link();

    mUniMVPM = mProgram.getUniformLocation("uMVPM");
    mUniTexture = mProgram.getUniformLocation("uTexture");

    glUniform1i(mUniTexture, 0);
}

GeneralBin::~GeneralBin()
{
}

void GeneralBin::displayActor(CGE::Actor* inActor)
{
    glUniformMatrix4fv(mUniMVPM, 1, GL_FALSE, inActor->compositeMatrix());
    inActor->display();
}

void GeneralBin::beforeRender()
{
    mProgram.use();
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
}

void GeneralBin::afterRender()
{
    //glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}
