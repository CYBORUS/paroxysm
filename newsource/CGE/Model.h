#ifndef MODEL_H
#define MODEL_H

#include "Texture2D.h"
#include "c3m.h"
#include "ClusterVBO.h"
#include <CGE/Matrix4x4.h>

#include <string>
#include <iostream>

namespace CGE
{
    class Model
    {
        public:
            Model(const char* inFile);
            virtual ~Model();

            inline void display()
            {
                mTexture.bind();
                mClusterVBO.display();
            }

            void setTranslation(vec3f inTranslation);
            vec3f getTranslation();
            inline Matrix4x4<float> getPosition() { return mTranslation; }

        private:

            void loadC3M(const char* inFile);

            CGE::Texture2D mTexture;
            CGE::ClusterVBO mClusterVBO;
            CGE::VertexBufferObject mBuffers[4];
            CGE::IndexVBO mIVBO;
            CGE::Matrix4x4<float> mTranslation;
            vec3f mTranslationValues;
    };
}

#endif
