#ifndef GAMEMODULE_H
#define GAMEMODULE_H

#include "ParoxysmModule.h"
#include "Tank.h"
#include <CGE/Actor.h>
#include <CGE/Model.h>
#include "GeneralBin.h"
#include "TerrainGrid.h"

#include <iostream>
#include <fstream>


class GameModule : public ParoxysmModule
{
    public:
        GameModule();
        virtual ~GameModule();

        enum MouseState
        {
            NONE,
            ROTATING,
            PANNING,
        };

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
        virtual void onKeyDown(SDLKey inSym, SDLMod inMod,
            Uint16 inUnicode);
        virtual void onKeyUp(SDLKey inSym, SDLMod inMod,
            Uint16 inUnicode);

        virtual void onMouseWheel(bool inUp);


    protected:
    private:
        TerrainGrid mGrid;

        GeneralBin mBin;
        std::list<CGE::Actor*> mActors;

        CGE::ResourceManager<CGE::Model> mManager;

        Tank* mPlayerTank;

        MouseState mMouseState;

        int mXStart;
        int mYStart;

        float mXPan;
        float mYPan;


};

#endif // GAMEMODULE_H