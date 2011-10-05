#ifndef ACTOR_H
#define ACTOR_H

#include "game/SceneGraphNode.h"
#include "Matrix4x4.h"
#include "Model.h"

namespace CGE
{
    class Actor : public SceneGraphNode
    {

        public:
            Actor(Model* inModel) { mModel = inModel; }
            virtual void display() { mModel->display(); };

        private:
            CGE::Model* mModel;

    };
}
#endif
