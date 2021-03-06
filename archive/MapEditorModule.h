/**
 *  This file is part of "Paroxysm".
 *
 *  "Paroxysm" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Paroxysm" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with "Paroxysm".  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAPEDITORMODULE_H
#define MAPEDITORMODULE_H

#include "Module.h"
#include "Matrix.h"
#include "Vector3D.h"
#include "Point2D.h"
#include "TSphere.h"
#include "TerrainGrid.h"
#include "MapEditorAction.h"
#include "SoundEngine.h"
#include "HUD.h"
#include "Button.h"
#include "TextBox.h"
#include "Label.h"

#include "OGL.h"

#include <cmath>
#include <fstream>
#include <list>
using namespace std;

#define VERTEX_DISTANCE 0.0f
#define VERTEX_STEP 0.1f

/// edit mode options
#define EM_TERRAIN 0
#define EM_TILE 1

/// button IDs
#define NUM_BUTTONS 5
#define B_TERRAIN_MODE 0
#define B_TILE_MODE 1
#define B_UNDO 2
#define B_REDO 3
#define B_SAVE 4

/// mode button details
#define MB_POS_X -8.0f
#define MB_POS_Y -2.0f
#define MB_WIDTH 4.0f
#define MB_HEIGHT 1.0f

class MapEditorModule : public Module
{
    public:
        /// module operation
        virtual bool onLoad();
        virtual void onOpen();
        virtual void onLoop();
        virtual void onFrame();
        virtual void onClose();
        virtual void onUnload();

        virtual bool isDead();
        virtual Module* next();

        void setSize(int inX, int inY);
        void loadMapFile(const char* inFile);

    protected:
        virtual void onKeyDown(SDLKey inSym, SDLMod inMod, Uint16 inUnicode);
        virtual void onLButtonDown(int inX, int inY);
        virtual void onLButtonUp(int inX, int inY);
        virtual void onRButtonDown(int inX, int inY);
        virtual void onRButtonUp(int inX, int inY);
        virtual void onMouseWheel(bool inUp, bool inDown);
        virtual void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);

    private:
        void doAction();
        void undoAction();
        void redoAction();

        virtual void onButtonPress(int inID);
        void switchModes();

        Vector3D<float> selectVertex(int inX, int inY);

        Module* mNextModule;
        bool mDead;

        Point2D<int> mTerrainSize;
        TerrainGrid mTerrainGrid;
        Light mLight;

        TSphere mSphere;

        Vector3D<GLfloat> mTrackball;
        Vector3D<GLfloat> mPanning;

        bool mSceneChanged;

        Matrix<GLdouble> mModelView;
        Vector3D<GLint> mViewport;
        Matrix<GLdouble> mProjection;

        Vector3D<float> mClickedVertex;

        Point2D<int> mDisplay;
        Point2D<int> mCenter;
        Point2D<int> mOldMouse;
        int mMouseMode;
        int mEditMode;

        list<MapEditorAction*> mUndo;
        list<MapEditorAction*> mRedo;
        MapEditorAction* mCurrentAction;

        Button* mButtons[NUM_BUTTONS];
        TextBox* mSaveBox;

        Label* mFPSLabel;
        unsigned int mFPS;
};

#endif
