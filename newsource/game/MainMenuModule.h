#ifndef MAINMENUMODULE_H
#define MAINMENUMODULE_H

#include <CGE/ManagedModule.h>
#include <CGE/OpenGL/Texture2D.h>
#include <CGE/Matrix4x4.h>
#include <CGE/Camera.h>
#include <CGE/Model.h>
#include <CGE/ResourceManager.h>
#include <CGE/Vectors.h>
#include <CGE/Actor.h>

#include "UI/UserInterface.h"
#include "UI/Button.h"
#include "UI/Label.h"

#include "ViewNode.h"

#include <list>

#define VERTEX_STEP 0.1f

class MainMenuModule : public CGE::ManagedModule
{
    public:
        MainMenuModule();
        virtual ~MainMenuModule();

        virtual void onLoad(CGE::PropertyList& inList);
        virtual void onUnload();
        virtual void onOpen();
        virtual void onClose();
        virtual void onLoop();
        virtual void onPulse();
        virtual void onLButtonDown(int inX, int inY);
        virtual void onLButtonUp(int inX, int inY);
        virtual void onRButtonDown(int inX, int inY);
        virtual void onRButtonUp(int inX, int inY);
        virtual void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);

        virtual void onResize(int inWidth, int inHeight);

    protected:
    private:

        ViewNode mViewNode; // head of the scene graph

        CGE::Vector<GLint, 4> mViewport;

        int mXStart;
        int mYStart;

        float mXPan;
        float mYPan;

        UserInterface mUI;

        static void mapEditorButtonCallBack(Widget* inWidget, void* inData);
        static void newGameButtonCallBack(Widget* inWidget, void* inData);
};

#endif
