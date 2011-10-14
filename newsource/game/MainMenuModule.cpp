#include "MainMenuModule.h"
#include <CGE/Exception.h>
#include "MapEditorModule.h"

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

/** @brief onMouseMove
  *
  * @todo: Mouse movement events
  */
void MainMenuModule::onMouseMove(int inX, int inY, int inRelX, int inRelY, bool inLeft, bool inRight, bool inMiddle)
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
    //cout << inX << ' ' << inY << endl;
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
    glDisable(GL_BLEND);
}

/** @brief onOpen
  *
  * @todo: Open Event
  */
void MainMenuModule::onOpen()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    //MapEditorModule Button
    Button* button = new Button("assets/images/hud/map_editor.png", 2.0f, 1.0f);
    button->setClickListener(mapEditorButtonCallBack, this);
    button->setPosition(0.0f, 1.5f);
    mUI.addWidget(button);

    //NewGameModule Button
    button = new Button("assets/images/hud/new_game.png", 2.0f, 1.0f);
    button->setClickListener(newGameButtonCallBack, this);
    button->setPosition(0.0f, 0.5f);
    mUI.addWidget(button);

    //Game Logo Label
    Label* label = new Label("assets/images/title.png", 4.0f, 2.0f);
    label->setPosition(0.0f, 3.0f);
    mUI.addWidget(label);

}

/** @brief ~MainMenuModule
  *
  * @todo: Destructor
  */
 MainMenuModule::~MainMenuModule()
{

}

/** @brief mapEditorButtonCallBack
  *
  * @todo: Click Listener Function for NewGameButton
  */
void MainMenuModule::mapEditorButtonCallBack(Widget* inWidget, void* inData)
{
    reinterpret_cast<MainMenuModule*>(inData)->mapEditorButtonPress();
}

/** @brief newGameButtonCallBack
  *
  * @todo: Click Listener Function for NewGameButton
  */
void MainMenuModule::newGameButtonCallBack(Widget* inWidget, void* inData)
{
    reinterpret_cast<MainMenuModule*>(inData)->newGameButtonPress();
}

/** @brief onResize
  *
  * @todo: document this function
  */
void MainMenuModule::onResize(int inWidth, int inHeight)
{
    GLfloat ratio = static_cast<GLfloat>(inWidth)
        / static_cast<GLfloat>(inHeight);

    mUI.onResize(inWidth, inHeight);

    CGE::Matrix4x4<GLfloat> projection;
    projection.perspective(30.0f, ratio, 1.0f, 1000.0f);
    mViewNode.setProjection(projection);
    glViewport(0, 0, inWidth, inHeight);
    glGetIntegerv(GL_VIEWPORT, mViewport);
}

/** @brief mapEditorButtonPress
  *
  * @todo: document this function
  */
void MainMenuModule::mapEditorButtonPress()
{
    mNextModule = new MapEditorModule;
    mRunning = false;
}

/** @brief mapEditorButtonPress
  *
  * @todo: document this function
  */
void MainMenuModule::newGameButtonPress()
{
    //mNextModule = new NewGameModule;
    //mRunning = false;
}
