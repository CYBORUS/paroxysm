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

Model3D::Model3D(const char* inFile)
{
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
                mVertices.push_back(p);
            }
        }
        else if (key == "vn")
        {
            for (int i = 0; i < 3; ++i)
            {
                GLfloat p;
                ss >> p;
                mNormals.push_back(p);
            }
        }
        else if (key == "vt")
        {
            for (int i = 0; i < 2; ++i)
            {
                GLfloat p;
                ss >> p;
                mTextureCoords.push_back(p);
            }
        }
        else if (key == "f")
        {
            Vector3D<float> v;

            int i; // keep the count outside the for loop
            for (i = 0; !ss.fail() && !ss.eof(); ++i)
            {
                ss >> v[i];
            }
        }
        else
        {
            cerr << "unknown key -- " << key << endl;
        }

        getline(modelFile, line);
    }

    modelFile.close();
}

Model3D::~Model3D()
{
}
