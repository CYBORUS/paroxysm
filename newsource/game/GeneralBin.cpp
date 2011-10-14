#include "GeneralBin.h"

GeneralBin::GeneralBin()
{
    mVS.loadFromFile("data/shaders/general.vs");
    mFS.loadFromFile("data/shaders/general.fs");
    mProgram.attachShader(mVS);
    mProgram.attachShader(mFS);
    mProgram.bindAttribLocation(0, "iVertex");
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
}
