#ifndef MODEL_H
#define MODEL_H

#include "Texture2D.h"
#include "c3m.h"
#include "ClusterVBO.h"

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

        private:

            void loadC3M(const char* inFile);

            CGE::Texture2D mTexture;
            CGE::ClusterVBO mClusterVBO;
            CGE::VertexBufferObject mBuffers[4];
            CGE::IndexVBO mIVBO;
    };
}

#endif
