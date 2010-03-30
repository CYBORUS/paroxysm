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

#ifndef MODEL3D_H
#define MODEL3D_H

#include "PowerVBO.h"
#include "Vector3D.h"
#include "DisplayEngine.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct BoundingBox
{
    float minX;
    float minY;
    float minZ;
    float maxX;
    float maxY;
    float maxZ;

    BoundingBox()
    {
        minX = 2000000;
        minY = 2000000;
        minZ = 2000000;
        maxX = -2000000;
        maxY = -2000000;
        maxZ = -2000000;
    }
};

class ModelStack
{
    public:
        ModelStack();
        ~ModelStack();

        void display();

        static ModelStack* load(const char* inFile);
        static void unloadAll();

    private:
        void loadOBJ(const char* inFile);
        void load3DS(const char* inFile);
        void loadC3M(const char* inFile);

        static map<string, ModelStack*> mModels;

        BoundingBox mBoundingBox;
        GLuint mTexture;

        template<class T> static unsigned short readBytes(istream& inStream,
            T& inTarget);

        PowerVBO mVBO;
};

inline void ModelStack::display()
{
    if (glIsTexture(mTexture))
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, mTexture);

        mVBO.display();

        glDisable(GL_TEXTURE_2D);
    }
    else
    {
        mVBO.display();
    }
}

template<class T>
unsigned short ModelStack::readBytes(istream& inStream, T& inTarget)
{
    static const unsigned short size = sizeof(T);
    char* bytes = (char*)(&inTarget);
    inStream.read(bytes, size);
    return size;
}

#endif
