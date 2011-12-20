#include <CGE/Engine.h>
#include <CGE/Exception.h>
#include "MainMenuModule.h"
#include <string>
using namespace std;

int main(int argc, char** argv)
{
    CGE::Engine::Settings s;
    s.windowTitle = "paroxysm 0.2";
    s.windowTitle2 = "paroxysm";
    //s.sound = false;

    CGE::Engine e(s);
    try
    {
        e.manage(new MainMenuModule);
    }
    catch (CGE::Exception& ex)
    {
        cerr << ex.header << '\n' << ex.message << '\n';

        string s;
        getline(cin, s);
    }

    return 0;
}
