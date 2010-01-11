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

#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <iostream>
#include <string>
#include <map>

#define DEFAULT_VALUE "`"

class Config
{
    public:
        static void initialize(int inArgc, char** inArgv);
        static void loadFromFile(const char* inFile);
        static void outputSettings();
        static void outputSettings(std::ostream& inStream);
        static const char* get(const char* inKey, const char* inDefault);
        static const char* get(const char* inKey);
        static int getInt(const char* inKey, int inDefault);
        static int getInt(const char* inKey);

    private:
        static std::map<std::string, std::string> mSettings;
};

#endif

