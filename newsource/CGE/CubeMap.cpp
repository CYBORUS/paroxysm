#include "CubeMap.h"
#include "Tools.h"

namespace CGE
{
    CubeMap::CubeMap()
    {
        mTarget = GL_TEXTURE_CUBE_MAP;
    }

    CubeMap::~CubeMap()
    {
    }

    void CubeMap::loadFromSurfaces(Surface inSurfaces[])
    {
        loadCubeMap(inSurfaces, mHandle);
    }

    void CubeMap::loadFromFiles(const char* inFiles[])
    {
        loadCubeMapFiles(inFiles, mHandle);
    }
}
