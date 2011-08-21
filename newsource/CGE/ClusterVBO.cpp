#include "ClusterVBO.h"

namespace CGE
{
    ClusterVBO::ClusterVBO(size_t inSize) : mSize(inSize), mIVBO(NULL)
    {
        if (!mSize) mSize = 1;
        mBuffers = new VertexBufferObject*[mSize];
    }

    ClusterVBO::~ClusterVBO()
    {
        delete [] mBuffers;
    }

    void ClusterVBO::mount(const IndexVBO& inIVBO)
    {
        mIVBO = &inIVBO;
    }

    void ClusterVBO::mount(VertexBufferObject& inVBO, size_t inIndex)
    {
        if (inIndex >= mSize) return;

        mBuffers[inIndex] = &inVBO;
    }

    void ClusterVBO::display(GLenum inMode)
    {
        display(*mIVBO, inMode);
    }

    void ClusterVBO::display(const IndexVBO& inIVBO, GLenum inMode)
    {
        for (size_t i = 0; i < mSize; ++i)
            mBuffers[i]->enableVAA(i);

        inIVBO.draw(inMode);

        for (size_t i = 0; i < mSize; ++i)
            mBuffers[i]->disableVAA();
    }

    void ClusterVBO::display(GLenum inMode, GLint inFirst, GLsizei inCount)
    {
        for (size_t i = 0; i < mSize; ++i)
            mBuffers[i]->enableVAA(i);

        glDrawArrays(inMode, inFirst, inCount);

        for (size_t i = 0; i < mSize; ++i)
            mBuffers[i]->disableVAA();
    }

    void ClusterVBO::displayLinear(GLenum inMode)
    {
        for (size_t i = 0; i < mSize; ++i)
            mBuffers[i]->enableVAA(i);

        glDrawArrays(inMode, 0, mBuffers[0]->mSize);

        for (size_t i = 0; i < mSize; ++i)
            mBuffers[i]->disableVAA();
    }
}
