#include "Texture2D.h"
#include "Tools.h"

namespace CGE
{
    Texture2D::Texture2D()
    {
        mTarget = GL_TEXTURE_2D;
    }

    Texture2D::~Texture2D()
    {
    }

    void Texture2D::loadFromSurface(Surface inSurface)
    {
        loadTexture(inSurface, mHandle);
    }

    void Texture2D::loadFromFile(const char* inFile)
    {
        loadTextureFile(inFile, mHandle);
    }
}
