#include "UserInterface.h"

UserInterface::UserInterface(float inRange) : mRange(inRange), mRatio(0.0f),
    mMouseX(0.0f), mMouseY(0.0f), mMouseOverWidget(NULL)
{
    if (mRange < 1.0f) mRange = 1.0f;
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
