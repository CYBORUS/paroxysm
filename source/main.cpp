/**
 *  This file is part of "Paroxysm".
 *
 *  "Paroxysm" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Paroxysm" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with "Paroxysm".  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Config.h"
#include "DisplayEngine.h"
#include "SoundEngine.h"
#include "MainMenuModule.h"
#include "CollisionEngine.h"

#include <commdlg.h>
using namespace std;

/*
#include <tchar.h>
using namespace std;

BOOL CALLBACK EnumWindowsProc(HWND inHwnd, LPARAM inLParam)
{
    TCHAR winText[400];
    GetWindowTextA(inHwnd, winText, sizeof(winText) / sizeof(winText[0]));
    cerr << "Window " << inHwnd << " text: " << _T(winText) << endl;
    return true;
}
*/

int main(int argc, char** argv)
{
    Config::initialize(argc, argv);
    DisplayEngine::initialize();
    SoundEngine::initialize();
    CollisionEngine::onSetup();

    Config::outputSettings();

    //char fileName[400];
    //tagOFNA* tag;
    //GetOpenFileNameA(tag);
    //cerr << "fileName: " << tag->lpstrFile << endl;
    //EnumDesktopWindows(NULL, EnumWindowsProc, 0);
    //EnumWindows(EnumWindowsProc, 0);
    DisplayEngine::start(new MainMenuModule);
    SoundEngine::cleanup();
    return 0;
}
