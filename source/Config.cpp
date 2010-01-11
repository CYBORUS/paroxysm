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

#include "Config.h"

#include <iostream>
#include <fstream>

map<string, string> Config::mSettings;

void trim(string& inString)
{
    size_t startPos = inString.find_first_not_of(" \t");
    size_t endPos = inString.find_last_not_of(" \t");

    if (startPos == string::npos || endPos == string::npos)
        inString = "";
    else
        inString = inString.substr(startPos, endPos - startPos + 1);
}

void Config::initialize(int inArgc, char** inArgv)
{
    loadFromFile(inArgc > 1 ? inArgv[1] : "settings.txt");
}

void Config::loadFromFile(const char* inFile)
{
    ifstream settingsFile;
    settingsFile.open(inFile);
    if (settingsFile.fail())
    {
        cerr << "failed to open " << inFile << endl;
        return;
    }

    string s;

    while (getline(settingsFile, s))
    {
        size_t eq = s.find_first_of('=');
        if (s.length() < 2 || s[0] == '`' || eq == string::npos) continue;

        string key = s.substr(0, eq);
        string value = s.substr(eq + 1);
        trim(key);
        trim(value);

        if (key.length() < 2 || value.length() < 1) continue;

        mSettings[key] = value;
    }

    settingsFile.close();
}

void Config::outputSettings()
{
    outputSettings(cout);
}

void Config::outputSettings(ostream& inStream)
{
    for (map<string, string>::iterator i = mSettings.begin();
        i != mSettings.end(); ++i)
        inStream << i->first << " = " << i->second << endl;
}

const char* Config::getRaw(const char* inKey, const char* inDefault)
{
    map<string, string>::iterator i = mSettings.find(inKey);

    if (i == mSettings.end())
    {
        mSettings[inKey] = inDefault;
        return inDefault;
    }

    return i->second.c_str();
}

const char* Config::getRaw(const char* inKey)
{
    return getRaw(inKey, DEFAULT_VALUE);
}
