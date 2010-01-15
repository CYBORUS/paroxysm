#ifndef MAPEDITORMODULE_H
#define MAPEDITORMODULE_H

#include "Module.h"
#include "Matrix.h"
#include "Vector3D.h"

#include <SDL_opengl.h>

#define TRACKBALL_STEP 1.0f
#define PANNING_STEP 0.1f
#define MM_DEFAULT 0
#define MM_PANNING 1
#define MM_ROTATING 2

class MapEditorModule : public Module
{
    public:
        /// module operation
        virtual bool onInit();
        virtual void onLoop();
        virtual void onFrame();

        virtual void onCleanup();

        virtual void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onLButtonDown(int inX, int inY);
        virtual void onLButtonUp(int inX, int inY);
        virtual void onRButtonDown(int inX, int inY);
        virtual void onRButtonUp(int inX, int inY);
        virtual void onMouseWheel(bool inUp, bool inDown);
        virtual void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);

    private:
        Matrix<int> mTerrainHeight;
        GLfloat* mTerrainVertices;
        GLuint* mTerrainIndices;

        GLuint mList;

        GLsizei mNumIndices;
        Vector3D<GLfloat> mTrackball;
        Vector3D<GLfloat> mPanning;
        int mCenterX;
        int mCenterY;
        int mOldMouseX;
        int mOldMouseY;
        int mMouseMode;
};

#endif
