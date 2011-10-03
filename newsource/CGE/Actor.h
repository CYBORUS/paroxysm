#ifndef ACTOR_H
#define ACTOR_H

#include "Matrix4x4.h"

namespace CGE
{
    class Actor
    {

        public:
            virtual void display() = 0;

            inline mat4f& matrix() { return mMatrix; }

        private:
            mat4f mMatrix;
    };
}
#endif
