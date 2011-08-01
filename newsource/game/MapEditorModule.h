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
#include "GeneralProgram.h"

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
        vec3f selectVertex(int inX, int inY);

        GeneralProgram mProgram;
        TerrainGrid mGrid;
        CGE::Model* mModel;
        CGE::ResourceManager<CGE::Model> mManager;

        MouseState mMouseState;


        CGE::Camera mCamera;
        CGE::Matrix4x4<GLfloat> mProjection;
        CGE::Matrix4x4<GLfloat> mModelView;
        vec4f mViewport;

        bool mLeftClickDown;
        bool mKeyDown;

        int mXStart;
        int mYStart;

        float mXPan;
        float mYPan;
};

#endif
