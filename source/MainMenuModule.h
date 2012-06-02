#ifndef MAINMENUMODULE_H
#define MAINMENUMODULE_H

#include <CGE/OpenGL/Texture2D.h>
#include <CGE/UI/UserInterface.h>
#include <CGE/UI/Button.h>
#include <CGE/UI/Label.h>
#include <CGE/UI/TextBox.h>
#include <CGE/Matrix4x4.h>
#include <CGE/Camera.h>
#include <CGE/ResourceManager.h>
#include <CGE/Vectors.h>

#include "ParoxysmModule.h"

#include <list>

class MainMenuModule : public ParoxysmModule
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

    protected:
    private:
        int mXStart;
        int mYStart;

        float mXPan;
        float mYPan;

        CGE::Font mFont;
        CGE::TextBox* mTextBox;
        float mCount;

        static void mapEditorButtonCallBack(CGE::Widget* inWidget,
            void* inData);
        static void newGameButtonCallBack(CGE::Widget* inWidget, void* inData);
        static void logoSpinCallback(CGE::Widget* inWidget, void* inData);
};

#endif
