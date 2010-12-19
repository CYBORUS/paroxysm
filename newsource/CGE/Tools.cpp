#include "Tools.h"

#include <SDL_image.h>

namespace CGE
{
    Surface loadImage(const char* inFile)
    {
        Surface t = IMG_Load(inFile);
        if (!t) return NULL;
        Surface outSurface = SDL_DisplayFormatAlpha(t);
        SDL_FreeSurface(t);
        return outSurface;
    }
}
