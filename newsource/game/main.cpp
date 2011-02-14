#include <CGE/Engine.h>
#include "MapEditorModule.h"

int main(int argc, char** argv)
{
    CGE::Engine::Settings s;
    s.windowTitle = "paroxysm 0.1";
    s.windowTitle2 = "paroxysm";

    CGE::Engine e(s);
    e.manage(new MapEditorModule);

    return 0;
}
