#include "Texture.h"

namespace CGE
{
    Texture::Texture()
    {
        glGenTextures(1, &mHandle);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &mHandle);
    }

}
