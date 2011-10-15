#include <CGE/Engine.h>
#include "MapEditorModule.h"
#include "MainMenuModule.h"

#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    CGE::Engine::Settings s;
    s.windowTitle = "paroxysm 0.1";
    s.windowTitle2 = "paroxysm";
    s.sound = false;

    CGE::Engine e(s);
    e.manage(new MainMenuModule);
    //e.manage(new MapEditorModule);

    return 0;
}
