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
#include <map>
using namespace std;

#define M3D_VBO_COUNT 4
#define M3D_TRIANGLES 0
#define M3D_QUADS 1
#define M3D_VERTICES 2
#define M3D_NORMALS 3

/**
 *  This object's primary function is to read in the 3D file format "obj".
 *  The spec is available at <http://en.wikipedia.org/wiki/Obj>.
 */
class Model3D
{
    public:
        ~Model3D();

        void display();

        static Model3D* load(const char* inFile);
        static void unloadAll();

    private:
        Model3D(const char* inFile);

        static map<string, Model3D*> mModels;

        GLuint mVBO[M3D_VBO_COUNT];
        struct {
            bool normals;
            bool textures;
            unsigned int quads;
            unsigned int triangles;
        } mActive;
};

#endif
