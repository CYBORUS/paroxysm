#ifndef SKYBOX_H
#define SKYBOX_H

#include <CGE/Model.h>
#include <CGE/OpenGL/InterleavedVBO.h>
#include <CGE/OpenGL/IndexVBO.h>
#include <CGE/OpenGL/CubeMap.h>
#include <CGE/Image.cpp>

class SkyBox : CGE::Model
{
    public:
        SkyBox();
        virtual ~SkyBox();
    protected:
    private:
        CGE::InterleavedVBO mVBO;
        CGE::IndexVBO mIVBO;
        CGE::CubeMap mCubeMap;
        CGE::Image mImages[6];
};

#endif // SKYBOX_H
