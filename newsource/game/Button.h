#ifndef BUTTON_H
#define BUTTON_H

#include <CGE/Image.h>
#include <CGE/Texture2D.h>
#include <CGE/ClusterVBO.h>
#include "Widget.h"

class Button : public Widget
{
    public:
        Button(const CGE::Image& inImage, float inWidth, float inHeight);
        virtual ~Button();

        void setPosition(float inX, float inY);

        virtual void display();

        virtual void onMouseIn(bool inIsClickCandidate);
        virtual void onMouseOut();
        virtual void onMouseDown();
        virtual void onMouseUp();

    protected:
    private:
        CGE::Texture2D mTexture;
        CGE::VertexBufferObject mVertexVBO;
        CGE::VertexBufferObject mTextureVBO;
        CGE::ClusterVBO mClusterVBO;
        GLint mFirst;
};

#endif
