#ifndef MAPEDITORMODULE_H
#define MAPEDITORMODULE_H

#include "Module.h"
#include "Matrix.h"
#include "Vector3D.h"

#include <SDL_opengl.h>

#define DEFAULT_TERRAIN_SIZE 10
#define TRACKBALL_STEP 1.0f

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
        virtual void onMouseWheel(bool inUp, bool inDown);
        virtual void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);

    private:
        Matrix<int> mTerrainHeight;
        GLfloat* mTerrainVertices;
        GLubyte* mTerrainIndices;
        GLsizei mNumIndices;
        Vector3D<GLfloat> mTrackball;
        int mCenterX;
        int mCenterY;
        bool mTrackMode;
};

#endif
