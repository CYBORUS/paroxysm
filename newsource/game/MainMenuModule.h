#ifndef MAINMENUMODULE_H
#define MAINMENUMODULE_H

#include <CGE/ManagedModule.h>

#include "Button.h"
#include "UserInterface.h"

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

    protected:
    private:

    UserInterface mUI;

    static void uiLoadMap(Widget* inWidget, void* inData);
};

#endif
