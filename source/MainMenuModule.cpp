#include "MainMenuModule.h"
#include "MapEditorModule.h"
#include "GameModule.h"

#include <CGE/Exception.h>

#include <iostream>
#include <fstream>
using namespace std;

static const float SpinStep = 3.1415926535898f / 40.0f;

MainMenuModule::MainMenuModule() : mFont("data/fonts/DejaVuSans.ttf", 24),
    mTextBox(NULL), mCount(0.0f)
{
}

 MainMenuModule::~MainMenuModule()
{
}

void MainMenuModule::onMouseMove(int inX, int inY, int inRelX, int inRelY,
    bool inLeft, bool inRight, bool inMiddle)
{
    mUI.onMouseMove(inX, inY);
}

void MainMenuModule::onRButtonUp(int inX, int inY)
{
}

void MainMenuModule::onRButtonDown(int inX, int inY)
{
}

void MainMenuModule::onLButtonUp(int inX, int inY)
{
    mUI.onMouseUp();
}

void MainMenuModule::onLButtonDown(int inX, int inY)
{
    mUI.onMouseDown();
}

void MainMenuModule::onPulse()
{
    if (mTextBox)
    {
        mCount += 1.0f;

        if (mCount >= 80.0f) mCount -= 80.0f;

        float n = mCount * SpinStep;
        float s = (sin(n) + 1.0f) / 2.0f;
        float c = (cos(n) + 1.0f) / 2.0f;

        mTextBox->setPosition(0.0f, -2.0f, s, c);
    }

    mUI.update();
}

void MainMenuModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mUI.display();
}

void MainMenuModule::onClose()
{
}

void MainMenuModule::onOpen()
{
}

void MainMenuModule::onUnload()
{

}

void MainMenuModule::onLoad(CGE::PropertyList& inList)
{
    // MapEditorModule Button
    CGE::Button* button = new CGE::Button("data/images/hud/map_editor.png",
        4.0f, 1.0f);
    button->setCallback(CGE::Widget::MouseClick, mapEditorButtonCallBack, this);
    button->setPosition(0.0f, -0.5f);
    mUI.addWidget(button);

    // NewGameModule Button
    button = new CGE::Button("data/images/hud/new_game.png", 4.0f, 1.0f);
    button->setCallback(CGE::Widget::MouseClick, newGameButtonCallBack, this);
    button->setPosition(0.0f, 0.5f);
    mUI.addWidget(button);

    // Game Logo Label
    CGE::Label* label = new CGE::Label("data/images/title.png", 8.0f, 2.0f);
    label->setPosition(0.0f, 2.5f);
    mUI.addWidget(label);

    // Text box test
    CGE::TextBox* textBox = new CGE::TextBox();
    textBox->setText(mFont, "Top");
    textBox->setPosition(-3.0f, -2.0f, CGE::TextBox::Top, CGE::TextBox::Center);
    mUI.addWidget(textBox);

    textBox = new CGE::TextBox();
    textBox->setText(mFont, "Middle");
    textBox->setPosition(0.0f, -2.0f, CGE::TextBox::Middle,
        CGE::TextBox::Center);
    textBox->setCallback(CGE::Widget::MouseClick, logoSpinCallback, this);
    mUI.addWidget(textBox);

    textBox = new CGE::TextBox();
    textBox->setText(mFont, "Bottom");
    textBox->setPosition(3.0f, -2.0f, CGE::TextBox::Bottom,
        CGE::TextBox::Center);
    mUI.addWidget(textBox);

    textBox = new CGE::TextBox();
    textBox->setText(mFont, "Left");
    textBox->setPosition(3.0f, 0.0f, CGE::TextBox::Middle,
        CGE::TextBox::Left);
    mUI.addWidget(textBox);

    textBox = new CGE::TextBox();
    textBox->setText(mFont, "Center");
    textBox->setPosition(3.0f, 0.6f, CGE::TextBox::Middle,
        CGE::TextBox::Center);
    mUI.addWidget(textBox);

    textBox = new CGE::TextBox();
    textBox->setText(mFont, "Right");
    textBox->setPosition(3.0f, 1.2f, CGE::TextBox::Middle,
        CGE::TextBox::Right);
    mUI.addWidget(textBox);
}

void MainMenuModule::mapEditorButtonCallBack(CGE::Widget* inWidget,
    void* inData)
{
    MainMenuModule* m = static_cast<MainMenuModule*>(inData);
    m->mNextModule = new MapEditorModule;
    m->mRunning = false;
    m->mDead = false;
}

void MainMenuModule::newGameButtonCallBack(CGE::Widget* inWidget, void* inData)
{
    MainMenuModule* m = static_cast<MainMenuModule*>(inData);
    m->mNextModule = new GameModule;
    m->mRunning = false;
    m->mDead = false;
}

void MainMenuModule::logoSpinCallback(CGE::Widget* inWidget, void* inData)
{
    MainMenuModule* m = static_cast<MainMenuModule*>(inData);
    m->mTextBox = m->mTextBox ? NULL : static_cast<CGE::TextBox*>(inWidget);
}
