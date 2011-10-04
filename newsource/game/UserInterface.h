#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <CGE/Matrix4x4.h>
#include <CGE/Program.h>
#include <CGE/VertexShader.h>
#include <CGE/FragmentShader.h>
#include "SceneGraphNode.h"
#include "Widget.h"
#include <list>

class UserInterface : protected SceneGraphNode
{
    public:
        UserInterface(float inRange = 4.0f);
        virtual ~UserInterface();

        void update();
        void display();

        void addWidget(Widget* inWidget);
        void removeWidget(Widget* inWidget);

        void onResize(int inWidth, int inHeight);
        void onMouseMove(int inX, int inY);
        void onMouseDown();
        void onMouseUp();

    protected:
    private:
        mat4f mProjection;
        float mRange;
        float mRatio;
        int mWidth;
        int mHeight;
        int mCenterX;
        int mCenterY;
        float mMouseX;
        float mMouseY;

        Widget* mMouseOverWidget;
        Widget* mClickCandidate;
        std::list<Widget*> mWidgets;

        CGE::Program<2> mProgram;
        CGE::VertexShader mVS;
        CGE::FragmentShader mFS;

        GLuniform mUniMVPM;
        GLuniform mUniTexture;
};

#endif
