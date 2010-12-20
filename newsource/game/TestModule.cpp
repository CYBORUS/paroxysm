#include "TestModule.h"

#include <CGE/Vector3D.h>
#include <CGE/Tools.h>

#define FOV 30.0f
#define NCC 1.0f
#define FCC 100.0f

GLchar VSSource[] =
    "uniform mat4 MVPM;\n"
    "uniform mat4 MVM;\n"
    "uniform mat4 NM;\n"
    "uniform vec3 lightPosition;\n"
    "\n"
    "attribute vec3 in_Position;\n"
    "attribute vec4 in_Color;\n"
    "attribute vec3 in_Normal;\n"
    "\n"
    "varying vec4 ex_Color;\n"
    "varying vec3 ex_Normal;\n"
    "varying vec3 ex_Texture;\n"
    "varying vec3 ex_LightDir;\n"
    "void main(void)\n"
    "{\n"
    "    vec4 p = vec4(in_Position, 1.0);\n"
    "    \n"
    "    ex_Normal = vec3(NM * vec4(in_Normal, 1.0));\n"
    "    ex_Texture = in_Normal;\n"
    "    \n"
    "    vec4 vPosition4 = MVM * p;\n"
    "    vec3 vPosition3 = vPosition4.xyz / vPosition4.w;\n"
    "    \n"
    "    ex_LightDir = normalize(lightPosition - vPosition3);\n"
    "    \n"
    "    gl_Position = MVPM * p;\n"
    "    ex_Color = in_Color;\n"
    "}\n"
    "\n";

GLchar FSSource[] =
    "uniform vec4 ambientColor;\n"
    "uniform vec4 diffuseColor;\n"
    "uniform vec4 specularColor;\n"
    "uniform samplerCube cubeMap;"
    "\n"
    "varying vec4 ex_Color;\n"
    "varying vec3 ex_Normal;\n"
    "varying vec3 ex_Texture;\n"
    "varying vec3 ex_LightDir;\n"
    "\n"
    "void main(void)\n"
    "{\n"
    "    float diff = max(0.0, dot(normalize(ex_Normal), normalize(ex_LightDir)));\n"
    "    \n"
    "    vec4 lightColor = diff * diffuseColor;\n"
    "    \n"
    "    lightColor += ambientColor;\n"
    "    \n"
    //"    lightColor *= ex_Color;\n"
    "    lightColor *= texture(cubeMap, ex_Texture);\n"
    "    \n"
    "    vec3 vReflection = normalize(reflect(-normalize(ex_LightDir),\n"
    "        normalize(ex_Normal)));\n"
    "    float spec = max(0.0, dot(normalize(ex_Normal), vReflection));\n"
    "    \n"
    "    if (diff != 0)\n"
    "    {\n"
    "        float fSpec = pow(spec, 128.0);\n"
    "        lightColor.rgb += vec3(fSpec, fSpec, fSpec);\n"
    "    }\n"
    "    \n"
    "    gl_FragColor = lightColor;\n"
    "}\n"
    "\n";

GLfloat points[24] = {
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f
};

GLfloat colors[32] = {
    1.0f, 0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

GLuint indices[36] = {
    0, 1, 2, 0, 2, 3, // top
    7, 6, 5, 7, 5, 4, // bottom
    1, 0, 4, 1, 4, 5, // right
    3, 2, 6, 3, 6, 7, // left
    2, 1, 5, 2, 5, 6, // front
    0, 3, 7, 0, 7, 4  // back
};

const GLfloat a = 1.0f / sqrt(3.0f);

GLfloat normals[24] = {
    a, a, a,
    a, -a, a,
    -a, -a, a,
    -a, a, a,
    a, a, -a,
    a, -a, -a,
    -a, -a, -a,
    -a, a, -a,
};

TestModule::TestModule() : mRotate(0.0f)
{
    glGenTextures(1, &mTexture);

    Surface pics[6];
    pics[0] = CGE::loadImage("data/images/icon.bmp");
    for (size_t i = 1; i < 6; ++i) pics[i] = pics[0];
    CGE::loadCubeMap(pics, mTexture);
    SDL_FreeSurface(pics[0]);

    mVS.loadFromBuffer(VSSource, GL_VERTEX_SHADER);
    mFS.loadFromBuffer(FSSource, GL_FRAGMENT_SHADER);
    mProgram.attachShader(mVS);
    mProgram.attachShader(mFS);
    mProgram.bindAttribLocation(0, "in_Position");
    mProgram.bindAttribLocation(1, "in_Color");
    mProgram.bindAttribLocation(2, "in_Normal");
    mProgram.link();
    mUniMVPM = mProgram.getUniformLocation("MVPM");
    mUniMVM = mProgram.getUniformLocation("MVM");
    mUniNM = mProgram.getUniformLocation("NM");

    GLint u;

    u = mProgram.getUniformLocation("cubeMap");
    glUniform1i(u, 0);

    vec4f v(0.1f, 0.1f, 0.1f, 1.0f);
    u = mProgram.getUniformLocation("ambientColor");
    glUniform4fv(u, 1, v.array());

    u = mProgram.getUniformLocation("diffuseColor");
    v[0] = 1.0f;
    v[1] = 1.0f;
    v[2] = 1.0f;
    glUniform4fv(u, 1, v.array());

    u = mProgram.getUniformLocation("specularColor");
    v[0] = 1.0f;
    v[1] = 1.0f;
    v[2] = 1.0f;
    v[3] = 0.0f;
    glUniform4fv(u, 1, v.array());

    u = mProgram.getUniformLocation("lightPosition");
    v[0] = 5.0f;
    v[2] = 100.0f;
    glUniform3fv(u, 1, v.array());

    mVBO.loadVAA(0, 3, 8, points);
    mVBO.loadVAA(1, 4, 8, colors);
    mVBO.loadVAA(2, 3, 8, normals);
    mIVBO.loadData(GL_TRIANGLES, 36, indices);
}

TestModule::~TestModule()
{
    glDeleteTextures(1, &mTexture);
}

void TestModule::onOpen()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture);

    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
}

void TestModule::onClose()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

void TestModule::onFrame()
{
    mRotate += 1.0f;
    if (mRotate > 180.0f) mRotate -= 360.0f;
}

void TestModule::onLoop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mModelView.loadIdentity();
    mModelView.translate(0.0f, 0.0f, -10.0f);
    mModelView.rotateX(mRotate);
    mModelView.rotateZ(mRotate);

    mNormalView.loadIdentity();
    mNormalView.rotateX(mRotate);
    mNormalView.rotateZ(mRotate);

    mat4f mvp(mProjection, mModelView);
    glUniformMatrix4fv(mUniMVPM, 1, GL_FALSE, mvp);
    glUniformMatrix4fv(mUniMVM, 1, GL_FALSE, mModelView);
    glUniformMatrix4fv(mUniNM, 1, GL_FALSE, mNormalView);
    mVBO.display(mIVBO);
}

void TestModule::onResize(int inWidth, int inHeight)
{
    float ratio = static_cast<float>(inWidth) / static_cast<float>(inHeight);
    mProjection.loadIdentity();
    mProjection.perspective(FOV, ratio, NCC, FCC, true);
    glViewport(0, 0, inWidth, inHeight);
}
