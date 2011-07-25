#ifndef MAPEDITORMODULE_H
#define MAPEDITORMODULE_H

#include <CGE/ManagedModule.h>
#include <CGE/Texture2D.h>
#include <CGE/Matrix4x4.h>
#include <CGE/Camera.h>
#include <CGE/Model.h>
#include <CGE/ResourceManager.h>
#include "TerrainGrid.h"
#include "GeneralProgram.h"

class MapEditorModule : public CGE::ManagedModule
{
    public:
        MapEditorModule();
        virtual ~MapEditorModule();

        virtual void onLoad(CGE::PropertyList& inList);
        virtual void onUnload();
        virtual void onOpen();
        virtual void onClose();
        virtual void onLoop();
        virtual void onPulse();
        virtual void onLButtonDown(int inX, int inY);
        virtual void onLButtonUp(int inX, int inY);
        virtual void onMouseMove(int inX, int inY, int inRelX, int inRelY,
                bool inLeft, bool inRight, bool inMiddle);

        virtual void onResize(int inWidth, int inHeight);

    private:
        GeneralProgram mProgram;
        TerrainGrid mGrid;
        CGE::Model* mModel;
        CGE::ResourceManager<CGE::Model> mManager;


        CGE::Camera mCamera;
        CGE::Matrix4x4<GLfloat> mProjection;
        CGE::Matrix4x4<GLfloat> mModelView;

        bool lClickDown;
        int xStart;
        int yStart;
};

#endif
