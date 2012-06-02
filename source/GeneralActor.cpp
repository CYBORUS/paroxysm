#include "GeneralActor.h"

GeneralActor::GeneralActor(const CGE::Matrix4x4<float>& inMatrix)
    : mModelViewProjection(inMatrix)
{
}

GeneralActor::~GeneralActor()
{
}
