#include "SkyBox.h"

SkyBox::SkyBox()
{
    mVBO.addField(0,3);
    mVBO.addField(1,3);

    const GLfloat a = 1.0f / sqrt(3.0f);
    //Vertices
    GLfloat data[8 * 6] = {
        1.0f, 1.0f, 1.0f, a, a, a,
        1.0f, -1.0f, 1.0f, a, -a, a,
        -1.0f, -1.0f, 1.0f, -a, -a, a,
        -1.0f, 1.0f, 1.0f, -a, a, a,
        1.0f, 1.0f, -1.0f, a, a, -a,
        1.0f, -1.0f, -1.0f, a, -a, -a,
        -1.0f, -1.0f, -1.0f, -a, -a, -a,
        -1.0f, 1.0f, -1.0f, -a, a, -a
    };

    mVBO.loadData(data, 8);

    GLuint indices[12 * 3] = {
        0, 1, 2, 0, 2, 3, // top
        7, 6, 5, 7, 5, 4, // bottom
        1, 0, 4, 1, 4, 5, // right
        3, 2, 6, 3, 6, 7, // left
        2, 1, 5, 2, 5, 6, // front
        0, 3, 7, 0, 7, 4  // back
        };

    mIVBO.loadData(indices, 12, 3);

    mImages[0].loadFile("data/images/skyboxsun5degTop.png")     // top
    mImages[1].loadFile("data/images/skyboxsun5degBottom.png")  // bottom
    mImages[2].loadFile("data/images/skyboxsun5degRight.png")   // right
    mImages[3].loadFile("data/images/skyboxsun5degLeft.png")    // left
    mImages[4].loadFile("data/images/skyboxsun5degFront.png")   // front
    mImages[5].loadFile("data/images/skyboxsun5degBack.png")    // back

}

SkyBox::~SkyBox()
{
    //dtor
}
