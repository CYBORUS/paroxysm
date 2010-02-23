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
bool DisplayEngine::mMipmapping = false;
Mask DisplayEngine::mMask;

void DisplayEngine::start(Module* inModule)
{
    if (inModule == NULL) return;
    Module* currentModule = inModule;

    list<Module*> moduleStack;

    unsigned nextFrame = SDL_GetTicks() + FRAME_LENGTH;
    SDL_Event event;

    while (currentModule != NULL || moduleStack.size() > 0)
    {
        if (currentModule == NULL)
        {
            currentModule = moduleStack.back();
            moduleStack.pop_back();
        }
        else if (!currentModule->onLoad())
        {
            cerr << "module failed to load" << endl;
            currentModule = NULL;
            break;
        }

        currentModule->onInit();


        unsigned int nextSecond = SDL_GetTicks() + 1000u;
        int framesPerSecond = 0;

        while (currentModule->isRunning())
        {
            while (SDL_PollEvent(&event)) currentModule->onEvent(&event);

            currentModule->onLoop();
            SDL_GL_SwapBuffers();
            ++framesPerSecond;

            unsigned int ticks = SDL_GetTicks();

            if (ticks > nextSecond)
            {
                //cout << framesPerSecond << " frames per second" << endl;
                nextSecond += 1000u;
                framesPerSecond = 0;
            }

            if (ticks > nextFrame)
            {
                currentModule->onFrame();
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
        else
        {
            moduleStack.push_back(deadModule);
            deadModule = NULL;
        }

    }

    cleanup();
}

void DisplayEngine::initialize()
{
    putenv((char*)"SDL_VIDEO_CENTERED=1");

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

    cerr << "SDL_GL_ACCELERATED_VISUAL -- "
        << SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) << endl;
    cerr << "SDL_GL_DOUBLEBUFFER -- "
        << SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) << endl;
    //cerr << "SDL_GL_DEPTH_SIZE -- "
        //<< SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16) << endl;

    int width = Config::get<int>("display width", 800);
    int height = Config::get<int>("display height", 600);

    mMipmapping = Config::get<bool>("mipmapping", false);

    if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    {
        mMask.red   = 0xff000000;
        mMask.green = 0x00ff0000;
        mMask.blue  = 0x0000ff00;
        mMask.alpha = 0x000000ff;
    }
    else
    {
        mMask.red   = 0x000000ff;
        mMask.green = 0x0000ff00;
        mMask.blue  = 0x00ff0000;
        mMask.alpha = 0xff000000;
    }


    Uint32 flags = SDL_OPENGL;

    if (Config::get<int>("full screen", 0) == 1) flags |= SDL_FULLSCREEN;

    mDisplay = SDL_SetVideoMode(width, height,
        Config::get<int>("bits per pixel", 32), flags);

    #ifndef __APPLE__
    // OSX does not support window icons
    //mWindowIcon = SDL_LoadBMP("content/icon.bmp");
    //if (mWindowIcon != NULL) SDL_WM_SetIcon(mWindowIcon, NULL);
    #endif

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_WM_SetCaption("Paroxysm version 0.1.1","Paroxysm");

    cerr << "\n\nVendor: " << (char*)glGetString(GL_VENDOR) << endl;
    cerr << "Renderer: " << (char*)glGetString(GL_RENDERER) << endl;
    cerr << "Opengl Version: " << (char*)glGetString(GL_VERSION) << endl;
    string stuff = (char*)glGetString(GL_EXTENSIONS);
    cerr << "\nString size: " << stuff.size() << endl << endl;
    for (unsigned int i = 0; i < stuff.size(); ++i)
    {
        int j = stuff.find_first_of(' ', i);
        cerr << stuff.substr(i, j - i) << endl;
        i = j;
    }
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

void DisplayEngine::printErrors()
{
    GLenum error;

    error = glGetError();

    if (error != GL_NO_ERROR)
    {
        cerr << "Opengl Error: ";
    }

    while (error != GL_NO_ERROR)
    {
        switch(error)
        {
            case GL_INVALID_ENUM:
            {
                cerr << "Invalid enum." << endl;
            }
            case GL_INVALID_VALUE:
            {
                cerr << "Invalid value." << endl;
            }
            case GL_INVALID_OPERATION:
            {
                cerr << "Invalid operation." << endl;
            }
            case GL_STACK_OVERFLOW:
            {
                cerr << "Stack Overflow" << endl;
            }
            case GL_STACK_UNDERFLOW:
            {
                cerr << "Stack Underflow" << endl;
            }
            case GL_OUT_OF_MEMORY:
            {
                cerr << "Out of memory." << endl;
            }
            case GL_TABLE_TOO_LARGE:
            {
                cerr << "Table too large." << endl;
            }
        }

        error = glGetError();
    }
}

bool DisplayEngine::loadTexture(Surface inSurface, GLuint inTexture)
{
    cerr << "Pre-texture errors:\n";

    printErrors();

    bool outSuccess = true;
    if (inSurface == NULL)
    {
        Uint32 flags = SDL_SWSURFACE | SDL_ASYNCBLIT;
        int bits = 0;

        Surface t;
        t = SDL_CreateRGBSurface(flags, 1, 1, bits, mMask.red, mMask.green,
            mMask.blue, mMask.alpha);
        inSurface = SDL_DisplayFormat(t);
        SDL_FreeSurface(t);
        outSuccess = false;
    }

    glBindTexture(GL_TEXTURE_2D, inTexture);

    if (mMipmapping)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 10);
    }

    if (mMipmapping)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);



    GLint nOfColors = inSurface->format->BytesPerPixel;
    GLenum tFormat = GL_RGBA;
    if (nOfColors == 4)
    {
        if (inSurface->format->Rmask == 0x000000ff)
            tFormat = GL_RGBA;
        else
            tFormat = GL_BGRA;
    }
    else if (nOfColors == 3)
    {
        if (inSurface->format->Rmask == 0x000000ff)
            tFormat = GL_RGB;
        else
            tFormat = GL_BGR;
    }
    else
    {
        cerr << "failed to load texture -- not true color\n";
        SDL_FreeSurface(inSurface);
        return false;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, inSurface->w, inSurface->h,
        0, tFormat, GL_UNSIGNED_BYTE, inSurface->pixels);

    cerr << "Post-texture errors:\n";

    printErrors();

    if (!outSuccess) SDL_FreeSurface(inSurface);
    return outSuccess;
}

bool DisplayEngine::loadTexture(const char* inFile, GLuint inTexture)
{
    Surface t = loadImage(inFile);
    if (t == NULL) return false;
    loadTexture(t, inTexture);
    SDL_FreeSurface(t);
    return true;
}


Point2D<int> DisplayEngine::convert2DObjectToPixel(Point2D<float> inPoint, Point2D<int> inDisplay, float inRange)
{
    Point2D<int> center;
    Point2D<int> result;

    center.x = inDisplay.x / 2;
    center.y = inDisplay.y / 2;

    float ratio = (inDisplay.x < inDisplay.y) ? ((float)center.x / inRange) : ((float)center.y / inRange);


    result.x = center.x + int(inPoint.x * ratio);
    result.y = center.y - int(inPoint.y * ratio);

    return result;
}


Point2D<float> DisplayEngine::convert2DPixelToObject(Point2D<int> inPoint, Point2D<int> inDisplay, float inRange)
{
    Point2D<float> center;
    Point2D<float> result;

    center.x = inDisplay.x / 2;
    center.y = inDisplay.y / 2;

    //float ratio = (inDisplay.x < inDisplay.y) ? ((float)center.x / inRange) : ((float)center.y / inRange);

    float ratio = (inDisplay.x < inDisplay.y) ? (inRange / (float)center.x) : (inRange / (float)center.y);

    cerr << "ratio: " << ratio << endl;
    result.x = (inPoint.x - center.x) * ratio;
    result.y = (-(inPoint.y - center.y)) * ratio;

    return result;

}

