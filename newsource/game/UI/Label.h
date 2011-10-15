#ifndef LABEL_Hx
#define LABEL_H

#include <CGE/Image.h>
#include <CGE/OpenGL/Texture2D.h>
#include <CGE/OpenGL/ClusterVBO.h>
#include "Widget.h"


class Label : public Widget
{
    public:
        Label(const CGE::Image& inImage, float inWidth, float inHeight);
        virtual ~Label();

        void setPosition(float inX, float inY);

        virtual void display();

    protected:
    private:
        CGE::Texture2D mTexture;
        CGE::VertexBufferObject mVertexVBO;
        CGE::VertexBufferObject mTextureVBO;
        CGE::ClusterVBO mClusterVBO;
};
#endif // LABEL_H
