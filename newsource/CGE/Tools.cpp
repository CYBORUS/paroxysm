#include "Tools.h"

#include <SDL_image.h>

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

    const GLenum defaultTexParam[] = {
        GL_TEXTURE_WRAP_S, GL_REPEAT,
        GL_TEXTURE_WRAP_T, GL_REPEAT,
        GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR,
        GL_TEXTURE_MAG_FILTER, GL_LINEAR,
        0 };

    const GLenum defaultCubeMapTexParam[] = {
        GL_TEXTURE_MAG_FILTER, GL_LINEAR,
        GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR,
        GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE,
        GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE,
        GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE,
        0 };

    const GLenum cubeFaces[6] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

    void processTexParameter(GLenum inTarget, const GLenum inParameters[])
    {
        for (const GLenum *i = inParameters; *i; i += 2)
            glTexParameteri(inTarget, i[0], i[1]);
    }

    void loadDetails(Surface inSurface, GLint& outColors, GLenum& outFormat)
    {
        outColors = inSurface->format->BytesPerPixel;
        if (outColors == 4)
        {
            if (inSurface->format->Rmask == 0x000000ff)
                outFormat = GL_RGBA;
            else
                outFormat = GL_BGRA;
        }
        else if (outColors == 3)
        {
            if (inSurface->format->Rmask == 0x000000ff)
                outFormat = GL_RGB;
            else
                outFormat = GL_BGR;
        }
        else
        {
            outFormat = 0;
        }
    }

    Surface blankSurface(int inWidth, int inHeight)
    {
        if (inWidth < 1 || inHeight < 1) return NULL;

        Surface t = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, inWidth,
            inHeight, 0, redMask, greenMask, blueMask, alphaMask);
        SDL_SetAlpha(t, SDL_SRCALPHA, 0xff);

        if (!t) return NULL;

        Surface outSurface = SDL_DisplayFormatAlpha(t);
        SDL_FreeSurface(t);
        return outSurface;
    }

    Surface loadImage(const char* inFile)
    {
        Surface t = IMG_Load(inFile);
        if (!t) return NULL;
        Surface outSurface = SDL_DisplayFormatAlpha(t);
        SDL_FreeSurface(t);
        return outSurface;
    }

    void loadTextureFile(const char* inFile, GLtexture inTexture)
    {
        if (!inFile || !*inFile) return;

        Surface s = loadImage(inFile);
        if (!s) return;
        loadTexture(s, inTexture);
        SDL_FreeSurface(s);
    }

    void loadTexture(Surface inSurface, GLtexture inTexture)
    {
        if (!inSurface) return;

        GLint nOfColors;
        GLenum tFormat;
        loadDetails(inSurface, nOfColors, tFormat);
        if (!tFormat) return;

        glBindTexture(GL_TEXTURE_2D, inTexture);

        processTexParameter(GL_TEXTURE_2D, defaultTexParam);

        glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, inSurface->w, inSurface->h,
            0, tFormat, GL_UNSIGNED_BYTE, inSurface->pixels);

        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void loadCubeMapFiles(const char* inFiles[], GLtexture inTexture)
    {
        Surface surfaces[6];

        for (size_t i = 0; i < 6; ++i) surfaces[i] = loadImage(inFiles[i]);

        loadCubeMap(surfaces, inTexture);

        for (size_t i = 0; i < 6; ++i) SDL_FreeSurface(surfaces[i]);
    }

    void loadCubeMap(Surface inSurface[], GLtexture inTexture)
    {
        GLint nOfColors;
        GLenum tFormat;
        loadDetails(inSurface[0], nOfColors, tFormat);
        if (!tFormat) return;

        glBindTexture(GL_TEXTURE_CUBE_MAP, inTexture);

        processTexParameter(GL_TEXTURE_CUBE_MAP, defaultCubeMapTexParam);

        for (size_t i = 0; i < 6; ++i)
        {
            glTexImage2D(cubeFaces[i], 0, nOfColors, inSurface[i]->w,
                inSurface[i]->h, 0, tFormat, GL_UNSIGNED_BYTE,
                inSurface[i]->pixels);
        }

        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}
