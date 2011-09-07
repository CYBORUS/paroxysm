#ifndef ACTOR_H
#define ACTOR_H

#include "CGE/Matrix4x4.h"
#include "CGE/Model.h"

namespace CGE
{
    class Actor
    {

        public:
            virtual void display();

            inline mat4f* getMatrix() { return &mMatrix; }

        private:
            CGE::Model* mModel;
            mat4f mMatrix;
    };

    void Actor::display()
    {
        mModel->display();
    }
}
#endif
