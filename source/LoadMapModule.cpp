#include "LoadMapModule.h"

bool LoadMapModule::onLoad()
{
    Label* info = new Label("Create a New Map", GENERIC_LABEL);
    info->setFontColor(0.0f, 0.6f, 0.8f, 1.0f);
    info->setFontSize(64);
    info->setLocation(0.0f, 8.0f);
    info->setSize(15.5f, 2.0f);

    mMapSize[0] = new TextBox(X_SIZE);
    mMapSize[0]->setLocation(-1.5f, 6.0f);
    mMapSize[0]->setSize(1.5f, 1.0f);
    mMapSize[0]->setText("");

    mHUD.addWidget(mMapSize[0]);
    mHUD.addWidget(info);

    info = new Label("X", GENERIC_LABEL);
    info->setFontColor(0.0f, 0.6f, 0.8f, 1.0f);
    info->setFontSize(24);
    info->setLocation(0.0f, 6.0f);
    info->setSize(0.45f, 0.75f);

    mHUD.addWidget(info);

    mMapSize[1] = new TextBox(Y_SIZE);
    mMapSize[1]->setLocation(1.5f, 6.0f);
    mMapSize[1]->setSize(1.5f, 1.0f);
    mMapSize[1]->setText("");

    mHUD.addWidget(mMapSize[1]);

    info = new Label("or Load an Existing One", GENERIC_LABEL);
    info->setFontColor(0.0f, 0.6f, 0.8f, 1.0f);
    info->setFontSize(64);
    info->setLocation(0.0f, 0.0f);
    info->setSize(20.0f, 2.0f);

    mHUD.addWidget(info);

    ScrollList* maps = new ScrollList(&mSelectedMap, MAPLIST);
    maps->setLocation(0.0f, -4.0f);
    maps->setSize(20.0f, 5.0f);
    maps->setFontSize(20);

    string mapsDir = "assets/maps";

    //load the list up with all the maps in the maps directory
    if (is_directory(mapsDir))
    {
        for (directory_iterator itr(mapsDir); itr != directory_iterator(); ++itr)
        {
            //cout << itr->path().filename() << ' ';
            if (is_regular_file(itr->status()))
            {
                maps->addListItem(itr->path().filename());
                //cout << " [" << file_size(itr->path()) << ']';
            }
        }
    }
    else
    {
        cout << (exists(mapsDir) ? "Found: " : "Not found: ") << mapsDir << endl;
    }

    Surface someImage = DisplayEngine::loadImage("assets/images/hud/load_map_up_arrow.png");

    maps->setUpArrow(someImage);

    someImage = DisplayEngine::loadImage("assets/images/hud/load_map_down_arrow.png");

    maps->setDownArrow(someImage);

    mHUD.addWidget(maps);

    Button* buttons = new Button("load_map", LOAD_BUTTON);
    buttons->setLocation(-8.0, -8.0);
    buttons->setSize(3.0, 1.5);

    mHUD.addWidget(buttons);

    buttons = new Button("cancel", CANCEL_BUTTON);
    buttons->setLocation(8.0, -8.0);
    buttons->setSize(3.0, 1.5);

    mHUD.addWidget(buttons);
    //mHUD.addWidget(info);

    return true;
}

void LoadMapModule::onCleanup()
{
    //dtor
}

void LoadMapModule::onInit()
{
    mRunning = true;
    mNextModule = NULL;

    GLdouble ratio = 0;
    int w = SDL_GetVideoSurface()->w;
    int h = SDL_GetVideoSurface()->h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    //gluPerspective(FIELD_OF_VIEW, (GLdouble)w / (GLdouble)h, NEAR_CP, FAR_CP);

    //we need to deal with the possibility of a monitor on it's side
    if (w >= h)
    {
        ratio = (GLdouble)w / (GLdouble)h;
        glOrtho(-HUD_RANGE * ratio, HUD_RANGE * ratio, -HUD_RANGE, HUD_RANGE, -10.0, 10.0);
    }
    else
    {
        ratio = (GLdouble)h / (GLdouble)w;
        glOrtho(-HUD_RANGE, HUD_RANGE, -HUD_RANGE * ratio, HUD_RANGE * ratio, 1.0, 10.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Point2D<int> display;
    display.x = w;
    display.y = h;

    mHUD.setDisplay(display);
}

void LoadMapModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT);
    mHUD.display();
}

void LoadMapModule::onFrame()
{
}

Module* LoadMapModule::next()
{
    return mNextModule;
}


void LoadMapModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
    bool inLeft, bool inRight, bool inMiddle)
{
    mHUD.setStates(inX, inY, inLeft);
}

void LoadMapModule::onMouseWheel(bool inUp, bool inDown)
{
}

void LoadMapModule::onLButtonDown(int inX, int inY)
{
}

void LoadMapModule::onLButtonUp(int inX, int inY)
{
}

void LoadMapModule::onRButtonDown(int inX, int inY)
{
}

void LoadMapModule::onRButtonUp(int inX, int inY)
{
}

void LoadMapModule::onButtonPress(int inID)
{
    switch(inID)
    {
        case CANCEL_BUTTON:
        {
            mNextModule = NULL;
            mRunning = false;
            break;
        }

        case LOAD_BUTTON:
        {
            MapEditorModule* map = new MapEditorModule();
            int width = atoi(mMapSize[0]->getText().c_str());
            int height = atoi(mMapSize[1]->getText().c_str());

            if (width <= 0)
            {
                width = 10;
            }

            if (height <= 0)
            {
                height = 10;
            }
            map->setSize(width, height);
            mNextModule = map;
            mRunning = false;
            break;
        }
        default:
        {
        }
    }
}
