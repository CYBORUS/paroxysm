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

#include "LogFile.h"

#include <string>

LogFile::LogFile(const char* inTitle)
{
    char buffer[15];
    time_t rawtime;
    time(&rawtime);
    tm* timeinfo = localtime(&rawtime);
    strftime(buffer, 15, "%Y%m%d%H%M%S", timeinfo);
    mTimestamp = buffer;

    string s("assets/logs/");
    s += inTitle;
    s += '-';
    s += mTimestamp;
    s += ".txt";

    mStream.open(s.c_str());
}

LogFile::~LogFile()
{
    if (mStream.is_open()) mStream.close();
}

void LogFile::addLine(const char* inText)
{
    mStream << inText << endl;
}