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

#include <CYBORUS/c3m.h>

#include "ModelStack.h"
#include "LogFile.h"

map<string, ModelStack*> ModelStack::mModels;

ModelStack* ModelStack::load(const char* inFile)
{
    map<string, ModelStack*>::iterator i = mModels.find(inFile);

    if (i == mModels.end())
    {
        //cerr << "loading model " << inFile << endl;
        string s(inFile);
        s = s.substr(s.rfind('.') + 1);

        ModelStack* m = new ModelStack;

        if (s == "obj")
        {
            m->loadOBJ(inFile);
        }
        else if (s == "3ds")
        {
            m->load3DS(inFile);
        }
        else if (s == "c3m")
        {
            m->loadC3M(inFile);
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

void ModelStack::unloadAll()
{
    for (map<string, ModelStack*>::iterator i = mModels.begin();
        i != mModels.end(); ++i)
    {
        delete i->second;
    }

    mModels.clear();
}

ModelStack::ModelStack()
{
}

ModelStack::~ModelStack()
{
}

void ModelStack::load3DS(const char* inFile)
{
    //vector<GLfloat> vertices;
    //vector<GLfloat> indices; // all triangles

    LogFile log3DS;
    log3DS.start("3ds");

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

void ModelStack::loadOBJ(const char* inFile)
{
    //vector<GLuint> quadIndices;
    vector<GLuint> triangleIndices;
    vector<GLuint> normalWeirdTriangleIndices;
    vector<GLuint> textureWeirdTriangleIndices;
    //vector<GLuint> normalWeirdQuadIndices;
    vector<GLfloat> vertices;
    vector<GLfloat> colors;
    vector<GLfloat> normals;
    vector<GLfloat> finalNormals;
    vector<GLfloat> textureCoords;
    vector<GLfloat> finalTexCoords;


    Vector3D<GLfloat> currentColor;

    map<string, Vector3D<GLfloat>* > materialColors;

    bool useNormals = false; //determine if normals were in the file
    bool useTextures = false; //determine if textures coordinates were in the file
    bool useColors = false; //are we using colors with this?
    bool useAlpha = false; //if we have an alpha bit to push on with the current color

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

        if (key == "v")
        {
            GLfloat p;
            ss >> p;
            vertices.push_back(p);

            if (p < mBoundingBox.minX)
            {
                mBoundingBox.minX = p;
            }
            else if (p > mBoundingBox.maxX)
            {
                mBoundingBox.maxX = p;
            }

            ss >> p;
            vertices.push_back(p);

            if (p < mBoundingBox.minY)
            {
                mBoundingBox.minY = p;
            }
            else if (p > mBoundingBox.maxY)
            {
                mBoundingBox.maxY = p;
            }

            ss >> p;
            vertices.push_back(p);

            if (p < mBoundingBox.minZ)
            {
                mBoundingBox.minZ = p;
            }
            else if (p > mBoundingBox.maxZ)
            {
                mBoundingBox.maxZ = p;
            }
        }
        else if (key == "vn")
        {
            useNormals = true;
            Vector3D<GLfloat> v;

            for (int i = 0; i < 3; ++i) ss >> v[i];

            v.normalize();

            for (int i = 0; i < 3; ++i) normals.push_back(v[i]);
        }
        else if (key == "vt")
        {
            useTextures = true;

                GLfloat p;
                ss >> p;

                textureCoords.push_back(p);

                ss >> p;

                //we need to invert the y axis, we think this is because
                //of sdl image
                p = 1.0f - p;

                textureCoords.push_back(p);

        }
        else if (key == "f")
        {
            GLuint v[4];
            GLuint vn[4];
            GLuint vt[4];
            char slash;

            int i = 0; // keep the count outside the for loop

            //there should always be at least 3 coordinates
            for (; i < 3 && !ss.fail() && !ss.eof(); ++i)
            {
                ss >> v[i];
                --v[i];

                if (useTextures && useNormals)
                {
                    ss >> slash >> vt[i] >> slash >> vn[i];
                    --vt[i];
                    --vn[i];
                }
                else if (useTextures)
                {
                    ss >> slash >> vt[i];
                    --vt[i];

                }
                else if (useNormals)
                {
                    ss >> slash >> slash >> vn[i];
                    --vn[i];
                }
            }

            //now see if there's a fourth coordinate, defining a quad instead
            //of a triangle
            ss >> v[i];

            if (!ss.fail())
            {
                --v[i];

                if (useTextures && useNormals)
                {
                    ss >> slash >> vt[i] >> slash >> vn[i];
                    --vt[i];
                    --vn[i];
                }
                else if (useTextures)
                {
                    ss >> slash >> vt[i];
                    --vt[i];

                }
                else if (useNormals)
                {
                    ss >> slash >> slash >> vn[i];
                    --vn[i];
                }

                ++i;
            }

            //set the colors
            if (useAlpha)
            {
                for (int j = 0; j < 3; ++j)
                {
                    int k = v[j] * 4;

                    colors[k] = currentColor[0];
                    colors[k + 1] = currentColor[1];
                    colors[k + 2] = currentColor[2];
                    colors[k + 3] = currentColor[3];
                }
            }
            else
            {
                for (int j = 0; j < 3; ++j)
                {
                    int k = v[j] * 3;

                    colors[k] = currentColor[0];
                    colors[k + 1] = currentColor[1];
                    colors[k + 2] = currentColor[2];
                }
            }

            //push on the first triangle
            triangleIndices.push_back(v[0]);
            triangleIndices.push_back(v[1]);
            triangleIndices.push_back(v[2]);

            normalWeirdTriangleIndices.push_back(vn[0]);
            normalWeirdTriangleIndices.push_back(vn[1]);
            normalWeirdTriangleIndices.push_back(vn[2]);

            textureWeirdTriangleIndices.push_back(vt[0]);
            textureWeirdTriangleIndices.push_back(vt[1]);
            textureWeirdTriangleIndices.push_back(vt[2]);

            //the face was a quad, we need to form a second triangle
            if (i == 4)
            {
                //first we need to push a color on for the fourth vertex
                if (useAlpha)
                {
                    int k = v[3] * 4;

                    colors[k] = currentColor[0];
                    colors[k + 1] = currentColor[1];
                    colors[k + 2] = currentColor[2];
                    colors[k + 3] = currentColor[3];
                }
                else
                {
                    int k = v[3] * 3;

                    colors[k] = currentColor[0];
                    colors[k + 1] = currentColor[1];
                    colors[k + 2] = currentColor[2];
                }



                triangleIndices.push_back(v[0]);
                triangleIndices.push_back(v[2]);
                triangleIndices.push_back(v[3]);

                normalWeirdTriangleIndices.push_back(vn[0]);
                normalWeirdTriangleIndices.push_back(vn[2]);
                normalWeirdTriangleIndices.push_back(vn[3]);

                textureWeirdTriangleIndices.push_back(vt[0]);
                textureWeirdTriangleIndices.push_back(vt[2]);
                textureWeirdTriangleIndices.push_back(vt[3]);

            }
        }
        else if (key == "mtllib")
        {
            useColors = true;

            ifstream materialsFile;

            //run through the materials file and map all the materials we need
            string materials;
            ss >> materials;
            materials = "assets/models/" + materials;

            materialsFile.open(materials.c_str());

            if (materialsFile.fail())
            {
                cerr << "failed to load " << materials << endl;
                exit(30);
            }
            string nextLine;
            string nextMaterial;
            Vector3D<GLfloat> nextColor;

            getline(materialsFile, nextLine);

            while (!materialsFile.eof())
            {
                stringstream buffer;
                string subKey;
                //bool foundNext = false;

                buffer << nextLine;
                buffer >> subKey;

                if (subKey == "newmtl")
                {
                    buffer >> nextMaterial;
                }
                else if (subKey == "Kd")
                {
                    buffer >> nextColor[0] >> nextColor[1] >> nextColor[2];
                }
                else if (subKey == "d" || subKey == "Tr")
                {
                    useAlpha = true;
                    buffer >> nextColor[3];
                }
                else if (subKey == "illum")
                {
                    //if we hit this, we're ready to store the color
                    materialColors[nextMaterial] = new Vector3D<GLfloat>(nextColor[0], nextColor[1], nextColor[2], nextColor[3]);
                }
                else if (subKey == "map_Kd")
                {
                    string texFile;
                    buffer >> texFile;

                    texFile = "assets/images/models/" + texFile;
                    glGenTextures(1, &mTexture);

                    DisplayEngine::loadTexture(texFile.c_str(), mTexture);
                }

                getline(materialsFile, nextLine);
            }
            materialsFile.close();
        }
        else if (key == "usemtl")
        {
            string currentMaterial;
            ss >> currentMaterial;

            currentColor = *(materialColors[currentMaterial]);

            //set the color vector to be the same size as the vertices vector
            Vector3D<GLfloat> white(1.0f, 1.0f, 1.0f);
            if (useAlpha)
            {
                //if there's an alpha bit in our colors, this becomes a little
                //more complicated
                while (colors.size() < (vertices.size() / 3 * 4))
                {
                    colors.push_back(white[0]);
                    colors.push_back(white[1]);
                    colors.push_back(white[2]);
                    colors.push_back(white[3]);
                }

            }
            else
            {
                while (colors.size() < vertices.size())
                {
                    colors.push_back(white[0]);
                    colors.push_back(white[1]);
                    colors.push_back(white[2]);
                }

            }
        }
        else if (key == "s")
        {
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

        for (size_t i = 0; i < normalWeirdTriangleIndices.size(); ++i)
        {
            normals[triangleIndices[i] * 3]
                = finalNormals[normalWeirdTriangleIndices[i] * 3];
            normals[triangleIndices[i] * 3 + 1]
                = finalNormals[normalWeirdTriangleIndices[i] * 3 + 1];
            normals[triangleIndices[i] * 3 + 2]
                = finalNormals[normalWeirdTriangleIndices[i] * 3 + 2];
        }

        mVBO.loadVertexArray(PVBO_NORMAL, 3, normals.size(), &normals[0]);
    }

    if (colors.size() > 0)
    {
        //we need to clear out the colors map
        map<string, Vector3D<GLfloat>* >::iterator it = materialColors.begin();

        for (; it != materialColors.end(); ++it)
        {
            delete it->second;
        }

        materialColors.clear();

        int colorStride = 3;

        if (useAlpha)
        {
            colorStride = 4;
        }

        mVBO.loadVertexArray(PVBO_COLOR, colorStride, colors.size(), &colors[0]);
    }

    if (textureCoords.size() > 0)
    {
        finalTexCoords.insert(finalTexCoords.begin(), textureCoords.begin(), textureCoords.end());
        textureCoords.clear();
        textureCoords.insert(textureCoords.begin(), vertices.size() / 3 * 2, 0.0f);

        for (size_t i = 0; i < textureWeirdTriangleIndices.size(); ++i)
        {
            textureCoords[triangleIndices[i] * 2]
                = finalTexCoords[textureWeirdTriangleIndices[i] * 2];
            textureCoords[triangleIndices[i] * 2 + 1]
                = finalTexCoords[textureWeirdTriangleIndices[i] * 2 + 1];
        }
        mVBO.loadVertexArray(PVBO_TEXTURE, 2, textureCoords.size(), &textureCoords[0]);
    }

    if (triangleIndices.size() > 0)
    {
        mVBO.loadIndexArray(GL_TRIANGLES, triangleIndices.size(),
            &triangleIndices[0]);
    }

    mModels[inFile] = this;
}

void ModelStack::loadC3M(const char* inFile)
{
    string file = "assets/models/";
    file += inFile;
    c3mModel* c3m = c3mOpen(file.c_str());
    if (c3mError)
    {
        //DisplayEngine::printErrors("c3m loader error");
        return;
    }


    int size = c3m->vertices.size;
    GLfloat* data = c3m->vertices.array;

    mVBO.loadVertexArray(PVBO_VERTEX, 3, size, data);


    size = c3m->normals.size;
    data = c3m->normals.array;

    mVBO.loadVertexArray(PVBO_NORMAL, 3, size, data);

    size = c3m->colors.size;
    if (size > 0)
    {
        data = c3m->colors.array;
        mVBO.loadVertexArray(PVBO_COLOR, 4, size, data);
    }


    size = c3m->textureCoordinates.size;
    if (size > 0)
    {
        data = c3m->textureCoordinates.array;
        mVBO.loadVertexArray(PVBO_TEXTURE, 2, size, data);

        glGenTextures(1, &mTexture);

        string texFile = "assets/images/models/";
        texFile += c3m->textureFile;

        DisplayEngine::loadTexture(texFile.c_str(), mTexture);
    }
    else
    {
        cerr << "error, no texture" << endl;
    }


    size = c3m->indices.size;
    GLuint* indices = c3m->indices.array;
    mVBO.loadIndexArray(GL_TRIANGLES, size, indices);

    c3mClose(c3m);


}
