#ifndef MAPEDITORMODULE_H
#define MAPEDITORMODULE_H

#include <CGE/ManagedModule.h>
#include <CGE/Texture2D.h>
#include <CGE/Matrix4x4.h>
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

        virtual void onResize(int inWidth, int inHeight);

    private:
        GeneralProgram mProgram;
        TerrainGrid mGrid;
        float mSpin;

        CGE::Texture2D mTerrainTexture;
        CGE::Matrix4x4<GLfloat> mProjection;
        CGE::Matrix4x4<GLfloat> mModelView;
};

#endif
