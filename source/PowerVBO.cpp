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

#include "PowerVBO.h"

#include <cstdlib>
#include <iostream>

PowerVBO::PowerVBO() : mTypes(0)
{
}

PowerVBO::~PowerVBO()
{
    for (unsigned int i = 0; i < mVertices.size(); ++i)
        glDeleteBuffers(1, &mVertices[i].binding);

    for (unsigned int i = 0; i < mIndices.size(); ++i)
        glDeleteBuffers(1, &mIndices[i].binding);
}

void PowerVBO::loadVertexArray(GLuint inType, GLuint inValuesPerVertex,
    GLuint inSize, GLfloat* inData, GLenum inUsage)
{
    PowerVertexArray pva;
    pva.type = inType;
    mTypes |= inType;
    pva.valuesPerVertex = inValuesPerVertex;
    glGenBuffers(1, &pva.binding);
    glBindBuffer(GL_ARRAY_BUFFER, pva.binding);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * inSize, inData, inUsage);

    mVertices.push_back(pva);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PowerVBO::loadIndexArray(GLenum inDrawMode, GLsizei inSize, GLuint* inData,
    GLenum inUsage)
{
    PowerIndexArray pia;
    pia.drawMode = inDrawMode;
    pia.size = inSize;
    glGenBuffers(1, &pia.binding);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pia.binding);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * inSize, inData,
        inUsage);

    mIndices.push_back(pia);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PowerVBO::display()
{
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    if (mTypes & PVBO_VERTEX ) glEnableClientState(GL_VERTEX_ARRAY);
    if (mTypes & PVBO_NORMAL ) glEnableClientState(GL_NORMAL_ARRAY);
    if (mTypes & PVBO_TEXTURE) glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    if (mTypes & PVBO_COLOR  ) glEnableClientState(GL_COLOR_ARRAY);

    for (unsigned int i = 0; i < mVertices.size(); ++i)
    {
        PowerVertexArray& pva = mVertices[i];
        glBindBuffer(GL_ARRAY_BUFFER, pva.binding);
        switch (pva.type)
        {
            case PVBO_VERTEX:
            {
                glVertexPointer(pva.valuesPerVertex, GL_FLOAT, 0, 0);
                break;
            }

            case PVBO_NORMAL:
            {
                glNormalPointer(GL_FLOAT, 0, 0);
                break;
            }

            case PVBO_TEXTURE:
            {
                glTexCoordPointer(pva.valuesPerVertex, GL_FLOAT, 0, 0);
                break;
            }

            case PVBO_COLOR:
            {
                glColorPointer(pva.valuesPerVertex, GL_FLOAT, 0, 0);
                break;
            }

            default:
            {
                cerr << "PowerVBO::display() -- invalid array type -- "
                    << pva.type << endl;
                exit(1);
            }
        }
    }

    for (unsigned int i = 0; i < mIndices.size(); ++i)
    {
        PowerIndexArray& pia = mIndices[i];
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pia.binding);
        glDrawElements(pia.drawMode, pia.size, GL_UNSIGNED_INT, 0);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glPopClientAttrib();
}
