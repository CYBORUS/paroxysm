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
#include "NetworkStream.h"
#include "GameServer.h"

#include "OGL.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_net.h>
#include <boost/filesystem.hpp>

#if SDL_IMAGE_PATCHLEVEL == 9
// SDL_image 1.2.9 has a bug that prevents it from loading more than one image.
#error Do not use SDL_image 1.2.9
#endif

#include <list>
#include <fstream>

Surface DisplayEngine::mDisplay = NULL;
Surface DisplayEngine::mWindowIcon = NULL;
SDL_Rect** DisplayEngine::mModes = NULL;
int DisplayEngine::mMipmapping = 0;
ColorMask DisplayEngine::mMask;
unsigned int DisplayEngine::mFPS = 0;
LogFile DisplayEngine::mLogFile;

void DisplayEngine::start(Module* inModule)
{
    if (inModule == NULL)
    {
        cleanup();
        return;
    }

    Module* currentModule = inModule;

    list<Module*> moduleStack;

    unsigned nextFrame = SDL_GetTicks() + FRAME_LENGTH;
    SDL_Event event;

    /// begin network code

    GameServer server;
    server.start(100);

    NetworkStream client;
    client.connect("127.0.0.1", Config::get<Uint16>("server port", 9421));

    /// end network initialization

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

        currentModule->onOpen();

        unsigned int nextSecond = SDL_GetTicks() + 1000u;
        int framesPerSecond = 0;
        mFPS = 0; //setup the frames per second

        while (currentModule->isRunning())
        {
            while (SDL_PollEvent(&event)) currentModule->onEvent(&event);

            unsigned int ticks = SDL_GetTicks();

            if (ticks > nextSecond)
            {
                nextSecond += 1000u;

                //store the # of frames printed this second
                mFPS = framesPerSecond;
                framesPerSecond = 0;

                client.sendData("cdaragorn", 10);
                client.sendData("TheBuzzSaw", 11);
            }

            if (ticks > nextFrame)
            {
                currentModule->onFrame();
                nextFrame += FRAME_LENGTH;
            }
            else
            {
                currentModule->onLoop();
                SDL_GL_SwapBuffers();
                ++framesPerSecond;
            }

            SDL_Delay(1); // prevent CPU abuse
        }

        currentModule->onClose();

        Module* deadModule = currentModule;
        currentModule = currentModule->next();

        if (deadModule->isDead())
        {
            // LOL @ "if dead module is dead..."
            deadModule->onUnload();
            delete deadModule;
        }
        else
        {
            moduleStack.push_back(deadModule);
            deadModule = NULL;
        }

    }

    server.stopAndWait();

    cleanup();
}

void DisplayEngine::initialize()
{
    using namespace boost::filesystem;

    //delete any old log files
    string logsDir(Config::getUserFolder());
    logsDir += "logs/";

    if (is_directory(logsDir))
    {
        for (directory_iterator itr(logsDir); itr != directory_iterator(); ++itr)
        {
            if (is_regular_file(itr->status()))
            {
                remove(itr->path());
            }
        }
    }

    mLogFile.start("ogl");

    putenv((char*)"SDL_VIDEO_CENTERED=1");

    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
    {
        cerr << "-- error on SDL_Init --" << endl;
        exit(1);
    }

    if (TTF_Init() == -1)
    {
        cerr << "-- error on TTF_Init -- " << TTF_GetError() << endl;
        exit(1);
    }

    if (SDLNet_Init() < 0)
    {
        cerr << "-- error on SDLNet_Init -- " << SDLNet_GetError() << endl;
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

    stringstream logStream;

    int setting;
    /**
     *  All of the SDL OpenGL settings need to be changed from outside the
     *  program. Difficulties with ATI graphics cards seem to result from
     *  improper settings here. This is will allow anyone to tweak settings
     *  appropriately. If the setting file specifies a negative value, its
     *  respective setting is not even set.
     *
     *  The attributes are all listed at
     *  http://www.libsdl.org/cgi/docwiki.cgi/SDL_GLattr
     */

    logStream << "setting SDL OpenGL settings (0 indicates success)" << endl
        << endl;

    #define SET_SDL_SETTING(n) setting = Config::get<int>(#n, -1); \
        if (setting >= 0) { logStream << #n << " -- " \
            << SDL_GL_SetAttribute(n, setting) << endl; }

    SET_SDL_SETTING(SDL_GL_RED_SIZE);
    SET_SDL_SETTING(SDL_GL_GREEN_SIZE);
    SET_SDL_SETTING(SDL_GL_BLUE_SIZE);
    SET_SDL_SETTING(SDL_GL_ALPHA_SIZE);
    SET_SDL_SETTING(SDL_GL_BUFFER_SIZE);
    SET_SDL_SETTING(SDL_GL_DOUBLEBUFFER);
    SET_SDL_SETTING(SDL_GL_DEPTH_SIZE);
    SET_SDL_SETTING(SDL_GL_STENCIL_SIZE);
    SET_SDL_SETTING(SDL_GL_ACCUM_RED_SIZE);
    SET_SDL_SETTING(SDL_GL_ACCUM_GREEN_SIZE);
    SET_SDL_SETTING(SDL_GL_ACCUM_BLUE_SIZE);
    SET_SDL_SETTING(SDL_GL_ACCUM_ALPHA_SIZE);
    SET_SDL_SETTING(SDL_GL_STEREO);
    SET_SDL_SETTING(SDL_GL_MULTISAMPLEBUFFERS);
    SET_SDL_SETTING(SDL_GL_MULTISAMPLESAMPLES);
    SET_SDL_SETTING(SDL_GL_SWAP_CONTROL);
    SET_SDL_SETTING(SDL_GL_ACCELERATED_VISUAL);

    int width = Config::get<int>("display width", 800);
    int height = Config::get<int>("display height", 600);

    mMipmapping = Config::get<int>("mipmapping", 0);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    mMask.red   = 0xff000000;
    mMask.green = 0x00ff0000;
    mMask.blue  = 0x0000ff00;
    mMask.alpha = 0x000000ff;
#else
    mMask.red   = 0x000000ff;
    mMask.green = 0x0000ff00;
    mMask.blue  = 0x00ff0000;
    mMask.alpha = 0xff000000;
#endif


    Uint32 flags = SDL_OPENGL;

    if (Config::get<int>("full screen", 0) == 1) flags |= SDL_FULLSCREEN;

    mDisplay = SDL_SetVideoMode(width, height,
        Config::get<int>("bits per pixel", 24), flags);

    logStream << endl << "current SDL OpenGL settings" << endl << endl;

    int value;

    #define GET_SDL_SETTING(n) SDL_GL_GetAttribute(n, &value); \
        logStream << #n << " : " << value << endl;

    GET_SDL_SETTING(SDL_GL_RED_SIZE);
    GET_SDL_SETTING(SDL_GL_GREEN_SIZE);
    GET_SDL_SETTING(SDL_GL_BLUE_SIZE);
    GET_SDL_SETTING(SDL_GL_ALPHA_SIZE);
    GET_SDL_SETTING(SDL_GL_BUFFER_SIZE);
    GET_SDL_SETTING(SDL_GL_DOUBLEBUFFER);
    GET_SDL_SETTING(SDL_GL_DEPTH_SIZE);
    GET_SDL_SETTING(SDL_GL_STENCIL_SIZE);
    GET_SDL_SETTING(SDL_GL_ACCUM_RED_SIZE);
    GET_SDL_SETTING(SDL_GL_ACCUM_GREEN_SIZE);
    GET_SDL_SETTING(SDL_GL_ACCUM_BLUE_SIZE);
    GET_SDL_SETTING(SDL_GL_ACCUM_ALPHA_SIZE);
    GET_SDL_SETTING(SDL_GL_STEREO);
    GET_SDL_SETTING(SDL_GL_MULTISAMPLEBUFFERS);
    GET_SDL_SETTING(SDL_GL_MULTISAMPLESAMPLES);
    GET_SDL_SETTING(SDL_GL_SWAP_CONTROL);
    GET_SDL_SETTING(SDL_GL_ACCELERATED_VISUAL);
    mLogFile.addLine(logStream.str());

#ifndef __APPLE__
    // OSX does not support window icons
    mWindowIcon = loadImage("assets/images/icon.bmp");
    if (mWindowIcon != NULL)
    {
        SDL_WM_SetIcon(mWindowIcon, NULL);
    }
    else
    {
        cerr << "window icon failed" << endl;
    }
#endif

    if (mMipmapping == 1)
    {
        if (GLEE_ARB_framebuffer_object || GLEE_EXT_framebuffer_object)
        {
            mLogFile.addLine("extension found -- using glGenerateMipmap");
            mMipmapping = 3;
        }
        else
        {
            mLogFile.addLine("extension not found -- using SGI method");
            mMipmapping = 2;
        }
    }

    GLsizei w = SDL_GetVideoSurface()->w;
    GLsizei h = SDL_GetVideoSurface()->h;
    glViewport(0, 0, w, h);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_WM_SetCaption("Paroxysm version 0.1.1","Paroxysm");

    openGLDriverInfo(mLogFile);
}

void DisplayEngine::cleanup()
{
#ifndef __APPLE__
    SDL_FreeSurface(mWindowIcon);
#endif

    SDLNet_Quit();
    TTF_Quit();
    SDL_Quit();
}

Surface DisplayEngine::loadImage(const char* inFile)
{
    Surface t = IMG_Load(inFile);
    Surface outSurface = NULL;

    if (t == NULL)
    {
        //cerr << "failed to load image: " << inFile << endl;
        return NULL;
    }

    outSurface = SDL_DisplayFormatAlpha(t);
    SDL_FreeSurface(t);

    return outSurface;
}

bool DisplayEngine::printErrors(const char* inMessage, ostream& inStream)
{
    GLenum error;
    bool isError = false;

    error = glGetError();

    //gluErrorString(error) ?

    if (error != GL_NO_ERROR)
    {
        inStream << inMessage;
        isError = true;
    }

    while (error != GL_NO_ERROR)
    {
        inStream << gluErrorString(error) << endl;
        /*
        switch(error)
        {
            case GL_INVALID_ENUM:
            {
                inStream << "Invalid enum." << endl;
                break;
            }
            case GL_INVALID_VALUE:
            {
                inStream << "Invalid value." << endl;
                break;
            }
            case GL_INVALID_OPERATION:
            {
                inStream << "Invalid operation." << endl;
                break;
            }
            case GL_STACK_OVERFLOW:
            {
                inStream << "Stack Overflow" << endl;
                break;
            }
            case GL_STACK_UNDERFLOW:
            {
                inStream << "Stack Underflow" << endl;
                break;
            }
            case GL_OUT_OF_MEMORY:
            {
                inStream << "Out of memory." << endl;
                break;
            }
            case GL_TABLE_TOO_LARGE:
            {
                inStream << "Table too large." << endl;
                break;
            }
            default:
            {
                inStream << "unknown error" << endl;
            }
        }
        */

        error = glGetError();
    }
    return isError;
}

bool DisplayEngine::loadTexture(Surface inSurface, GLuint inTexture,
    bool inDelete)
{

    printErrors("Pre-texture errors:\n", mLogFile);

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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (mMipmapping == 2)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, inSurface->w, inSurface->h,
        0, tFormat, GL_UNSIGNED_BYTE, inSurface->pixels);

    if (mMipmapping == 3)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);
    }

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    printErrors("Post-texture errors:\n", mLogFile);


    if (!outSuccess || inDelete) SDL_FreeSurface(inSurface);
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


Point2D<int> DisplayEngine::convert2DObjectToPixel(Point2D<float> inPoint,
    Point2D<int> inDisplay, float inRange)
{
    Point2D<int> center;
    Point2D<int> result;

    center.x = inDisplay.x / 2;
    center.y = inDisplay.y / 2;

    float ratio = (inDisplay.x < inDisplay.y) ? ((float)center.x / inRange)
        : ((float)center.y / inRange);


    result.x = center.x + int(inPoint.x * ratio);
    result.y = center.y - int(inPoint.y * ratio);

    return result;
}


Point2D<float> DisplayEngine::convert2DPixelToObject(Point2D<int> inPoint,
    Point2D<int> inDisplay, float inRange)
{
    Point2D<float> center;
    Point2D<float> result;

    center.x = inDisplay.x / 2;
    center.y = inDisplay.y / 2;
/*
    float ratio = (inDisplay.x < inDisplay.y) ? ((float)center.x / inRange)
        : ((float)center.y / inRange);
*/
    float ratio = (inDisplay.x < inDisplay.y) ? (inRange / (float)center.x)
        : (inRange / (float)center.y);

    result.x = (inPoint.x - center.x) * ratio;
    result.y = (-(inPoint.y - center.y)) * ratio;

    return result;

}

void DisplayEngine::openGLDriverInfo(ostream& inStream)
{
    inStream << "Vendor: " << (char*)glGetString(GL_VENDOR) << endl;
    inStream << "Renderer: " << (char*)glGetString(GL_RENDERER) << endl;
    inStream << "OpenGL Version: " << (char*)glGetString(GL_VERSION) << endl;
    inStream << "\n---[ EXTENSIONS ]---" << endl;

    string stuff = (char*)glGetString(GL_EXTENSIONS);

    for (size_t i = 0; i < stuff.length(); ++i)
    {
        size_t j = stuff.find_first_of(' ', i);
        if (j != string::npos)
        {
            inStream << stuff.substr(i, j - i) << endl;
            i = j;
        }
        else
        {
            inStream << stuff.substr(i) << endl;
            i = stuff.length();
        }
    }
    inStream << endl;
}

int DisplayEngine::processKey(SDLKey inSym, SDLMod inMod)
{
    int c = inSym;
    if (inMod & (KMOD_LSHIFT | KMOD_RSHIFT))
    {
        if (c >= 'a' && c <= 'z')
        {
            c -= 32;
        }
        else
        {
            switch (inSym)
            {
                case SDLK_1: c = '!'; break;
                case SDLK_2: c = '@'; break;
                case SDLK_3: c = '#'; break;
                case SDLK_4: c = '$'; break;
                case SDLK_5: c = '%'; break;
                case SDLK_6: c = '^'; break;
                case SDLK_7: c = '&'; break;
                case SDLK_8: c = '*'; break;
                case SDLK_9: c = '('; break;
                case SDLK_0: c = ')'; break;

                case SDLK_LEFTBRACKET: c = '{'; break;
                case SDLK_RIGHTBRACKET: c = '}'; break;
                case SDLK_SEMICOLON: c = ':'; break;
                case SDLK_QUOTE: c = '"'; break;
                case SDLK_COMMA: c = '<'; break;
                case SDLK_PERIOD: c = '>'; break;
                case SDLK_SLASH: c = '?'; break;
                case SDLK_MINUS: c = '_'; break;
                case SDLK_EQUALS: c = '+'; break;
                case SDLK_BACKQUOTE: c = '~'; break;
                case SDLK_BACKSLASH: c = '|'; break;
                case SDLK_SPACE: c = ' '; break;

                default:
                {
                    c = 0;
                }
            }
        }
    }
    else if (c < 273 && (c < 160 || c > 255))
    {
        switch (inSym)
        {
            case SDLK_KP_PLUS: c = '+'; break;
            case SDLK_KP_DIVIDE: c = '/'; break;
            case SDLK_KP_MULTIPLY: c = '*'; break;
            case SDLK_KP_MINUS: c = '-'; break;
            case SDLK_KP_PERIOD: c = '.'; break;

            case SDLK_KP1:
            case SDLK_KP2:
            case SDLK_KP3:
            case SDLK_KP4:
            case SDLK_KP5:
            case SDLK_KP6:
            case SDLK_KP7:
            case SDLK_KP8:
            case SDLK_KP9:
            case SDLK_KP0:
            {
                c -= 208;
                break;
            }

            case SDLK_PAUSE:
            case SDLK_DELETE:
            case SDLK_BACKSPACE:
            case SDLK_RETURN:
            case SDLK_TAB:
            {
                // the blacklisted keys :)
                c = 0;
                break;
            }

            default:
            {
            }
        }
    }
    else
    {
        c = 0;
    }

    return c;
}
