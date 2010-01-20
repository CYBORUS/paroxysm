#ifndef TSPHERE_H
#define TSPHERE_H

#include <SDL_opengl.h>

#include "Vector3D.h"

class TSphere
{
    public:
        TSphere();
        ~TSphere();

        void display();

        void moveSphere(float inX, float inY, float inZ);

    private:
        void swapColor();
        void drawTriangle(float* inA, float* inB, float* inC);
        void drawTriangle(Vector3D<float>& inA, Vector3D<float>& inB,
            Vector3D<float>& inC);
        void subdivide(float* inA, float* inB, float* inC, int inDepth);

        bool mFill;
        Vector3D<float> mColor[2];
        short mCurrentColor;
        int mDetail;
        GLuint mID;
        static GLfloat mVData[12][3];
        static GLuint mTIndices[20][3];
        Vector3D<float> mTranslation;
        //Vector3D<double>** mRows;
};

#endif
