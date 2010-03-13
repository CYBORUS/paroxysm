/**
 *  This file is part of "Paroxysm".
 *
 *  "Paroxysm" is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  "Paroxysm" is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with "Paroxysm".  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TSPHERE_H
#define TSPHERE_H

#include "OGL.h"
#include "Vector3D.h"
#include "PowerVBO.h"

#include <vector>

class TSphere
{
    public:
        TSphere(int inDetail = 1);
        ~TSphere();

        void display();

        void moveSphere(float inX, float inY, float inZ);
        void setColor(float inR, float inG, float inB);
        void setScale(float inX, float inY, float inZ);

    private:
        void swapColor();
        void drawTriangle(float* inA, float* inB, float* inC);
        void drawTriangle(Vector3D<float>& inA, Vector3D<float>& inB,
            Vector3D<float>& inC);
        void subdivide(float* inA, float* inB, float* inC, int inDepth);

        bool mFill;
        Vector3D<float> mColor[2];
        Vector3D<float> mScale;
        short mCurrentColor;
        int mDetail;
        static GLfloat mVData[12][3];
        static GLuint mTIndices[20][3];
        Vector3D<float> mTranslation;

        PowerVBO mVBO;
        vector<GLuint> mIndices;
        vector<GLfloat> mVertices;
};

#endif
