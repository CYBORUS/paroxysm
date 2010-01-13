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

#include "DisplayEngine.h"
#include "Module.h"
#include "Config.h"

#include <SDL_opengl.h>
#include <SDL_image.h>

#if SDL_IMAGE_PATCHLEVEL == 9
// SDL_image 1.2.9 has a bug that prevents it from loading more than one image.
#error Do not use SDL_image 1.2.9
#endif

#include <iostream>
using namespace std;

Surface DisplayEngine::mDisplay = NULL;
Surface DisplayEngine::mWindowIcon = NULL;
SDL_Rect** DisplayEngine::mModes = NULL;

void DisplayEngine::start(Module* inModule)
{
    if (inModule == NULL) return;
    Module* currentModule = inModule;

    unsigned nextFrame = SDL_GetTicks() + FRAME_LENGTH;
    SDL_Event event;

    while (currentModule != NULL)
    {
        if (!currentModule->onInit())
        {
            cerr << "failed to initialize module\n";
            currentModule = NULL;
            break;
        }

        unsigned int nextSecond = SDL_GetTicks() + 1000u;
        int framesPerSecond = 0;

        while (currentModule->isRunning())
        {
            while (SDL_PollEvent(&event)) currentModule->onEvent(&event);

            currentModule->onLoop();
            ++framesPerSecond;

            unsigned int ticks = SDL_GetTicks();

            if (ticks > nextSecond)
            {
                cout << framesPerSecond << " frames per second" << endl;
                nextSecond += 1000u;
                framesPerSecond = 0;
            }

            if (ticks > nextFrame)
            {
                glClear(GL_COLOR_BUFFER_BIT);
                currentModule->onFrame();
                SDL_GL_SwapBuffers();
                nextFrame += FRAME_LENGTH;
            }

            SDL_Delay(1); // prevent CPU abuse
        }

        Module* deadModule = currentModule;
        currentModule = currentModule->next();

        if (deadModule->isDead())
        {
            // LOL @ "if dead module is dead..."
            deadModule->onCleanup();
            delete deadModule;
        }
    }

    cleanup();
}

void DisplayEngine::initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cerr << "error on SDL_Init" << endl;
        exit(1);
    }

    #ifdef __WIN32__
    // redirect output to screen (instead of text files)
    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);
    #endif

    // get available full screen modes
    mModes = SDL_ListModes(NULL, SDL_FULLSCREEN);

    if (mModes == (SDL_Rect**)0)
    {
        cout << "No modes available!\n";
    }
    else if (mModes == (SDL_Rect**)-1)
    {
        cout << "All resolutions available.\n";
    }
    else
    {
        cout << "-- Available Full Screen Resolutions --\n";
        for (int i = 0; mModes[i]; ++i)
            cout << "  " << mModes[i]->w << " x " << mModes[i]->h << endl;
    }

    int width = Config::get<int>("display width", 800);
    int height = Config::get<int>("display height", 600);

    Uint32 flags = SDL_OPENGL;
    if (Config::get<int>("full screen", 0) == 1) flags |= SDL_FULLSCREEN;
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    mDisplay = SDL_SetVideoMode(width, height,
        Config::get<int>("bits per pixel", 0), flags);

    #ifndef __APPLE__
    // OSX does not support window icons
    //mWindowIcon = SDL_LoadBMP("content/icon.bmp");
    //if (mWindowIcon != NULL) SDL_WM_SetIcon(mWindowIcon, NULL);
    #endif

    SDL_WM_SetCaption("Paroxysm version 0.0.1","Paroxysm");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void DisplayEngine::cleanup()
{
    #ifndef __APPLE__
    //SDL_FreeSurface(mWindowIcon);
    #endif

    SDL_Quit();
}

Surface DisplayEngine::loadImage(const char* inFile)
{
    Surface t = IMG_Load(inFile);
    Surface outSurface = NULL;

    if (t == NULL)
    {
        cerr << "failed to load image: " << inFile << endl;
        return NULL;
    }

    outSurface = SDL_DisplayFormatAlpha(t);
    SDL_FreeSurface(t);

    return outSurface;
}
