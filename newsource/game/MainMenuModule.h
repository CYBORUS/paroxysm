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

#include "TerrainGrid.h"
#include "ViewNode.h"
#include "GeneralBin.h"
#include "ParoxysmEntity.h"
#include "Tank.h"

#include <list>

#define VERTEX_STEP 0.1f

class MainMenuModule : public CGE::ManagedModule
{
    public:
        MainMenuModule();
        virtual ~MainMenuModule();

        enum MouseState
        {
            NONE,
            ROTATING,
            PANNING,
            EDITING_TERRAIN
        };

        virtual void onLoad();
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

        MouseState mMouseState;

        CGE::Vector<GLint, 4> mViewport;

        bool mLeftClickDown;

        int mXStart;
        int mYStart;

        float mXPan;
        float mYPan;

        UserInterface mUI;

        static void uiLoadMap(Widget* inWidget, void* inData);
};

#endif
