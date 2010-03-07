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

#include "OGL.h"

#include <string>
#include <vector>
using namespace std;

/**
 *  This object's primary function is to read in the 3D file format "obj".
 *  The spec is available at <http://en.wikipedia.org/wiki/Obj>.
 */
class Model3D
{
    public:
        Model3D(const char* inFile);
        ~Model3D();

    private:
        vector<GLfloat> mVertices;
        vector<GLfloat> mNormals;
        vector<GLfloat> mTextureCoords;
};

#endif
