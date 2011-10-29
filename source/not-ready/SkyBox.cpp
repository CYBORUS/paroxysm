#include "SkyBox.h"
#include <cmath>

SkyBox::SkyBox()
{
    GLfloat vertices[8 * 3] = {
        // top triangles
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f
        };

    mVertexVBO.loadData(vertices, 8, 3);
    mClusterVBO.mount(mVertexVBO, 0);

    GLuint indices[12 * 3] = {
        0, 1, 2, 0, 2, 3, // top
        7, 6, 5, 7, 5, 4, // bottom
        1, 0, 4, 1, 4, 5, // right
        3, 2, 6, 3, 6, 7, // left
        2, 1, 5, 2, 5, 6, // front
        0, 3, 7, 0, 7, 4  // back
        };

    mIndexVBO.loadData(indices, 12, 3);
    mClusterVBO.mount(mIndexVBO);

    const GLfloat a = 1.0f / sqrt(3.0f);

    GLfloat normals[8 * 3] = {
        a, a, a,
        a, -a, a,
        -a, -a, a,
        -a, a, a,
        a, a, -a,
        a, -a, -a,
        -a, -a, -a,
        -a, a, -a,
        };

    mTextureVBO.loadData(normals, 8, 3);
    mClusterVBO.mount(mTextureVBO, 1);
}

SkyBox::~SkyBox()
{
}

void SkyBox::display()
{
    mCubeMap.bind();
    mClusterVBO.display();
}
