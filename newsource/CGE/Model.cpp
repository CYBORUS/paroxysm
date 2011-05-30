#include "Model.h"

using namespace std;
using namespace CGE;

//Model::Model()
//{
//    //ctor
//}

Model::~Model()
{
    //dtor
    delete mVBO;
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

    CGE::VertexBufferObject* buffers[4] = {NULL};
    int clusterSize = 2;


    int size = c3m->vertices.size;
    GLfloat* data = c3m->vertices.array;

    //mVBO.loadVertexArray(PVBO_VERTEX, 3, size, data);

    buffers[0] = new VertexBufferObject();
    buffers[0]->loadData(data, size, 3);



    size = c3m->normals.size;
    data = c3m->normals.array;

    //mVBO.loadVertexArray(PVBO_NORMAL, 3, size, data);

    buffers[1] = new VertexBufferObject();
    buffers[1]->loadData(data, size, 3);

    size = c3m->colors.size;
    if (size > 0)
    {
        ++clusterSize;
        data = c3m->colors.array;
        //mVBO.loadVertexArray(PVBO_COLOR, 4, size, data);
        buffers[2] = new VertexBufferObject();
        buffers[2]->loadData(data, size, 4);
    }


    size = c3m->textureCoordinates.size;
    if (size > 0)
    {
        ++clusterSize;
        data = c3m->textureCoordinates.array;
        //mVBO.loadVertexArray(PVBO_TEXTURE, 2, size, data);
        buffers[3] = new VertexBufferObject();
        buffers[3]->loadData(data, size, 2);

        //glGenTextures(1, &mTexture);

        string texFile = "assets/images/models/";
        texFile += c3m->textureFile;

        //DisplayEngine::loadTexture(texFile.c_str(), mTexture);
        mTexture.loadImage(texFile.c_str());
    }
    else
    {
        cerr << "error, no texture" << endl;
    }

    mVBO = new ClusterVBO(clusterSize);
    mVBO->mount(*buffers[0], 0);
    mVBO->mount(*buffers[1], 1);

    //just in case there aren't any colors defined
    int nextIndex = 2;

    if (buffers[2] != NULL)
    {
        mVBO->mount(*buffers[2], nextIndex);
        ++nextIndex;
    }

    if (buffers[3] != NULL)
    {
        mVBO->mount(*buffers[3], nextIndex);
    }


    size = c3m->indices.size;
    GLuint* indices = c3m->indices.array;
    //mVBO.loadIndexArray(GL_TRIANGLES, size, indices);
    CGE::IndexVBO* index = new IndexVBO();
    index->loadData(indices, size, 3);
    mVBO->mount(*index);

    c3mClose(c3m);


}
