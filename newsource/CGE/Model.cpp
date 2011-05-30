#include "Model.h"

using namespace std;
using namespace CGE;

namespace CGE
{
    Model::~Model()
    {
    }

    /**
    *   Constructor to load a resource on construction
    */
    Model::Model(const char* inFile) : mClusterVBO(2)
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
            cerr << "error!" << endl;
            return;
        }

        bool useColors = false;
        bool useTexture = false;
        int clusterSize = 2;


        int size = c3m->vertices.size / 3;
        GLfloat* data = c3m->vertices.array;

        //mVBO.loadVertexArray(PVBO_VERTEX, 3, size, data);

        mBuffers[0].loadData(data, size, 3);



        size = c3m->normals.size / 3;
        data = c3m->normals.array;

        //mVBO.loadVertexArray(PVBO_NORMAL, 3, size, data);

        mBuffers[1].loadData(data, size, 3);

        size = c3m->colors.size / 4;

        //somethings wrong, breaking colors
        //size = 0;
        if (size > 0)
        {
            ++clusterSize;
            useColors = true;
            data = c3m->colors.array;
            //mVBO.loadVertexArray(PVBO_COLOR, 4, size, data);

            mBuffers[2].loadData(data, size, 4);
        }


        size = c3m->textureCoordinates.size / 2;
        if (size > 0)
        {
            ++clusterSize;
            useTexture = true;
            data = c3m->textureCoordinates.array;
            //mVBO.loadVertexArray(PVBO_TEXTURE, 2, size, data);

            mBuffers[3].loadData(data, size, 2);

            //glGenTextures(1, &mTexture);

            string texFile = "assets/images/models/";
            texFile += c3m->textureFile;
            cerr << texFile << endl;

            mTexture.loadImage(texFile.c_str());
        }
        else
        {
            cerr << "error, no texture" << endl;
        }

        mClusterVBO.mount(mBuffers[0], 0);
        //mClusterVBO.mount(mBuffers[1], 1);

        //just in case there aren't any colors defined
        //int nextIndex = 2;

        if (useColors)
        {
            //mClusterVBO.mount(mBuffers[2], nextIndex);
            //++nextIndex;
        }

        if (useTexture)
        {
            //mClusterVBO.mount(mBuffers[3], nextIndex);
            mClusterVBO.mount(mBuffers[3], 1);
        }


        size = c3m->indices.size / 3;
        GLuint* indices = c3m->indices.array;
        //mVBO.loadIndexArray(GL_TRIANGLES, size, indices);
    //    CGE::IndexVBO* index = new IndexVBO();
    //    index->loadData(indices, size);
        mIVBO.loadData(indices, size, 3);
        mClusterVBO.mount(mIVBO);

        c3mClose(c3m);


    }

}
