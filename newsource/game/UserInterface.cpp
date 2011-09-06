#include "UserInterface.h"

UserInterface::UserInterface() : mRatio(0.0f), mMouseOverWidget(NULL)
{
    memset(mMouse2D, 0, sizeof(mMouse2D));
}

UserInterface::~UserInterface()
{
}

void UserInterface::addWidget(Widget* inWidget)
{
    mWidgets.push_back(inWidget);
    addChildNode(*inWidget);
}

void UserInterface::removeWidget(Widget* inWidget)
{
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
    mProjection.orthographic(Range, mRatio);
}

void UserInterface::onMouseMove(int inX, int inY)
{
    float adjust = mWidth < mHeight ? Range / float(mCenterX)
        : Range / float(mCenterY);

    mMouse2D[0] = static_cast<float>(inX - mCenterX) * adjust;
    mMouse2D[1] = static_cast<float>(mCenterY - inY) * adjust;
}
