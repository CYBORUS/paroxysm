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

    glGenBuffers(M3D_VBO_COUNT, mVBO);

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
            for (; i < 4 && !ss.fail() && !ss.eof(); ++i) ss >> v[i];

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

    glBindBuffer(GL_ARRAY_BUFFER, mVBO[M3D_VERTICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(),
        &vertices[0], GL_DYNAMIC_DRAW);

    mActive.normals = false;
    if (normals.size() > 0)
    {
        mActive.normals = true;
        glBindBuffer(GL_ARRAY_BUFFER, mVBO[M3D_NORMALS]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normals.size(),
            &normals[0], GL_DYNAMIC_DRAW);
    }

    mActive.triangles = triangleIndices.size();
    if (mActive.triangles > 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBO[M3D_TRIANGLES]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)
            * triangleIndices.size(), &triangleIndices[0], GL_DYNAMIC_DRAW);
    }

    mActive.quads = quadIndices.size();
    if (mActive.quads > 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBO[M3D_QUADS]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)
            * quadIndices.size(), &quadIndices[0], GL_DYNAMIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    mModels[inFile] = this;
}

Model3D::~Model3D()
{
    glDeleteBuffers(M3D_VBO_COUNT, mVBO);
}

void Model3D::display()
{
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    glEnableClientState(GL_VERTEX_ARRAY);


    glBindBuffer(GL_ARRAY_BUFFER, mVBO[M3D_VERTICES]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    if (mActive.normals)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, mVBO[M3D_NORMALS]);
        glNormalPointer(GL_FLOAT, 0, 0);
    }


    if (mActive.triangles > 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBO[M3D_TRIANGLES]);
        glDrawElements(GL_TRIANGLES, mActive.triangles, GL_UNSIGNED_INT, 0);
    }


    if (mActive.quads > 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mVBO[M3D_QUADS]);
        glDrawElements(GL_QUADS, mActive.quads, GL_UNSIGNED_INT, 0);
    }


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glPopClientAttrib();
}
