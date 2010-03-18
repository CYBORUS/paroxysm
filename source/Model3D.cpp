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
    vector<GLuint> normalWeirdTriangleIndices;
    vector<GLuint> normalWeirdQuadIndices;
    vector<GLfloat> vertices;
    vector<GLfloat> normals;
    vector<GLfloat> finalNormals;
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
            GLuint vn[4];

            int i = 0; // keep the count outside the for loop
            for (; i < 4 && !ss.fail() && !ss.eof(); ++i)
            {
                char slash;
                char slash2;
                ss >> v[i] >> slash >> slash2 >> vn[i];
                --v[i];
                --vn[i];
            }

            if (i == 3)
            {
                triangleIndices.push_back(v[0]);
                triangleIndices.push_back(v[1]);
                triangleIndices.push_back(v[2]);

                normalWeirdTriangleIndices.push_back(vn[0]);
                normalWeirdTriangleIndices.push_back(vn[1]);
                normalWeirdTriangleIndices.push_back(vn[2]);
            }
            else if (i == 4)
            {
                quadIndices.push_back(v[0]);
                quadIndices.push_back(v[1]);
                quadIndices.push_back(v[2]);
                quadIndices.push_back(v[3]);

                normalWeirdQuadIndices.push_back(vn[0]);
                normalWeirdQuadIndices.push_back(vn[1]);
                normalWeirdQuadIndices.push_back(vn[2]);
                normalWeirdQuadIndices.push_back(vn[3]);
            }
        }
        else
        {
            //cerr << "unknown key -- " << ss.str() << endl;
        }

        getline(modelFile, line);
    }

    modelFile.close();

    mVBO.loadVertexArray(PVBO_VERTEX, 3, vertices.size(), &vertices[0]);

    if (normals.size() > 0)
    {
        finalNormals.insert(finalNormals.begin(), normals.begin(), normals.end());
        normals.clear();
        normals.insert(normals.begin(), vertices.size(), 0.0f);

        for (int i = 0; i < normalWeirdTriangleIndices.size(); ++i)
        {
            normals[triangleIndices[i] * 3] = finalNormals[normalWeirdTriangleIndices[i] * 3];
            normals[triangleIndices[i] * 3 + 1] = finalNormals[normalWeirdTriangleIndices[i] * 3 + 1];
            normals[triangleIndices[i] * 3 + 2] = finalNormals[normalWeirdTriangleIndices[i] * 3 + 2];
        }

        for (int i = 0; i < normalWeirdQuadIndices.size(); ++i)
        {
            normals[quadIndices[i] * 3] = finalNormals[normalWeirdQuadIndices[i] * 3];
            normals[quadIndices[i] * 3 + 1] = finalNormals[normalWeirdQuadIndices[i] * 3 + 1];
            normals[quadIndices[i] * 3 + 2] = finalNormals[normalWeirdQuadIndices[i] * 3 + 2];
        }

        mVBO.loadVertexArray(PVBO_NORMAL, 3, normals.size(), &normals[0]);
    }

    if (triangleIndices.size() > 0)
    {
        mVBO.loadIndexArray(GL_TRIANGLES, triangleIndices.size(),
            &triangleIndices[0]);
    }

    if (quadIndices.size() > 0)
    {
        mVBO.loadIndexArray(GL_QUADS, quadIndices.size(), &quadIndices[0]);
    }

    mModels[inFile] = this;
}

Model3D::~Model3D()
{
}
