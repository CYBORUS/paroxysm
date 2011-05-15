#ifndef MODEL_H
#define MODEL_H

#include "Texture2D.h"

class Model
{
    public:
        Model();
        Model(const char* inFile);
        virtual ~Model();

        void display();

    protected:

    private:

        void loadC3M(const char* inFile);

        Texture2D mTexture;
};

inline void Model::display()
{
    if (glIsTexture(mTexture))
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, mTexture);

        mVBO.display();

        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        mVBO.display();
    }

}

#endif // MODEL_H
