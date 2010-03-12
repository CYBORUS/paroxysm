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

LogFile::LogFile(const char* inTitle)
{
    string logsDir = "assets/logs/";

    //load the list up with all the maps in the maps directory
    if (is_directory(logsDir))
    {
        for (directory_iterator itr(logsDir); itr != directory_iterator(); ++itr)
        {
            //cout << itr->path().filename() << ' ';
            if (is_regular_file(itr->status()))
            {
                //delete any log files that are more than 10 seconds old
                if (difftime(time(NULL), last_write_time(itr->path())) > 10)
                {
                    //maps->addListItem(itr->path().filename());
                    remove(itr->path());
                }
            }
        }
    }

    //remove_all("assets/logs");
    //create_

    char buffer[15];
    time_t rawtime;
    time(&rawtime);
    tm* timeinfo = localtime(&rawtime);
    strftime(buffer, 15, "%Y%m%d%H%M%S", timeinfo);


    string s("assets/logs/");
    s += inTitle;
    s += '-';
    s += buffer;
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

void LogFile::addLine(const string& inText)
{
    mStream << inText << endl;
}
