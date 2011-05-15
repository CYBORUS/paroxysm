#include "Model.h"

Model::Model()
{
    //ctor
}

Model::~Model()
{
    //dtor
}

/**
*   Constructor to load a resource on construction
*/
Model::Model(const char* inFile)
{
    string s(inFile);
    s = s.substr(s.rfind('.') + 1);

    if (s == "c3m")
    {
        loadC3M(inFile);
    }
    else
    {
        //error
    }
}


void Model::loadC3M(const char* inFile)
{
    string file = "assets/models/";
    file += inFile;
    c3mModel* c3m = c3mOpen(file.c_str());
    if (c3mError)
    {
        //DisplayEngine::printErrors("c3m loader error");
        return;
    }


    int size = c3m->vertices.size;
    GLfloat* data = c3m->vertices.array;

    mVBO.loadVertexArray(PVBO_VERTEX, 3, size, data);


    size = c3m->normals.size;
    data = c3m->normals.array;

    mVBO.loadVertexArray(PVBO_NORMAL, 3, size, data);

    size = c3m->colors.size;
    if (size > 0)
    {
        data = c3m->colors.array;
        mVBO.loadVertexArray(PVBO_COLOR, 4, size, data);
    }


    size = c3m->textureCoordinates.size;
    if (size > 0)
    {
        data = c3m->textureCoordinates.array;
        mVBO.loadVertexArray(PVBO_TEXTURE, 2, size, data);

        glGenTextures(1, &mTexture);

        string texFile = "assets/images/models/";
        texFile += c3m->textureFile;

        DisplayEngine::loadTexture(texFile.c_str(), mTexture);
    }
    else
    {
        cerr << "error, no texture" << endl;
    }


    size = c3m->indices.size;
    GLuint* indices = c3m->indices.array;
    mVBO.loadIndexArray(GL_TRIANGLES, size, indices);

    c3mClose(c3m);


}
