#include "SkyBoxActor.h"

SkyBoxActor::SkyBoxActor(SkyBox& inSkyBox) : mSkyBox(inSkyBox),
    mModelViewProjection(mMatrix)
{
}

SkyBoxActor::~SkyBoxActor()
{
}

void SkyBoxActor::display()
{
    mSkyBox.display();
}
