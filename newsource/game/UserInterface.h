#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <CGE/Matrix4x4.h>
#include "SceneGraphNode.h"
#include "Widget.h"
#include <list>

class UserInterface : protected SceneGraphNode
{
    public:
        UserInterface();
        virtual ~UserInterface();

        void addWidget(Widget* inWidget);
        void removeWidget(Widget* inWidget);

        void onResize(int inWidth, int inHeight);
        void onMouseMove(int inX, int inY);

    protected:
    private:
        mat4f mProjection;
        float mRatio;
        int mWidth;
        int mHeight;
        int mCenterX;
        int mCenterY;
        float mMouse2D[2];

        Widget* mMouseOverWidget;
        std::list<Widget*> mWidgets;

        static const float Range = 4.0f;
};

#endif
