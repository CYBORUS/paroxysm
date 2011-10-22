#include "MainMenuModule.h"
#include <CGE/Exception.h>
#include "MapEditorModule.h"
#include "GameModule.h"

#include <iostream>
#include <fstream>
using namespace std;

/** @brief MainMenuModule
  *
  * @todo: Constructor
  */
MainMenuModule::MainMenuModule()
{
}

/** @brief ~MainMenuModule
  *
  * @todo: Destructor
  */
 MainMenuModule::~MainMenuModule()
{

}

/** @brief onMouseMove
  *
  * @todo: Mouse movement events
  */
void MainMenuModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
    bool inLeft, bool inRight, bool inMiddle)
{
    mUI.onMouseMove(inX, inY);
}

/** @brief onRButtonUp
  *
  * @todo: Mouse Right Button Up Event
  */
void MainMenuModule::onRButtonUp(int inX, int inY)
{
}

/** @brief onRButtonDown
  *
  * @todo: Mouse Right Button Down Event
  */
void MainMenuModule::onRButtonDown(int inX, int inY)
{
}

/** @brief onLButtonUp
  *
  * @todo: Mouse Left Button Up Event
  */
void MainMenuModule::onLButtonUp(int inX, int inY)
{
    mUI.onMouseUp();
}

/** @brief onLButtonDown
  *
  * @todo: Mouse Left Button Down Event
  */
void MainMenuModule::onLButtonDown(int inX, int inY)
{
    mUI.onMouseDown();
}

/** @brief onPulse
  *
  * @todo: Pulse Event
  */
void MainMenuModule::onPulse()
{
    mUI.update();
}

/** @brief onLoop
  *
  * @todo: Loop Event
  */
void MainMenuModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mUI.display();
}

/** @brief onClose
  *
  * @todo: Close Event
  */
void MainMenuModule::onClose()
{
}

/** @brief onOpen
  *
  * @todo: Open Event
  */
void MainMenuModule::onOpen()
{
}

/** @brief onUnload
  *
  * @todo: Unload Event
  */
void MainMenuModule::onUnload()
{

}

/** @brief onLoad
  *
  * @todo: Load Event
  */
void MainMenuModule::onLoad(CGE::PropertyList& inList)
{
    // MapEditorModule Button
    Button* button = new Button("assets/images/hud/map_editor.png", 4.0f, 1.0f);
    button->setClickListener(mapEditorButtonCallBack, this);
    button->setPosition(0.0f, 1.5f);
    mUI.addWidget(button);

    // NewGameModule Button
    button = new Button("assets/images/hud/new_game.png", 4.0f, 1.0f);
    button->setClickListener(newGameButtonCallBack, this);
    button->setPosition(0.0f, 0.5f);
    mUI.addWidget(button);

    // Game Logo Label
    Label* label = new Label("assets/images/title.png", 8.0f, 2.0f);
    label->setPosition(0.0f, 3.0f);
    mUI.addWidget(label);
}

/** @brief mapEditorButtonCallBack
  *
  * @todo: Click Listener Function for NewGameButton
  */
void MainMenuModule::mapEditorButtonCallBack(Widget* inWidget, void* inData)
{
    MainMenuModule* m = reinterpret_cast<MainMenuModule*>(inData);
    m->mNextModule = new MapEditorModule;
    m->mRunning = false;
    m->mDead = false;
}

/** @brief newGameButtonCallBack
  *
  * @todo: Click Listener Function for NewGameButton
  */
void MainMenuModule::newGameButtonCallBack(Widget* inWidget, void* inData)
{
    MainMenuModule* m = reinterpret_cast<MainMenuModule*>(inData);
    m->mNextModule = new GameModule;
    m->mRunning = false;
    m->mDead = false;
}
