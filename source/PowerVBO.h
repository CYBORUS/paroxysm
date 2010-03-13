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

#ifndef POWERVBO_H
#define POWERVBO_H

#include "OGL.h"

#include <vector>
using namespace std;

#define PVBO_VERTEX  0x0001
#define PVBO_NORMAL  0x0002
#define PVBO_TEXTURE 0x0004
#define PVBO_COLOR   0x0008

struct PowerVertexArray
{
    GLuint binding;
    GLuint type;
    GLuint valuesPerVertex;
};

struct PowerIndexArray
{
    GLuint binding;
    GLenum drawMode;
    GLsizei size;
};

class PowerVBO
{
    public:
        PowerVBO();
        ~PowerVBO();

        void loadVertexArray(GLuint inType, GLuint inValuesPerVertex,
            GLuint inSize, GLfloat* inData, GLenum inUsage = GL_STATIC_DRAW);
        void loadIndexArray(GLenum inDrawMode, GLsizei inSize, GLuint* inData,
            GLenum inUsage = GL_STATIC_DRAW);
        void display();

    private:
        GLuint mTypes;
        vector<PowerVertexArray> mVertices;
        vector<PowerIndexArray> mIndices;
};

#endif
