#include "Button.h"

Button::Button(const CGE::Image& inImage, float inWidth, float inHeight) : mFirst(0)
{
    mTexture.loadImage(inImage);

    const float Min = 0.1f;
    if (inWidth < Min) inWidth = Min;
    if (inHeight < Min) inHeight = Min;

    mRadiusX = inWidth / 2.0f;
    mRadiusY = inHeight / 2.0f;

    mClusterVBO.mount(mVertexVBO, 0);
    mClusterVBO.mount(mTextureVBO, 1);

    GLfloat vertices[4 * 4 * 2];
    GLfloat* vertex = vertices;

    GLfloat texCoords[4 * 4 * 2];
    GLfloat* texCoord = texCoords;

    for (size_t i = 0; i < 4; ++i)
    {
        float tcy = float(i) * 0.25f;
        *vertex++ = mRadiusX;
        *vertex++ = mRadiusY;
        *texCoord++ = 1.0f;
        *texCoord++ = tcy + 0.25f;

        *vertex++ = mRadiusX;
        *vertex++ = -mRadiusY;
        *texCoord++ = 1.0f;
        *texCoord++ = tcy;

        *vertex++ = -mRadiusX;
        *vertex++ = -mRadiusY;
        *texCoord++ = 0.0f;
        *texCoord++ = tcy;

        *vertex++ = -mRadiusX;
        *vertex++ = mRadiusY;
        *texCoord++ = 0.0f;
        *texCoord++ = tcy + 0.25f;
    }

    mVertexVBO.loadData(vertices, 16, 2);
    mTextureVBO.loadData(texCoords, 16, 2);
}

Button::~Button()
{
}

void Button::setPosition(float inX, float inY)
{
    mX = inX;
    mY = inY;

    mTransform.loadIdentity();
    mTransform.translate(mX, mY, 0.0f);
}


void Button::display()
{
    mTexture.bind();
    mClusterVBO.display(GL_TRIANGLE_FAN, mFirst, 4);
}
