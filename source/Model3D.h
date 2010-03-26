#ifndef CYBORUS_MODEL3D_H
#define CYBORUS_MODEL3D_H

#include <iostream>
#include <string>

namespace cyborus
{
    template<class T>
    struct Block
    {
        T* array;
        unsigned int size;

        Block<T>() : array(NULL), size(0) {}
        ~Block<T>() { if (array != NULL && size > 0) delete [] array; }
    };

    class Model3D
    {
        public:
            Model3D(const char* inFile);
            virtual ~Model3D();

            float* getVertexArray();
            unsigned int getVertexArraySize();
            float* getNormalArray();
            unsigned int getNormalArraySize();
            float* getColorArray();
            unsigned int getColorArraySize();
            float* getTextureCoordinateArray();
            unsigned int getTextureCoordinateArraySize();
            unsigned int* getIndexArray();
            unsigned int getIndexArraySize();
            const std::string& getTextureFile();

        private:
            Block<float> mVertices;
            Block<float> mNormals;
            Block<float> mColors;
            Block<float> mTextureCoordinates;
            Block<unsigned int> mIndices;
            std::string mTextureFile;
    };

    inline float* Model3D::getVertexArray()
    {
        return mVertices.array;
    }

    inline unsigned int Model3D::getVertexArraySize()
    {
        return mVertices.size;
    }

    inline float* Model3D::getNormalArray()
    {
        return mNormals.array;
    }

    inline unsigned int Model3D::getNormalArraySize()
    {
        return mNormals.size;
    }

    inline float* Model3D::getColorArray()
    {
        return mColors.array;
    }

    inline unsigned int Model3D::getColorArraySize()
    {
        return mColors.size;
    }

    inline float* Model3D::getTextureCoordinateArray()
    {
        return mTextureCoordinates.array;
    }

    inline unsigned int Model3D::getTextureCoordinateArraySize()
    {
        return mTextureCoordinates.size;
    }

    inline unsigned int* Model3D::getIndexArray()
    {
        return mIndices.array;
    }

    inline unsigned int Model3D::getIndexArraySize()
    {
        return mIndices.size;
    }

    inline const std::string& Model3D::getTextureFile()
    {
        return mTextureFile;
    }
}
#endif
