#include "UserInterface.h"
#include <iostream>

UserInterface::UserInterface(float inRange) : mRange(inRange), mRatio(0.0f),
    mMouseX(0.0f), mMouseY(0.0f), mMouseOverWidget(NULL)
{
    if (mRange < 1.0f) mRange = 1.0f;

    mVS.loadFromFile("data/shaders/ui.vs");
    mFS.loadFromFile("data/shaders/ui.fs");
    mProgram.attachShader(mVS);
    mProgram.attachShader(mFS);
    mProgram.bindAttribLocation(0, "iVertex");
    mProgram.bindAttribLocation(1, "iTexture");
    mProgram.link();
    mUniMVPM = mProgram.getUniformLocation("uMVPM");
    mUniTexture = mProgram.getUniformLocation("uTexture");

    glUniform1i(mUniTexture, 0);
}

UserInterface::~UserInterface()
{
}

void UserInterface::update()
{
    updateMatrices(mProjection);
}

void UserInterface::display()
{
    mProgram.use();
    for (std::list<Widget*>::iterator i = mWidgets.begin();
        i != mWidgets.end(); ++i)
    {
        Widget* w = *i;
        glUniformMatrix4fv(mUniMVPM, 1, GL_FALSE, w->compositeMatrix());
        w->display();
    }
}

void UserInterface::addWidget(Widget* inWidget)
{
    mWidgets.push_back(inWidget);
    addChildNode(*inWidget);
}

void UserInterface::removeWidget(Widget* inWidget)
{
    if (mMouseOverWidget == inWidget) mMouseOverWidget = NULL;
    mWidgets.remove(inWidget);
    removeChildNode(*inWidget);
}

void UserInterface::onResize(int inWidth, int inHeight)
{
    mWidth = inWidth;
    mHeight = inHeight;

    mCenterX = inWidth / 2;
    mCenterY = inHeight / 2;

    mRatio = static_cast<float>(mWidth) / static_cast<float>(mHeight);

    mProjection.loadIdentity();
    mProjection.orthographic(mRange, mRatio);
}

void UserInterface::onMouseMove(int inX, int inY)
{
    float adjust = mWidth < mHeight ? mRange / float(mCenterX)
        : mRange / float(mCenterY);

    mMouseX = static_cast<float>(inX - mCenterX) * adjust;
    mMouseY = static_cast<float>(mCenterY - inY) * adjust;

    if (mMouseOverWidget && !mMouseOverWidget->contains(mMouseX, mMouseY))
    {
        mMouseOverWidget->onMouseOut();
        mMouseOverWidget = NULL;
    }

    for (std::list<Widget*>::iterator i = mWidgets.begin();
        i != mWidgets.end(); ++i)
    {
        Widget* w = *i;
        if (w->contains(mMouseX, mMouseY))
        {
            if (mMouseOverWidget && mMouseOverWidget != w)
            {
                mMouseOverWidget->onMouseOut();
            }

            w->onMouseIn();
            mMouseOverWidget = w;
        }
    }
}

void UserInterface::onMouseDown()
{
    std::cout << mMouseX << ", " << mMouseY << std::endl;
    if (mMouseOverWidget) mMouseOverWidget->onMouseDown();
}
