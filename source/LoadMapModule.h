#ifndef LOADMAPMODULE_H
#define LOADMAPMODULE_H

#include "Module.h"
#include "Point2D.h"
#include "HUD.h"

#include "Button.h"
#include "ScrollList.h"
#include "TextBox.h"
#include "Label.h"
#include "ScrollList.h"

#include "MapEditorModule.h"

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

#include <iostream>
#include <cstdlib>
using namespace std;

enum LoadMapHUD { X_SIZE, Y_SIZE };


class LoadMapModule : public Module
{
    public:
        bool onLoad();
        void onInit();
        void onLoop();
        void onFrame();
        void onCleanup();
        Module* next();

    protected:
        void onMouseMove(int inX, int inY, int inRelX, int inRelY,
            bool inLeft, bool inRight, bool inMiddle);
        void onMouseWheel(bool inUp, bool inDown);
        void onLButtonDown(int inX, int inY);
        void onLButtonUp(int inX, int inY);
        void onRButtonDown(int inX, int inY);
        void onRButtonUp(int inX, int inY);

        void onButtonPress(int inID);

        Module* mNextModule;

        TextBox* mMapSize[2];

        string mSelectedMap;
};

#endif // LOADMAPMODULE_H
