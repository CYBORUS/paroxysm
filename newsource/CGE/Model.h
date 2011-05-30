#ifndef MODEL_H
#define MODEL_H

#include "Texture2D.h"
#include "c3m.h"
#include "ClusterVBO.h"

#include <string>
#include <iostream>

class Model
{
    public:
        //Model();
        Model(const char* inFile);
        virtual ~Model();

        void display();

    protected:

    private:

        void loadC3M(const char* inFile);

        CGE::Texture2D mTexture;
        CGE::ClusterVBO* mVBO;
        CGE::VertexBufferObject mBuffers[4];
        CGE::IndexVBO mIVBO;
};

inline void Model::display()
{
//    if (glIsTexture(mTexture))
//    {
//        glEnable(GL_TEXTURE_2D);
//        glBindTexture(GL_TEXTURE_2D, mTexture);
//
//        mVBO.display();
//
//        glDisable(GL_TEXTURE_2D);
//    }
//    else
//    {
//        mVBO.display();
//    }

    mTexture.bind();
    mVBO->display();
}

#endif // MODEL_H
