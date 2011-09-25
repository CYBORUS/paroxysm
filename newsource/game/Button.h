#ifndef BUTTON_H
#define BUTTON_H

#include <CGE/Texture2D.h>
#include <CGE/ClusterVBO.h>
#include "Widget.h"

class Button : public Widget
{
    public:
        Button(CGE::Texture2D& inTexture, float inWidth, float inHeight);
        virtual ~Button();

        void setPosition(float inX, float inY);

        virtual void display();
    protected:
    private:
        CGE::Texture2D& mTexture;
        CGE::VertexBufferObject mVertexVBO;
        CGE::VertexBufferObject mTextureVBO;
        CGE::ClusterVBO mClusterVBO;
        GLint mFirst;
};

#endif
