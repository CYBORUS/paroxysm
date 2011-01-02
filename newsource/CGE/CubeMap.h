#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "Texture.h"

namespace CGE
{
    class CubeMap : public Texture
    {
        public:
            CubeMap();
            virtual ~CubeMap();

            void loadFromSurfaces(Surface inSurfaces[]);
            void loadFromFiles(const char* inFiles[]);
    };
}

#endif
