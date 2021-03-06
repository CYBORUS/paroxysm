#ifndef MAPEDITORMODULE_H
#define MAPEDITORMODULE_H

#include <CGE/OpenGL/Texture2D.h>
#include <CGE/UI/Button.h>
#include <CGE/Matrix4x4.h>
#include <CGE/Camera.h>
#include <CGE/ModelFromFile.h>
#include <CGE/ResourceManager.h>
#include <CGE/Vectors.h>
#include <CGE/Actor.h>

#include "ParoxysmModule.h"

#include "TerrainGrid.h"
#include "GeneralBin.h"

#include <list>

#define VERTEX_STEP 0.1f

class MapEditorModule : public ParoxysmModule
{
    public:
        MapEditorModule();
        virtual ~MapEditorModule();

        enum MouseState
        {
            NONE,
            ROTATING,
            PANNING,
            EDITING_TERRAIN
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

    private:
        vec4f selectVertex(int inX, int inY);

        GeneralBin mBin;
        std::list<CGE::Actor*> mActors;

        TerrainGrid mGrid;
        CGE::Actor* mSphere;
        CGE::Actor* mGridActor;

        CGE::ResourceManager<CGE::ModelFromFile> mManager;

        vec4f mClickedVertex;
        vec4f mSelectPosition;
        vec4f mSpherePosition;

        MouseState mMouseState;

        bool mLeftClickDown;

        int mXStart;
        int mYStart;

        float mXPan;
        float mYPan;
};

#endif
