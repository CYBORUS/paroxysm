#include "ModelActor.h"

ModelActor::ModelActor(CGE::ModelFromFile& inModel) : GeneralActor(mMatrix),
    mModel(inModel),
    mModelViewProjection(mMatrix)
{
}

ModelActor::~ModelActor()
{
}

void ModelActor::display()
{
    mModel.display();
}

void ModelActor::rotateX(double inDegrees)
{
    mRotation[0] += inDegrees;
    mMatrix.rotateX(inDegrees);
}

void ModelActor::rotateY(double inDegrees)
{
    mRotation[1] += inDegrees;
    mMatrix.rotateY(inDegrees);
}
void ModelActor::rotateZ(double inDegrees)
{
    mRotation[2] += inDegrees;
    mMatrix.rotateZ(inDegrees);
}
