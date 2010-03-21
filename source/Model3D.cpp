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
#include "LogFile.h"

map<string, Model3D*> Model3D::mModels;

Model3D* Model3D::load(const char* inFile)
{
    map<string, Model3D*>::iterator i = mModels.find(inFile);

    if (i == mModels.end())
    {
        cerr << "loading model " << inFile << endl;
        string s(inFile);
        s = s.substr(s.rfind('.') + 1);

        Model3D* m = new Model3D;

        if (s == "obj")
        {
            m->loadOBJ(inFile);
        }
        else if (s == "3ds")
        {
            m->load3DS(inFile);
        }
        else
        {
            cerr << "invalid 3D file extension -- " << s << endl;
            exit(1);
        }

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

Model3D::Model3D()
{
}

Model3D::~Model3D()
{
}

void Model3D::load3DS(const char* inFile)
{
    //vector<GLfloat> vertices;
    //vector<GLfloat> indices; // all triangles

    LogFile log3DS("m3d");

    string f("assets/models/");
    f += inFile;

    ifstream modelFile;
    modelFile.open(f.c_str());
    if (modelFile.fail())
    {
        cerr << "failed to open model: " << f << endl;
        return;
    }

    unsigned short chunkName;
    unsigned int chunkSize;
    unsigned int fileSize;
    readBytes(modelFile, chunkName);

    if (chunkName == 0x4d4d)
    {
        readBytes(modelFile, chunkSize);
        fileSize = chunkSize;
        cerr << "valid 3DS file: " << fileSize << " bytes" << endl;
    }
    else
    {
        cerr << "invalid 3DS file" << endl;
        modelFile.close();
        return;
    }

    while (modelFile.tellg() < fileSize)
    {
        readBytes(modelFile, chunkName);
        readBytes(modelFile, chunkSize);
        unsigned int dataSize = chunkSize - 6;

        switch (chunkName)
        {
            case 0x3d3d: // EDIT3DS
            case 0x4100:
            {
                // nothing to process; simply move onto first internal chunk
                break;
            }

            case 0x4000: // EDIT_OBJECT
            {
                string s;
                for (char c = modelFile.get(); c != '\0'; c = modelFile.get())
                {
                    s += c;
                }
                log3DS.addLine(s);
                break;
            }

            case 0x4110: // TRI_VERTEXL
            {
                unsigned short numVertices;
                readBytes(modelFile, numVertices);
                for (unsigned short i = 0; i < numVertices; ++i)
                {
                    union {
                        unsigned int raw;
                        float result;
                    } capture;

                    stringstream ss;
                    ss << "vertex #" << i << " :";

                    for (unsigned short j = 0; j < 3; ++j)
                    {
                        readBytes(modelFile, capture.raw);
                        ss << ' ' << capture.result;
                    }
                    log3DS.addLine(ss.str());
                }
                break;
            }

            case 0x4120: // TRI_FACEL1
            {
                unsigned short numIndices;
                readBytes(modelFile, numIndices);
                for (unsigned short i = 0; i < numIndices; ++i)
                {
                    stringstream ss;
                    ss << "face #" << i << " :";
                    for (unsigned short j = 0; j < 4; ++j)
                    {
                        unsigned short stuff;
                        readBytes(modelFile, stuff);
                        ss << ' ' << stuff;
                    }
                    log3DS.addLine(ss.str());
                }
                break;
            }

            default:
            {
                modelFile.ignore(dataSize);
            }
        }
    }

    modelFile.close();
}

void Model3D::loadOBJ(const char* inFile)
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
                ss >> v[i] >> slash >> slash >> vn[i];
                --v[i];
                --vn[i];
            }

            i = 3; // overriding triangles only... quads are not working right
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

        for (unsigned int i = 0; i < normalWeirdTriangleIndices.size(); ++i)
        {
            normals[triangleIndices[i] * 3]
                = finalNormals[normalWeirdTriangleIndices[i] * 3];
            normals[triangleIndices[i] * 3 + 1]
                = finalNormals[normalWeirdTriangleIndices[i] * 3 + 1];
            normals[triangleIndices[i] * 3 + 2]
                = finalNormals[normalWeirdTriangleIndices[i] * 3 + 2];
        }

        for (unsigned int i = 0; i < normalWeirdQuadIndices.size(); ++i)
        {
            cerr << "yarr? " << i << " : " << quadIndices[i] << endl;
            normals[quadIndices[i] * 3]
                = finalNormals[normalWeirdQuadIndices[i] * 3];
            normals[quadIndices[i] * 3 + 1]
                = finalNormals[normalWeirdQuadIndices[i] * 3 + 1];
            normals[quadIndices[i] * 3 + 2]
                = finalNormals[normalWeirdQuadIndices[i] * 3 + 2];
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
