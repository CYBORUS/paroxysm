#ifndef TERRAINGRID_H
#define TERRAINGRID_H

#include <CGE/VertexArrayObject.h>
#include <CGE/IndexVBO.h>
#include <iostream>

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

    private:
        size_t toIndex(size_t inRow, size_t inCol);
        void create(size_t inRows, size_t inCols);
        void buildVBO();
        void destroy();

        CGE::VertexBufferObject mVertexVBO;
        CGE::VertexBufferObject mTextureVBO;
        CGE::IndexVBO mIVBO;

        float* mHeights;
        size_t mRows;
        size_t mCols;
        size_t mSize;
};

#endif
