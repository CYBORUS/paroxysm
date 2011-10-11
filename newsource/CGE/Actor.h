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
            friend class RenderBin;

            Actor(Model* inModel);
            virtual ~Actor();

            virtual void display();

            void remove();

            inline CGE::Matrix4x4<float>& matrix() { return mTransform; }
        private:
            Model* mModel;

            Actor** mHead;
            Actor* mNextActor;
            Actor* mPreviousActor;
    };
}
#endif
