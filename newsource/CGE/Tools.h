#ifndef TOOLS_H
#define TOOLS_H

#include "Graphics.h"

namespace CGE
{
    Surface blankSurface(int inWidth, int inHeight);
    Surface loadImage(const char* inFile);
    void loadTextureFile(const char* inFile, GLtexture inTexture);
    void loadTexture(Surface inSurface, GLtexture inTexture);
    void loadCubeMap(Surface inSurface[], GLtexture inTexture);
}

#endif
