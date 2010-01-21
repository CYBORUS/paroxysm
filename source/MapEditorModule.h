#ifndef MAPEDITORMODULE_H
#define MAPEDITORMODULE_H

#include "Module.h"
#include "Matrix.h"
#include "Vector3D.h"
#include "TSphere.h"
#include "TerrainGrid.h"

#include <SDL_opengl.h>

#include <cmath>
#include <fstream>
using namespace std;

#define VERTEX_DISTANCE 0.0f
#define TRACKBALL_STEP 1.0f
#define PANNING_STEP 0.1f
#define MM_DEFAULT 0
#define MM_PANNING 1
#define MM_ROTATING 2
#define MM_EDITING_VERTEX 3

struct Light
{
    Vector3D<GLfloat> ambient;
    Vector3D<GLfloat> diffuse;
    Vector3D<GLfloat> specular;

    Vector3D<GLfloat> position;
    Vector3D<GLfloat> direction;
};

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
        int mTerrainSize;
        TerrainGrid mTerrainGrid;
        Light mLight;

        GLuint mList;

        TSphere mSphere;

        Vector3D<GLfloat> mTrackball;
        Vector3D<GLfloat> mPanning;

        bool mSceneChanged;

        Matrix<GLdouble> mModelView;
        Vector3D<GLint> mViewport;
        Matrix<GLdouble> mProjection;

        Vector3D<float> mClickedVertex;

        int mCenterX;
        int mCenterY;
        int mOldMouseX;
        int mOldMouseY;
        int mMouseMode;
};

#endif
