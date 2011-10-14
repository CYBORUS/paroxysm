#ifndef TERRAINGRID_H
#define TERRAINGRID_H

#include <CGE/OpenGL/VertexArrayObject.h>
#include <CGE/OpenGL/IndexVBO.h>
#include <CGE/OpenGL/ClusterVBO.h>
#include <CGE/OpenGL/Texture2D.h>
#include <CGE/Tools.h>
#include <CGE/Matrix.h>
#include <CGE/Vectors.h>

#include <CGE/Model.h>

#include <iostream>
#include <vector>

class TerrainGrid : public CGE::Model
{
    public:
        TerrainGrid();
        virtual ~TerrainGrid();

        virtual void display();

        friend std::istream& operator>>(std::istream& inStream,
            TerrainGrid& inTerrainGrid);
        friend std::ostream& operator<<(std::ostream& inStream,
            const TerrainGrid& inTerrainGrid);

        void set(int inRow, int inCol, float inHeight, bool inFindNormal);
        float findHeight(float inX, float inY);
        void findNormal(int inRow, int inCol);
        void create(size_t inRows, size_t inCols);
        void buildVBO();

        inline const Matrix<float>& getMatrix() const { return mHeights; }
        vec4f getVertex(int inRow, int inCol);

    private:
        size_t toIndex(size_t inRow, size_t inCol);
        void destroy();

//        CGE::ClusterVBO mVBO;
//        CGE::Texture2D mTexture;
//        CGE::VertexBufferObject mVertexVBO;
//        CGE::VertexBufferObject mTextureVBO;
//        CGE::IndexVBO mIVBO;

        Matrix<float> mHeights;
        size_t mRows;
        size_t mCols;
        size_t mSize;
};

#endif
