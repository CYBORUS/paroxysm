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

#include "Model3D.h"
#include "Vector3D.h"

#include <iostream>
#include <fstream>
#include <sstream>

map<string, Model3D*> Model3D::mModels;

Model3D* Model3D::load(const char* inFile)
{
    map<string, Model3D*>::iterator i = mModels.find(inFile);

    if (i == mModels.end())
    {
        Model3D* m = new Model3D(inFile);
        mModels[inFile] = m;
        return m;
    }

    return i->second;
}

void Model3D::unloadAll()
{
    for (map<string, Model3D*>::iterator i = mModels.begin();
        i != mModels.end(); ++i)
    {
        delete i->second;
    }

    mModels.clear();
}

Model3D::Model3D(const char* inFile)
{
    vector<GLuint> quadIndices;
    vector<GLuint> triangleIndices;
    vector<GLfloat> vertices;
    vector<GLfloat> normals;
    vector<GLfloat> textureCoords;

    string f("assets/models/");
    f += inFile;

    ifstream modelFile;
    modelFile.open(f.c_str());
    if (modelFile.fail())
    {
        cerr << "failed to open model: " << f << endl;
        return;
    }

    string line;
    getline(modelFile, line);
    while (!modelFile.eof())
    {
        stringstream ss;
        ss << line;

        string key;
        ss >> key;

        //cout << "key = " << key << endl;
        if (key == "v")
        {
            for (int i = 0; i < 3; ++i)
            {
                GLfloat p;
                ss >> p;
                vertices.push_back(p);
            }
        }
        else if (key == "vn")
        {
            Vector3D<GLfloat> v;

            for (int i = 0; i < 3; ++i) ss >> v[i];

            v.normalize();

            for (int i = 0; i < 3; ++i) normals.push_back(v[i]);
        }
        else if (key == "vt")
        {
            for (int i = 0; i < 2; ++i)
            {
                GLfloat p;
                ss >> p;
                textureCoords.push_back(p);
            }
        }
        else if (key == "f")
        {
            GLuint v[4];

            int i = 0; // keep the count outside the for loop
            for (; i < 4 && !ss.fail() && !ss.eof(); ++i)
            {
                ss >> v[i];
                --v[i];
            }

            if (i == 3)
            {
                triangleIndices.push_back(v[0]);
                triangleIndices.push_back(v[1]);
                triangleIndices.push_back(v[2]);
            }
            else if (i == 4)
            {
                quadIndices.push_back(v[0]);
                quadIndices.push_back(v[1]);
                quadIndices.push_back(v[2]);
                quadIndices.push_back(v[3]);
            }
        }
        else
        {
            cerr << "unknown key -- " << key << endl;
        }

        getline(modelFile, line);
    }

    modelFile.close();

    mVBO.loadVertexArray(PVBO_VERTEX, 3, vertices.size(), &vertices[0],
        GL_STATIC_DRAW);

    if (normals.size() > 0)
    {
        mVBO.loadVertexArray(PVBO_NORMAL, 3, normals.size(), &normals[0],
            GL_STATIC_DRAW);
    }

    if (triangleIndices.size() > 0)
    {
        mVBO.loadIndexArray(GL_TRIANGLES, triangleIndices.size(),
            &triangleIndices[0], GL_STATIC_DRAW);
    }

    if (quadIndices.size() > 0)
    {
        mVBO.loadIndexArray(GL_QUADS, quadIndices.size(),
            &quadIndices[0], GL_STATIC_DRAW);
    }

    mModels[inFile] = this;
}

Model3D::~Model3D()
{
}
