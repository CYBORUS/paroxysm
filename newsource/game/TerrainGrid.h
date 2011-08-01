#ifndef TERRAINGRID_H
#define TERRAINGRID_H

#include <CGE/VertexArrayObject.h>
#include <CGE/IndexVBO.h>
#include <CGE/ClusterVBO.h>
#include <CGE/Texture2D.h>
#include <iostream>
#include <CGE/Tools.h>
#include <CGE/Matrix.h>
#include <vector>
#include <CGE/Vectors.h>

class TerrainGrid
{
    public:
        TerrainGrid();
        ~TerrainGrid();

        void display();

        friend std::istream& operator>>(std::istream& inStream,
            TerrainGrid& inTerrainGrid);
        friend std::ostream& operator<<(std::ostream& inStream,
            const TerrainGrid& inTerrainGrid);

        void set(int inRow, int inCol, float inHeight, bool inFindNormal);
        float findHeight(float inX, float inY);
        void findNormal(int inRow, int inCol);
        void create(size_t inRows, size_t inCols);
        void buildVBO();

    private:
        size_t toIndex(size_t inRow, size_t inCol);
        void destroy();

        CGE::ClusterVBO mVBO;
        CGE::Texture2D mTexture;
        CGE::VertexBufferObject mVertexVBO;
        CGE::VertexBufferObject mTextureVBO;
        CGE::IndexVBO mIVBO;

        Matrix<float> mHeights;
        size_t mRows;
        size_t mCols;
        size_t mSize;
};

#endif
