#include "Image.h"

#include <SDL_image.h>

#include <iostream>
using namespace std;

namespace CGE
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const Uint32 redMask   = 0xff000000;
    const Uint32 greenMask = 0x00ff0000;
    const Uint32 blueMask  = 0x0000ff00;
    const Uint32 alphaMask = 0x000000ff;
#else
    const Uint32 redMask   = 0x000000ff;
    const Uint32 greenMask = 0x0000ff00;
    const Uint32 blueMask  = 0x00ff0000;
    const Uint32 alphaMask = 0xff000000;
#endif

    Image::Image(const char* inFile) : mData(NULL), mFormat(0)
    {
        if (inFile) loadFile(inFile);
    }

    Image::Image(int inWidth, int inHeight)
    {
        if (inWidth < 1 || inHeight < 1) return; // TODO: report error

        Surface data = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA,
            inWidth, inHeight, 0, redMask, greenMask, blueMask, alphaMask);
        SDL_SetAlpha(data, SDL_SRCALPHA, 0xff);

        if (!data) return;

        mData = SDL_DisplayFormatAlpha(data);
        SDL_FreeSurface(data);
        findFormat();
    }

    Image::Image(const Image& inImage)
    {
        mData = SDL_DisplayFormatAlpha(inImage.mData);
        findFormat();
    }

    Image::~Image()
    {
        destroy();
    }

    void Image::loadFile(const char* inFile)
    {
        if (!inFile || !*inFile)
        {
            cerr << "Image::loadfile -- invalid file name\n";
            return;
        }

        destroy();

        Surface data = IMG_Load(inFile);
        if (!data)
        {
            cerr << "Image::loadfile -- failed to load file: " << inFile
                << endl;
            return;
        }
        mData = SDL_DisplayFormatAlpha(data);
        SDL_FreeSurface(data);
        findFormat();
    }

    void Image::blitOnto(Image& inImage) const
    {
        if (mData && inImage.mData)
            SDL_BlitSurface(mData, NULL, inImage.mData, NULL);
    }

    void Image::loadIntoTexture(GLenum inTarget) const
    {
        if (!mData)
        {
            cerr << "Image::loadIntoTexture -- invalid image data\n";
            return;
        }

        glTexImage2D(inTarget, 0, mColors, mData->w, mData->h, 0, mFormat,
            GL_UNSIGNED_BYTE, mData->pixels);
    }

    int Image::width() const
    {
        return mData ? mData->w : 0;
    }

    int Image::height() const
    {
        return mData ? mData->h : 0;
    }

    void Image::destroy()
    {
        SDL_FreeSurface(mData); // safe to call on a NULL pointer
        mData = NULL;
        mFormat = 0;
    }

    void Image::findFormat()
    {
        mColors = mData->format->BytesPerPixel;
        if (mColors == 4)
        {
            if (mData->format->Rmask == 0x000000ff)
                mFormat = GL_RGBA;
            else
                mFormat = GL_BGRA;
        }
        else if (mColors == 3)
        {
            if (mData->format->Rmask == 0x000000ff)
                mFormat = GL_RGB;
            else
                mFormat = GL_BGR;
        }
        else
        {
            mFormat = 0;
        }
    }

    void setWindowIcon(const Image& inImage)
    {
        SDL_WM_SetIcon(inImage.mData, NULL);
    }
}
