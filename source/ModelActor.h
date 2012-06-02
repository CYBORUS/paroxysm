#ifndef MODELACTOR_H
#define MODELACTOR_H

#include "GeneralActor.h"
#include <CGE/ModelFromFile.h>
#include <CGE/Vectors.h>

class ModelActor : public GeneralActor
{
    public:
        ModelActor(CGE::ModelFromFile& inModel);
        virtual ~ModelActor();

        virtual void display();



        inline void resetMatrix() { mMatrix.loadIdentity(); }

        void rotateX(double inDegrees);
        void rotateY(double inDegrees);
        void rotateZ(double inDegrees);

        inline const vec3d& getRotation() const { return mRotation; }

    protected:
    private:
        CGE::ModelFromFile& mModel;
        CGE::Matrix4x4<float> mMatrix;
        CGE::MatrixNode<float> mModelViewProjection;
        vec3d mRotation;
};

#endif
