#include "LoadGameModule.h"

bool LoadGameModule::onLoad()
{
    ScrollList* maps = new ScrollList(&mSelectedMap, 2.0f, 5.0f, MAPLIST);
    maps->setLocation(0.0f, 3.0f);
    maps->setSize(25.0f, 8.0f);
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


    Label* info = new Label("Load a Map", LOAD_GAME_LABEL);
    info->setFontColor(0.0f, 0.6f, 0.8f, 1.0f);
    info->setFontSize(52);
    info->setLocation(0.0f, 7.0f);
    info->setSize(8.0f, 4.0f);

    mFadeLabel = info;
    mFadeLabel->setFadeRate(0.003f);

    Button* buttons = new Button("load_map", LOAD_BUTTON);
    buttons->setLocation(-8.0, -8.0);
    buttons->setSize(3.0, 1.0);

    mHUD.addWidget(buttons);

    buttons = new Button("cancel", CANCEL_BUTTON);
    buttons->setLocation(8.0, -8.0);
    buttons->setSize(3.0, 1.0);

    mHUD.addWidget(buttons);

    Point2D<int> display;

    display.x = Config::get<int>("display width", 800);
    display.y = Config::get<int>("display height", 600);

    mHUD.addWidget(maps);
    mHUD.addWidget(info);

    return true;
}

void LoadGameModule::onInit()
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
        glOrtho(-GAME_RANGE * ratio, GAME_RANGE * ratio, -GAME_RANGE, GAME_RANGE, -10.0, 10.0);
    }
    else
    {
        ratio = (GLdouble)h / (GLdouble)w;
        glOrtho(-GAME_RANGE, GAME_RANGE, -GAME_RANGE * ratio, GAME_RANGE * ratio, 1.0, 10.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Point2D<int> display;
    display.x = w;
    display.y = h;

    mHUD.setDisplay(display, GAME_RANGE);

}

void LoadGameModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT);
    mHUD.display();
}

void LoadGameModule::onFrame()
{
    //mFadeLabel->fade();
}

void LoadGameModule::onCleanup()
{
}

Module* LoadGameModule::next()
{
    return mNextModule;
}


void LoadGameModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
    bool inLeft, bool inRight, bool inMiddle)
{
    mHUD.setStates(inX, inY, inLeft);
}

void LoadGameModule::onMouseWheel(bool inUp, bool inDown)
{
}

void LoadGameModule::onLButtonDown(int inX, int inY)
{
}


void LoadGameModule::onLButtonUp(int inX, int inY)
{
}


void LoadGameModule::onRButtonDown(int inX, int inY)
{
}


void LoadGameModule::onRButtonUp(int inX, int inY)
{
}

void LoadGameModule::onButtonPress(int inID)
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
            mNextModule = new GameModule(mSelectedMap.c_str());
            mRunning = false;
            break;
        }
        default:
        {
        }
    }
}


