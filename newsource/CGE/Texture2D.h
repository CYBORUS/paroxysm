#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "Texture.h"

namespace CGE
{
    class Texture2D : public Texture
    {
        public:
            Texture2D();
            virtual ~Texture2D();

            void loadFromSurface(Surface inSurface);
            void loadFromFile(const char* inFile);
    };
}

#endif
