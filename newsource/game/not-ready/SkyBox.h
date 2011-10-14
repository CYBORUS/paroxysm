#ifndef SKYBOX_H
#define	SKYBOX_H

#include <CGE/Actor.h>
#include <CGE/ClusterVBO.h>
#include <CGE/CubeMap.h>

class SkyBox : public CGE::Actor
{
public:
    SkyBox();
    virtual ~SkyBox();
    
    virtual void display();
    
private:
    CGE::ClusterVBO mClusterVBO;
    CGE::VertexBufferObject mVertexVBO;
    CGE::VertexBufferObject mTextureVBO;
    CGE::IndexVBO mIndexVBO;
    CGE::CubeMap mCubeMap;
};

#endif
