#ifndef TERRAINGRID_H
#define TERRAINGRID_H

#include <CGE/ClusterVBO.h>
#include <iostream>

class TerrainGrid
{
    public:
        TerrainGrid();
        ~TerrainGrid();

        void display() const;

        friend std::istream& operator>>(std::istream& inStream,
            TerrainGrid& inTerrainGrid);
        friend std::ostream& operator<<(std::ostream& inStream,
            const TerrainGrid& inTerrainGrid);

    private:
        size_t toIndex(size_t inRow, size_t inCol);
        void create(size_t inRows, size_t inCols);
        void buildVBO();
        void destroy();

        CGE::ClusterVBO<2> mVBO;
        CGE::IndexVBO mIVBO;

        float* mHeights;
        size_t mRows;
        size_t mCols;
        size_t mSize;
};

#endif
