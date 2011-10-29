#include <CGE/Engine.h>
#include "MainMenuModule.h"

int main(int argc, char** argv)
{
    CGE::Engine::Settings s;
    s.windowTitle = "paroxysm 0.2";
    s.windowTitle2 = "paroxysm";
    s.sound = false;

    CGE::Engine e(s);
    e.manage(new MainMenuModule);

    return 0;
}
