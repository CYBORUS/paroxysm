#ifndef MAPEDITORMODULE_H
#define MAPEDITORMODULE_H

#include <CGE/ManagedModule.h>
#include <CGE/Texture2D.h>
#include <CGE/Matrix4x4.h>
#include <CGE/Camera.h>
#include <CGE/Model.h>
#include <CGE/ResourceManager.h>
#include <CGE/Vectors.h>
#include "TerrainGrid.h"

#include "ViewNode.h"
#include "GeneralBin.h"
#include "ActorNode.h"
#include "SimpleMatrixNode.h"

#include <list>

#define VERTEX_STEP 0.1f

class MapEditorModule : public CGE::ManagedModule
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

        virtual void onResize(int inWidth, int inHeight);

    private:
        vec4f selectVertex(int inX, int inY);

        ViewNode mViewNode; // head of the scene graph
        GeneralBin mBin;
        std::list<ActorNode*> mActors;

        TerrainGrid mGrid;
        CGE::Model* mModel;
        CGE::Model* mSphere;
        SimpleMatrixNode* mSphereNode;

        CGE::ResourceManager<CGE::Model> mManager;

        vec4f mClickedVertex;
        vec4f mSpherePosition;

        MouseState mMouseState;

        CGE::Vector<GLint, 4> mViewport;

        bool mLeftClickDown;

        int mXStart;
        int mYStart;

        float mXPan;
        float mYPan;
};

#endif
