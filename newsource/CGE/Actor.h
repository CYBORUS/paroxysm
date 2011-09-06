#ifndef ACTOR_H
#define ACTOR_H

#include "CGE/Matrix4x4.h"

namespace CGE
{
    class Actor
    {

        public:
            virtual void display() = 0;

        private:
            mat4f mMatrix;
    };
}
#endif
