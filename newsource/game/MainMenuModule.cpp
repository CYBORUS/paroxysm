#include "MainMenuModule.h"


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

}

/** @brief onLButtonDown
  *
  * @todo: Mouse Left Button Down Event
  */
void MainMenuModule::onLButtonDown(int inX, int inY)
{

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
void MainMenuModule::onLoad()
{

    Button* button = new Button("assets/images/hud/load_map.png", 2.0f, 1.0f);
    button->setClickListener(uiLoadMap, this);
    button->setPosition(3.0f, -2.0f);
    mUI.addWidget(button);

}

/** @brief ~MainMenuModule
  *
  * @todo: Destructor
  */

 MainMenuModule::~MainMenuModule()
{

}
