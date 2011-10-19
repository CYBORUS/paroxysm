#ifndef BUTTON_H
#define BUTTON_H

#include <CGE/Image.h>
#include <CGE/OpenGL/Texture2D.h>
#include <CGE/OpenGL/InterleavedVBO.h>
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
        CGE::InterleavedVBO mVBO;
        GLint mFirst;
};

#endif
